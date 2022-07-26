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

#include "gtest/gtest.h"
#include "mock_spi.h"
#include "spi_chain.h"

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
    // Have the daisy chain parse that probe response
    chain_.ParseProbeResponse(probe_response, sizeof(probe_response));

    // Setup the mock spi to copy its tx buffer to slaves_states when spi_.SendNextcommand() is
    // called for the first time.  At this point I don't bother queueing the initial slaves_state
    // to the rx data because it will get discarded anyway.
    spi_.TestSetSpiFlowTarget(slaves_state_, chain_.num_slaves());
  };

  MockChannel spi_;
  // setting min cs high time to 5 microseconds
  static constexpr Duration MinCSHighTime{microseconds(5)};
  DaisyChain<MaxSlaves, Q> chain_{"test", "for tests", &spi_, MinCSHighTime};

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

TEST(DaisyChain, CannotSendToInvalidSlave) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{1};
  constexpr size_t MaxSlaves{NumSlaves + 1};
  TestEnvironment<MaxSlaves, 1> test_chain{NumSlaves, 0};

  // Cannot queue a request to a non-existant slave (even if slave < MaxSlave )
  uint8_t command{0};
  Request request{.command{&command}, .length{1}};
  EXPECT_FALSE(test_chain.SendRequest(request, NumSlaves));
}

TEST(DaisyChain, ForwardQueueRejection) {
  // Setup a simple daisy chain with queue length = 1
  constexpr size_t NumSlaves{1};
  TestEnvironment<NumSlaves, 1> test_chain{NumSlaves, 0};

  uint8_t command{0};
  Request request{.command{&command}, .length{1}};

  EXPECT_TRUE(test_chain.SendRequest(request, 0));
  // Queue is now full, we cannot send any more stuff
  EXPECT_FALSE(test_chain.SendRequest(request, 0));
}

TEST(DaisyChain, CannotSendWithFullCommandBuffer) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{2};
  constexpr size_t MaxSlaves{NumSlaves};
  TestEnvironment<MaxSlaves, 2> test_chain{NumSlaves, 0};

  uint8_t command[20 * MaxSlaves] = {0};
  Request request{.command{command}, .length{1}};

  EXPECT_TRUE(test_chain.SendRequest(request, 1));
  // Cannot queue a request that is too long to fit the command buffer
  // Assumes size of 20 bytes/slave with one byte already in the command buffer
  request.length = 20 * MaxSlaves;
  EXPECT_FALSE(test_chain.SendRequest(request, 0));

  // But it is OK with one less byte
  request.length--;
  EXPECT_TRUE(test_chain.SendRequest(request, 0));
}

TEST(DaisyChain, FlushSlavesDataSendsNullCommands) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{5};
  constexpr size_t MaxSlaves{NumSlaves + 1};
  constexpr uint8_t NullCommand{0xBF};
  TestEnvironment<MaxSlaves, 1> test_chain{NumSlaves, NullCommand};

  // setup the spi mock to send its flow to a buffer 1 byte longer than the expected flow
  // assumes the spi_chain is setup for at most 5 response bytes to its longest command
  uint8_t spi_flow[MaxSlaves * 5 + 1] = {0};
  test_chain.spi_.TestSetSpiFlowTarget(spi_flow, sizeof(spi_flow));
  test_chain.chain_.FlushSlavesData();

  for (size_t i = 0; i < sizeof(spi_flow) - 1; i++) {
    EXPECT_EQ(spi_flow[i], NullCommand);
  }
  EXPECT_EQ(spi_flow[sizeof(spi_flow) - 1], 0);
}

