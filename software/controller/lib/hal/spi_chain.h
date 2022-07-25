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

#pragma once

#include "circular_buffer.h"
#include "serial_listeners.h"
#include "spi.h"
#include "system_timer.h"
#include "vars.h"

namespace SPI {

// SPI slaves can be setup in a daisy chain configuration, meaning that the output of the
// first one is linked to the input of the second one, and so on, the last slave's output is
// linked to the input of the master(see
// https://en.wikipedia.org/wiki/Serial_Peripheral_Interface#Daisy_chain_configuration).
//
// While the chip_select pin is low, slaves simply copy whatever data they received to their
// output line after one clock cycle, and when the chip select pin transitions to high, they
// consider the last data they received as theirs, and set their output accordingly.
//
// With this setup, we talk to all the slaves at once, but in as many clock cycles as there are
// slaves, by keeping the chip select low as we send one word per slave.
// For convenience in this implementation, we have numbered the slaves in reverse order, so that
// byte 0 of the send buffer is destined for slave 0, but that slave is technically the last in
// the chain.
// Also, we need to distribute their responses to the managing entities accordingly, as the
// answers are tangled, for example after sending commands to three slaves s0, s1, s2, we will
// receive [s0 resp[0], s1 resp[0], s2 resp[0], s0 resp[1], s1 resp[1], s2 resp[1], ...]

// Structure that represents a request that can be queued up of sending to one of the slaves in
// the SPI daisy chain.  Note that we only support 8 bits transmission for now.
struct Request {
  uint8_t *command{nullptr};
  size_t length{0};            // (in bytes)
  uint8_t *response{nullptr};  // Place the caller wants us to put the response
                               // Note that because of the way a daisy chain works, the response
                               // must be one byte shorter than the command and the caller must
                               // reserve at least that much space for the response.
                               // In practice, this means the caller needs to fill the command
                               // to sizeof(expected_response) + 1 with nullcommand bytes.
  bool *processed{nullptr};    // boolean we set to inform the caller that the request is processed
};

template <size_t MaxSlaves, size_t MaxRequestsPerSlave>
class DaisyChain : public RxListener {
 public:
  DaisyChain(const char *name, const char *help_supplement, Channel *spi,
             Duration min_cs_high_time);

  void ProbeSlaves(uint8_t null_command, uint8_t reset_command);

  bool SendRequest(const Request &request, size_t slave);

  size_t num_slaves() { return num_slaves_; }

  void on_rx_complete() override;
  void on_rx_error(RxError) override{};
  void on_character_match() override{};

  // Functions used for probing, made public for testing purposes: if we do not have the hardware
  // in the loop, SendDataWithBusyWait will send the transmission but never get a response.
  // To alleviate this, the ProbeSlaves method is split at each step where a response is
  // needed, so the test can call these functions individually and insert the response on the
  // spi bus in between.
  void SetNullCommand(uint8_t null_command);
  void FlushSlavesData();
  void ResetSlaves(uint8_t reset_command, uint8_t *response_buffer, size_t length);
  void ParseProbeResponse(uint8_t *response_buffer, size_t length);

 private:
  Channel *spi_;

  // Number of slaves that are actually present in the chain, determined during initilization
  size_t num_slaves_{0};

  // null command we fill the send buffer with when there is nothing to send to a slave
  uint8_t null_command_{0};

  // Transmission status
  bool transmitting_data_{false};

  void TransmitNextCommand();

  // When a command is queued, its data is stored in a local buffer since the scope of the data
  // being pointed to is not certain.
  // Length of 20 bytes per slave is arbitrary but should suffice for stepper motors, which take
  // short commands (length <= 4).
  // Note that this buffer is shared between slaves
  static constexpr size_t CommandBufferSize{20 * MaxSlaves};
  size_t command_buffer_count_{0};

  uint8_t command_buffer_[CommandBufferSize] = {0};

  // We need to handle as many requests queues as we have slaves. We are using normal arrays to do
  // this as the requests should not stack up, and we should be able to empty the queues beween
  // cycles of the high priority loop.
  Request request_queue_[MaxSlaves][MaxRequestsPerSlave];
  size_t queue_count_[MaxSlaves] = {0};
  size_t current_request_[MaxSlaves] = {0};
  size_t command_index_[MaxSlaves] = {0};
  bool end_of_request_[MaxSlaves] = {false};
  size_t response_count_[MaxSlaves] = {0};
  bool save_response_[MaxSlaves] = {false};

  // Buffers used to transmit/receive data from the SPI peripheral
  uint8_t send_buffer_[MaxSlaves] = {0};
  uint8_t receive_buffer_[MaxSlaves] = {0};

  // Minimum time between bytes when transmitting (this depends on the nature of the slaves)
  Duration min_cs_high_time_;
  // Time when CS was last set to high
  Time last_cs_rise_{microsSinceStartup(0)};

  void ProcessReceivedData();
  void SendDataWithBusyWait(uint8_t *command_buffer, uint8_t *response_buffer, size_t length);

  Request *GetCurrentRequest(size_t slave);
  void AdvanceToNextRequest(size_t slave);

  Debug::Variable::UInt32 queueing_errors_{"queuing_errors", Debug::Variable::Access::ReadOnly, 0,
                                           "", "Queueing error counter"};
};

}  // namespace SPI

#include "spi_chain.tpp"
