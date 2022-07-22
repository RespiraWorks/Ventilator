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

#include "mock_spi_chain.h"

#include "gtest_main.h"

using namespace SPI;


// Helper function that enqueues a SPI and updates the expected spi flow (if provided,
// along with the number of slaves in the chain)
template <size_t N>
void EnqueueRequestAndTestResult(DaisyChain<N> *chain, Request &request, size_t slave,
                                 bool expected_result, uint8_t *expected_flow = nullptr,
                                 size_t num_slaves = 0) {

  EXPECT_EQ(chain->SendRequest(request, slave), expected_result);
  if(expected_result){
    EXPECT_FALSE(*(request.processed));

    if(expected_flow){
      for(size_t byte = 0 ; byte < request.length ; byte++){
        expected_flow[byte*num_slaves] = request.command[byte];
      }
    }
  }
}

// Helper function that checks that the daisy chain state matches the expected one.
template <size_t N>
void CheckDaisyState(MockChain<N> *chain, uint8_t *expected, size_t num_slaves) {
  uint8_t chain_state[num_slaves] = {0};
  chain->GetChainState(chain_state, num_slaves);
  for(size_t slave = 0 ; slave < num_slaves ; slave++){
    SCOPED_TRACE("slave=" + std::to_string(slave));
    EXPECT_EQ(chain_state[slave], expected[slave]);
  }
}

TEST(SpiDaisyChain, RequestQueue) {
  // This test assumes at least 2 slaves.
  constexpr size_t NumSlaves{2};
  // Create a daisy chain with more potential than actual slaves. This is especially useful in this
  // test because it allows us to have extra space in the command buffer.
  MockChain<NumSlaves*2> daisy_chain{NumSlaves, 0};
  
  // I need to block the transmission in order for the requests to be queued, otherwise (with mock
  // class), the requests are processed as soon as I call SendRequest, and don't really get queued
  daisy_chain.BlockTransmission();

  // Initialize a long command array from where requests will arbitrarily pick commands
  uint8_t command[256];
  for(size_t i=0 ; i < sizeof(command) ; i++){
    // using multiples of 7 so that the array (which is modulo 256) never repeats
    command[i] = static_cast<uint8_t>(i ); //* 7);
  }
  // We are sending requests of length 3, which is quite small but longer than most requests for
  // stepper motors.
  size_t request_length{3};

  // Reserve space for max_queue_length+1 processed bytes
  bool processed[(daisy_chain.max_queue_length()+1)*NumSlaves];
  for (size_t req = 0 ; req < sizeof(processed)/sizeof(processed[0]) ; req++) {
    processed[req] = true;
  }

  // We will build the expected spi flow along the way
  uint8_t expected_spi_flow[256] = {0};
  size_t slave_0_index{0};
  size_t slave_1_index{1};

  // Fill the queue to slave 0
  size_t request_number{0};
  for(size_t i = 0 ; i < daisy_chain.max_queue_length() ; i++){
    Request request{
        .command = &(command[1+request_number*request_length]),
        .length = request_length,
        .response = nullptr, // I don't care about the response for now, this tests the queueing
        .processed = &processed[request_number],
    };
    SCOPED_TRACE("slave 0, request " + std::to_string(i));

    EnqueueRequestAndTestResult(&daisy_chain, request, /*slave=*/0, /*expected_result=*/true,
                                &(expected_spi_flow[slave_0_index]), NumSlaves);
    slave_0_index += NumSlaves * request.length;
    request_number++;
  }

  // Add a few requests to slave 1 as well
  for(size_t i = 0 ; i < 3 ; i++){
    Request request{
        .command = &(command[1+request_number*request_length]),
        .length = request_length,
        .response = nullptr,
        .processed = &processed[request_number],
    };
    SCOPED_TRACE("slave 1, request " + std::to_string(i));

    EnqueueRequestAndTestResult(&daisy_chain, request, /*slave=*/1, /*expected_result=*/true,
                                &(expected_spi_flow[slave_1_index]), NumSlaves);
    slave_1_index += NumSlaves * request.length;
    request_number++;
  }

  // One more request for fail tests
  Request extra_request{
    .command = &(command[request_number*3+1]),
    .length = 1,
    .response = nullptr,
    .processed = &processed[request_number],
  };

  // The queue for slave 0 is full
  EnqueueRequestAndTestResult(&daisy_chain, extra_request, /*slave=*/0, /*expected_result=*/false);

  // Enqueuing to an extra slave should fail as well
  EnqueueRequestAndTestResult(&daisy_chain, extra_request, NumSlaves+1, /*expected_result=*/false);

  // Proving that that request was fine by enqueuing it to slave 1
  EnqueueRequestAndTestResult(&daisy_chain, extra_request, /*slave=*/1, /*expected_result=*/true,
                              &(expected_spi_flow[slave_1_index]), NumSlaves);
  slave_1_index += NumSlaves * extra_request.length;
  request_number++;

  // Enqueueing to slave 1 from now on now, which should be fine unless the command is NOK
  // Empty command
  extra_request.processed = &processed[request_number];
  extra_request.length = 0;
  EnqueueRequestAndTestResult(&daisy_chain, extra_request, /*slave=*/1, /*expected_result=*/false);

  // Invalid command
  extra_request.length = 0;
  extra_request.command = nullptr;
  EnqueueRequestAndTestResult(&daisy_chain, extra_request, /*slave=*/1, /*expected_result=*/false);


  // Command too long 
  extra_request.length = daisy_chain.FreeCommandBuffer() + 1;
  extra_request.command = command;
  EnqueueRequestAndTestResult(&daisy_chain, extra_request, /*slave=*/1, /*expected_result=*/false);

  // Making this slightly shorter to be able to push it to the queue for slave 1
  extra_request.length = extra_request.length-1;
  EnqueueRequestAndTestResult(&daisy_chain, extra_request, /*slave=*/1, /*expected_result=*/true,
                              &(expected_spi_flow[slave_1_index]), NumSlaves);
  
  slave_1_index += NumSlaves * extra_request.length;
  request_number++;

  // Dequeueing and checking that all requests have been procesed, as well as the resulting spi flow.
  uint8_t spi_flow[sizeof(expected_spi_flow)] = {0};
  daisy_chain.ExportSpiFlow(spi_flow, sizeof(spi_flow));

  daisy_chain.StartTransmission();

  for(size_t i = 0 ; i < request_number ; i++){
    SCOPED_TRACE("request " + std::to_string(i) + "/" + std::to_string(request_number-1));
    EXPECT_TRUE(processed[i]);
  }
  
  for(size_t byte = 0 ; byte < sizeof(spi_flow) ; byte++){
    SCOPED_TRACE("byte=" + std::to_string(byte));
    EXPECT_EQ(spi_flow[byte], expected_spi_flow[byte]);
  }
}