TEST(DaisyChain, ResetSlavesSendsDesiredByte) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{5};
  constexpr size_t MaxSlaves{NumSlaves + 4};
  constexpr uint8_t ResetCommand{0x42};
  TestEnvironment<MaxSlaves, 1> test_chain{NumSlaves, 0};

  constexpr size_t ResetLength{3};
  constexpr size_t BufferLength{ResetLength + 1};
  uint8_t spi_flow[BufferLength + 1] = {0};
  // setup a response buffer
  uint8_t response[BufferLength] = {0};
  // queue dummy data to the spi flow
  uint8_t rx_data[BufferLength];
  for (size_t i = 0; i < BufferLength; i++) {
    rx_data[i] = static_cast<uint8_t>(i + 7);
  }
  test_chain.spi_.QueueReceiveData(rx_data, BufferLength);

  // setup the mock spi to send its flow to our buffer, then send a reset command that is 1 byte
  // shorter
  test_chain.spi_.TestSetSpiFlowTarget(spi_flow, BufferLength);
  test_chain.chain_.ResetSlaves(ResetCommand, response, ResetLength);

  for (size_t i = 0; i < ResetLength; i++) {
    EXPECT_EQ(spi_flow[i], ResetCommand);
    EXPECT_EQ(response[i], rx_data[i]);
  }
  EXPECT_EQ(spi_flow[ResetLength], 0);
  EXPECT_EQ(response[ResetLength], 0);
}

TEST(DaisyChain, ProbeSlavesIntegrationTest) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{4};
  constexpr size_t MaxSlaves{NumSlaves + 2};
  constexpr uint8_t NullCommand{0xBF};
  constexpr uint8_t ResetCommand{0x42};

  TestEnvironment<MaxSlaves, 1> test_chain{NumSlaves, NullCommand};

  // setup spi mock to send data to spi_flow
  uint8_t spi_flow[MaxSlaves * 6 + 2] = {0};
  test_chain.spi_.TestSetSpiFlowTarget(spi_flow, sizeof(spi_flow));

  // setup spi mock to receive representative data
  uint8_t rx_flow[MaxSlaves * 6 + 2] = {0};
  // first NumSlaves bytes are 0, then we have 5*MaxSlaves NullCommand
  size_t byte = NumSlaves;
  for (size_t i = 0; i < MaxSlaves * 5; i++) {
    rx_flow[byte++] = NullCommand;
  }
  // and finally filled with ResetCommand
  while (byte < sizeof(rx_flow)) {
    rx_flow[byte++] = ResetCommand;
  }
  test_chain.spi_.QueueReceiveData(rx_flow, sizeof(rx_flow));
  EXPECT_EQ(test_chain.chain_.ProbeSlaves(NullCommand, ResetCommand), NumSlaves);

  // check data sent on the spi bus:
  for (size_t i = 0; i < MaxSlaves * 5; i++) {
    SCOPED_TRACE("byte: " + std::to_string(i));
    EXPECT_EQ(spi_flow[i], NullCommand);
  }
  for (size_t i = MaxSlaves * 5; i < MaxSlaves * 6 + 1; i++) {
    SCOPED_TRACE("byte: " + std::to_string(i));
    EXPECT_EQ(spi_flow[i], ResetCommand);
  }
  EXPECT_EQ(spi_flow[MaxSlaves * 6 + 1], 0);
}

TEST(DaisyChain, EnsureMinCSHighTime) {
  // Setup a simple daisy chain
  constexpr size_t NumSlaves{1};
  TestEnvironment<NumSlaves, 1> test_chain{NumSlaves, 0};

  Time start = SystemTimer::singleton().now();
  // There is nothing in the queue, so on_rx_complete will set last_cs_rise_, but not delay during
  // call to TransmitNextCommand().
  test_chain.chain_.on_rx_complete();
  // wait a smaller duration than MinCSHighTime between calls
  SystemTimer::singleton().delay(test_chain.MinCSHighTime - microseconds(1));
  test_chain.chain_.EnsureMinCSHighTime();
  Time now = SystemTimer::singleton().now();
  // call to EnsureMinCSHighTime() makes it so we are now test_chain.MinCSHighTime after the call
  // to on_rx_complete()
  EXPECT_EQ(now, start + test_chain.MinCSHighTime);

  // Same setup, with a longer duration than MinCSHighTime between calls
  test_chain.chain_.on_rx_complete();
  SystemTimer::singleton().delay(test_chain.MinCSHighTime + microseconds(1));
  start = SystemTimer::singleton().now();
  test_chain.chain_.EnsureMinCSHighTime();
  now = SystemTimer::singleton().now();
  // call to EnsureMinCSHighTime() added no delay
  EXPECT_EQ(now, start);
}

