/* Copyright 2022, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "spi_chain.h"

#include "circular_buffer.h"
#include "gtest_main.h"
#include "mock_spi.h"

using namespace SPI;

// This class encapsulates the whole setup for daisy chain tests to make initialization simpler
// and emulates the behavior of a physical daisy chain when sending next bytes.
template <size_t MaxSlaves, size_t Q>
class TestEnvironment {
 public:
  TestEnvironment(size_t num_slaves, uint8_t null_command) {
    chain_.SetNullCommand(null_command);
    // Set num slaves in the daisy chain to the desired number:
    // first set a probe response accordingly
    uint8_t probe_response[MaxSlaves + 1] = {0};
    for (size_t i = 0; i < MaxSlaves + 1; i++) {
      if (i < num_slaves) {
        probe_response[i] = null_command;
      } else {
        probe_response[i] = null_command ^ 1;
      }
    }
    // Parse that probe response
    chain_.ParseProbeResponse(probe_response, sizeof(probe_response));
    // setup the mock spi to copy its tx buffer to slaves_states when spi_.SendNextcommand() is
    // called for the first time.
    spi_.TestSetSpiFlowTarget(slaves_state_, chain_.num_slaves());
  };

  MockChannel spi_;
  DaisyChain<MaxSlaves, Q> chain_{"test", "for tests", &spi_, microseconds(0)};

  bool SendRequest(Request &request, size_t slave) { return chain_.SendRequest(request, slave); }

  void SendNextBytes() {
    // Queue the current slaves_state to the spi rx data so when chain_.on_rx_complete() calls
    // spi_.SetupReception(), it will copy the slaves state to the receive buffer
    spi_.QueueReceiveData(slaves_state_, chain_.num_slaves());
    // Set the spi flow target to slaves state so that when chain_.on_rx_complete() calls
    // spi_.SendNextcommand(), the set of bytes is copied to slaves_state.
    // This is safe because it happens after spi_.SetupReception(), so SetupReception will have
    // copied the proper slaves_state to the receive buffer
    spi_.TestSetSpiFlowTarget(slaves_state_, chain_.num_slaves());
    // Finally, call chain_.on_rx_complete() to trigger all of the things we have just setup.
    chain_.on_rx_complete();
  }

  uint8_t slaves_state_[MaxSlaves] = {0};
};

TEST(SpiDaisyChain, CannotSendInvalidRequests) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{1};
  constexpr size_t MaxSlaves{NumSlaves + 1};
  TestEnvironment<MaxSlaves, 1> test_chain{NumSlaves, 0};

  // Cannot queue a request with nullptr command
  Request request{.command{nullptr}, .length{1}, .response{nullptr}, .processed{nullptr}};
  EXPECT_FALSE(test_chain.SendRequest(request, 0));

  // Cannot queue a request with length 0
  uint8_t command[20 * MaxSlaves] = {0};
  request.command = command;
  request.length = 0;
  EXPECT_FALSE(test_chain.SendRequest(request, 0));

  // Cannot queue a request to a non-existant slave (even if slave < MaxSlave )
  request.length = 1;
  EXPECT_FALSE(test_chain.SendRequest(request, NumSlaves));
}

TEST(SpiDaisyChain, CannotSendWhenQueueFull) {
  // Setup a simple daisy chain with queue length = 1
  constexpr size_t NumSlaves{1};
  constexpr size_t MaxSlaves{NumSlaves};
  TestEnvironment<MaxSlaves, 1> test_chain{NumSlaves, 0};

  uint8_t command{0};
  Request request{.command{&command}, .length{1}, .response{nullptr}, .processed{nullptr}};

  EXPECT_TRUE(test_chain.SendRequest(request, 0));
  // Queue is now full, we cannot send any more stuff
  EXPECT_FALSE(test_chain.SendRequest(request, 0));
}

TEST(SpiDaisyChain, CannotSendWithFullCommandBuffer) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{2};
  constexpr size_t MaxSlaves{NumSlaves};
  TestEnvironment<MaxSlaves, 2> test_chain{NumSlaves, 0};

  uint8_t command[20 * MaxSlaves] = {0};
  Request request{.command{command}, .length{1}, .response{nullptr}, .processed{nullptr}};

  EXPECT_TRUE(test_chain.SendRequest(request, 1));
  // Cannot queue a request that is too long to fit the command buffer
  // Assumes size of 20 bytes/slave with one byte already in the command buffer
  request.length = 20 * MaxSlaves;
  EXPECT_FALSE(test_chain.SendRequest(request, 0));

  // But it is OK with one less byte
  request.length--;
  EXPECT_TRUE(test_chain.SendRequest(request, 0));
}

TEST(SpiDaisyChain, DequeueingWithMultipleSlaves) {
  // Setup a simple daisy chain with queue length = 2 and one less slave than its max capacity
  constexpr size_t NumSlaves{2};
  constexpr size_t QueueLength{2};
  constexpr uint8_t NullCommand{0xBF};
  TestEnvironment<NumSlaves + 1, QueueLength> test_chain{NumSlaves, NullCommand};

  // We are sending requests of length 4, the longest requests for stepper motors.
  size_t request_length{4};

  // Initialize a command array from where requests will arbitrarily pick commands
  uint8_t command[NumSlaves * QueueLength * request_length];
  for (size_t i = 0; i < sizeof(command); i++) {
    command[i] = static_cast<uint8_t>(i);
  }
  // Reserve space for processed bytes
  bool processed[NumSlaves * QueueLength + 1] = {false};

  // Fill the request queues
  size_t num_request{0};
  for (size_t slave = 0; slave < NumSlaves; slave++) {
    for (size_t i = 0; i < QueueLength; i++) {
      Request request{
          .command = &(command[num_request * request_length]),
          .length = request_length,
          .response = nullptr,  // I don't care about the response for now, this tests the queues
          .processed = &processed[num_request],
      };
      test_chain.SendRequest(request, slave);
      num_request++;
    }
  }

  // Check that queue to slave 0 is now full
  Request extra_request{
      .command = &(command[0]),
      .length = request_length,
      .response = nullptr,
      .processed = &processed[num_request],
  };
  EXPECT_FALSE(test_chain.SendRequest(extra_request, 0));

  size_t byte{0};
  size_t bytes_per_slave{QueueLength * request_length};
  do {
    SCOPED_TRACE("Byte: " + std::to_string(byte));
    // The first slave is one byte ahead of the others, as the first call to SendRequest
    // triggered the sending of the first byte to the first slave, with other queues empty
    // meaning other slaves will have their command filled with NullCommand on the first byte,
    // while the first slave will have its command filled with NullCommand on the last byte.
    if (byte < bytes_per_slave) {
      EXPECT_EQ(test_chain.slaves_state_[0], command[byte]);
    } else {
      EXPECT_EQ(test_chain.slaves_state_[0], NullCommand);
    }
    for (size_t slave = 1; slave < NumSlaves; slave++) {
      SCOPED_TRACE("Slave: " + std::to_string(slave));
      if (byte > 0) {
        EXPECT_EQ(test_chain.slaves_state_[slave], command[slave * bytes_per_slave + byte - 1]);
      } else {
        EXPECT_EQ(test_chain.slaves_state_[slave], NullCommand);
      }
    }
    test_chain.SendNextBytes();
    byte++;
  } while (byte <= bytes_per_slave);

  // The final call to SendNextBytes() flagged all the request as processed while not sending
  // anything to the spi line, therefore not changing the slaves state
  for (size_t i = 0; i < NumSlaves * QueueLength; i++) {
    EXPECT_TRUE(processed[i]);
  }
  EXPECT_EQ(test_chain.slaves_state_[0], NullCommand);
  for (size_t slave = 1; slave < NumSlaves; slave++) {
    SCOPED_TRACE("Slave: " + std::to_string(slave));
    EXPECT_EQ(test_chain.slaves_state_[slave],
              command[slave * bytes_per_slave + bytes_per_slave - 1]);
  }

  // Queue to slave 0 is no longer full
  EXPECT_TRUE(test_chain.SendRequest(extra_request, 0));
}