TEST(SpiDaisyChain, SingleRequestAnySlave) {
  constexpr size_t NumSlaves{4};
  constexpr uint8_t NullCommand{0x20};
  // Create a daisy chain with slightly more potential than actual slaves
  MockChain<NumSlaves+1> daisy_chain{NumSlaves, NullCommand};

  // Check initial state of the chain (all zeros)
  {
    uint8_t expected[NumSlaves] = {0};
    CheckDaisyState(&daisy_chain, expected, NumSlaves);
  }

  for(size_t target_slave = 0 ; target_slave < NumSlaves ; target_slave++){
    SCOPED_TRACE("target_slave=" + std::to_string(target_slave));
    // Process one request and check: 
    // - the state of the daisy chain after each byte sent
    // - the processed boolean
    // - the contents of the response
    uint8_t command[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t response[3] = {0};
    bool processed{false};
    
    Request request{
      .command = command,
      .length = sizeof(command),
      .response = response,
      .processed = &processed,
    };

    daisy_chain.BlockInterrupts();
    EXPECT_TRUE(daisy_chain.SendRequest(request, target_slave));
    
    size_t byte=0;
    do {
      SCOPED_TRACE("byte=" + std::to_string(byte));
      // check that each slave has received the correct byte
      uint8_t expected[NumSlaves];
      for(size_t slave=0;slave<NumSlaves;slave++) {
        expected[slave]=NullCommand;
      }
      expected[target_slave] = command[byte++];
      CheckDaisyState(&daisy_chain, expected, NumSlaves);
    } while (daisy_chain.TransmitNextBytes());

    // Check the final state of the chain
    {
      uint8_t expected[NumSlaves];
      for(size_t slave=0;slave<NumSlaves;slave++) {
        expected[slave]=NullCommand;
      }
      expected[target_slave] = command[sizeof(command)-1];
      CheckDaisyState(&daisy_chain, expected, NumSlaves);
    }

    EXPECT_TRUE(processed);
    // We don't actually simulate any behaviour from the slaves, the response we get is a copy of
    // the command (minus its last byte, which we won't get before the next call to SendRequest,
    // and will be discarded as the first byte received from the next request).
    for(size_t i = 0 ; i < sizeof(response) ; i++){
      SCOPED_TRACE("i=" + std::to_string(i));
      EXPECT_EQ(response[i], command[i]);
    }

  }
}

// Macro definition we are using to automate the next test a little better
#define QUEUE_NEXT_REQUEST(target) \
transmitting[target] = true; \
request = { \
  .command = command[next_command], \
  .length = sizeof(command[next_command]), \
  .response = response[next_command], \
  .processed = &processed[next_command] \
}; \
daisy_chain.SendRequest(request, target);

TEST(SpiDaisyChain, SendDuringTransmission) {
  constexpr size_t NumSlaves{3};
  constexpr uint8_t NullCommand{0x20};
  MockChain<NumSlaves> daisy_chain{NumSlaves, NullCommand};
  daisy_chain.BlockInterrupts();
  
  // going to send 5 commands to different slaves 
  uint8_t command[5][5] = {{0xC0, 0xFF, 0xEE, 0xBA, 0xBE},
                           {0x12, 0x34, 0x56, 0x78, 0x90},
                           {0xC0, 0xDE, 0xBA, 0x5E, 0x28},
                           {0xAC, 0xDC, 0x57, 0x68, 0x70},
                           {0xAB, 0xCD, 0xEF, 0xFE, 0x0F}};
  uint8_t response[5][4] = {0};
  bool processed[5] = {false};

  size_t next_command{0};
  Request request;

  // Maintain the current state of the chain
  bool transmitting[NumSlaves] = {false};
  size_t current_request[NumSlaves] = {0};
  size_t next_byte[NumSlaves] = {0};
  uint8_t expected[NumSlaves] = {0};

  QUEUE_NEXT_REQUEST(0)
  current_request[0] = next_command++;

  while(!processed[4]){
    SCOPED_TRACE("last queued request " + std::to_string(next_command-1));
    // update and check expected state
    for (size_t slave = 0 ; slave < NumSlaves ; slave++) {
      expected[slave] = transmitting[slave] ? command[current_request[slave]][next_byte[slave]++]
                                            : NullCommand;
      if(next_byte[slave] >= sizeof(command[0])){
        next_byte[slave] = 0;
        transmitting[slave] = false;
      }
    }
    
    // Queue next request on certain conditions:
    // The requests are organized as follow, with time on a horizontal axis, each char is a byte:
    // slave0: req_0req_3
    // slave1:   req_1 req4
    // slave2:    req_2
    if(next_command==1 && next_byte[0]==2){
      QUEUE_NEXT_REQUEST(1)
      current_request[1] = next_command++;
    }
    if(next_command==2 && next_byte[0]==2){
      QUEUE_NEXT_REQUEST(2)
      current_request[2] = next_command++;
    }
    if(next_command==3 && next_byte[0]==4){
      QUEUE_NEXT_REQUEST(0)
      next_command++;
    }
    if(next_command==4 && next_byte[2]==0){
      QUEUE_NEXT_REQUEST(1)
      current_request[1] = next_command++;
    }

    // Enventual queueing should have changed nothing to the spi line: we are already transmitting
    CheckDaisyState(&daisy_chain, expected, NumSlaves);

    // Since I queued command[3] to slave 0 while we were transmitting to that same slave, I must
    // manually set some states that were prematurely reset by the for loop above.
    if(next_command==4 && next_byte[0]==0){
      transmitting[0] = true;
      current_request[0] = 3;
    }

    // Transmit next set of bytes to the daisy chain
    daisy_chain.TransmitNextBytes();
  }

  // check responses, like with previous test, equal to the commands, minus its last byte:
  for(size_t req = 0 ; req < sizeof(response)/sizeof(response[0]) ; req++){
    SCOPED_TRACE("req=" + std::to_string(req));
    for(size_t i = 0 ; i < sizeof(response[0]) ; i++){
      SCOPED_TRACE("i=" + std::to_string(i));
      EXPECT_EQ(response[req][i], command[req][i]);
    }
  }

}

TEST(SpiDaisyChain, ProbeSlaves) {
  constexpr size_t NumSlaves{5};
  constexpr uint8_t NullCommand{0};
  MockChain<NumSlaves> daisy_chain{NumSlaves, NullCommand};
  
  for(size_t num_slaves=0; num_slaves<=NumSlaves ; num_slaves++){
    EXPECT_EQ(daisy_chain.TestProbeSlaves(num_slaves, NullCommand, 0x20),num_slaves);
  }
}