TEST(DaisyChain, FullIntegrationTest) {
  // Setup a simple daisy chain with queue length = 2 and one less slave than its max capacity
  constexpr size_t NumSlaves{3};
  constexpr size_t QueueLength{2};
  constexpr uint8_t NullCommand{0xBF};
  TestEnvironment<NumSlaves + 1, QueueLength> test_chain{NumSlaves, NullCommand};

  // We are sending requests of length 4, the longest requests the controller currently uses.
  constexpr size_t RequestLength{4};
  constexpr size_t NumRequests{NumSlaves * QueueLength};

  // Initialize a command array from where requests will arbitrarily pick commands
  uint8_t commands[NumRequests][RequestLength];
  for (size_t i = 0; i < sizeof(commands); i++) {
    for (size_t j = 0; j < sizeof(commands[0]); j++)
      commands[i][j] = static_cast<uint8_t>(i * RequestLength + j);
  }
  // Reserve space for responses
  uint8_t responses[NumRequests][(RequestLength - 1)] = {0};
  // Reserve space for processed bytes
  bool processed[NumRequests] = {false};

  // Fill the request queues except the one to the last slave
  for (size_t num_request = 0; num_request < NumRequests - 1; num_request++) {
    Request request{
        .command = commands[num_request],
        .length = RequestLength,
        .response = responses[num_request],
        .processed = &processed[num_request],
    };
    test_chain.SendRequest(request, num_request % NumSlaves);
  }

  // Check that queue to slave 0 is now full
  Request extra_request{
      .command = commands[NumRequests - 1],
      .length = RequestLength,
      .response = responses[NumRequests],
      .processed = &processed[NumRequests],
  };
  EXPECT_FALSE(test_chain.SendRequest(extra_request, 0));

  size_t bytes_per_slave{RequestLength * QueueLength};
  for (size_t num_byte = 0; num_byte <= bytes_per_slave; num_byte++) {
    SCOPED_TRACE("Byte: " + std::to_string(num_byte));
    // The first slave is one byte ahead of the others, as the first call to SendRequest
    // triggered the sending of the first byte to the first slave, with other queues empty
    // meaning other slaves will have their command filled with NullCommand on the first byte,
    // while the first slave will have its command filled with NullCommand on the last byte.
    if (num_byte < bytes_per_slave) {
      size_t num_request{static_cast<size_t>(num_byte / RequestLength) * NumSlaves};
      EXPECT_EQ(test_chain.slaves_state_[0], commands[num_request][num_byte % RequestLength]);
    } else {
      EXPECT_EQ(test_chain.slaves_state_[0], NullCommand);
    }
    // other slaves
    for (size_t slave = 1; slave < NumSlaves; slave++) {
      SCOPED_TRACE("Slave: " + std::to_string(slave));
      size_t num_request{static_cast<size_t>((num_byte - 1) / RequestLength) * NumSlaves + slave};
      if (num_byte > 0 && num_request < NumRequests - 1) {
        EXPECT_EQ(test_chain.slaves_state_[slave],
                  commands[num_request][(num_byte - 1) % RequestLength]);
      } else {
        EXPECT_EQ(test_chain.slaves_state_[slave], NullCommand);
      }
    }
    test_chain.SendNextBytes();
  }

  // The final call to SendNextBytes() flagged all the request as processed while not sending
  // anything to the spi line, therefore not changing the slaves state
  for (size_t req = 0; req < NumRequests - 1; req++) {
    SCOPED_TRACE("Request: " + std::to_string(req));
    EXPECT_TRUE(processed[req]);
    // response is equal to the command, trimmed from its last byte
    for (size_t byte = 0; byte < RequestLength - 1; byte++) {
      SCOPED_TRACE("Byte: " + std::to_string(byte));
      EXPECT_EQ(responses[req][byte], commands[req][byte]);
    }
  }
  EXPECT_EQ(test_chain.slaves_state_[0], NullCommand);
  for (size_t slave = 1; slave < NumSlaves - 1; slave++) {
    SCOPED_TRACE("Slave: " + std::to_string(slave));
    EXPECT_EQ(test_chain.slaves_state_[slave],
              commands[NumRequests - NumSlaves + slave][RequestLength - 1]);
  }
  EXPECT_EQ(test_chain.slaves_state_[NumSlaves - 1], NullCommand);

  // We can queue again to slave 0
  EXPECT_TRUE(test_chain.SendRequest(extra_request, 0));
}
