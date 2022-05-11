/* Copyright 2020-2022, RespiraWorks
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

#include <stdint.h>

#include "network_protocol.pb.h"
#include "uart.h"

// This module periodically sends messages to the GUI device and receives
// messages from the GUI.  The only way it communicates with other modules is
// by modifying the gui_status pointer in CommsHandler.
class Comms {
 public:
  explicit Comms(UART::Channel *uart) : uart_(uart){};

  // `controller_status` should be the controller's current status.  It's sent
  // periodically to the GUI.  When we receive a message from the GUI, we update
  // gui_status accordingly.
  void Handler(const ControllerStatus &controller_status, GuiStatus *gui_status);

 private:
  UART::Channel *uart_;
  // Our outgoing (serialized) ControllerStatus proto is stored in tx_buffer.  We
  // then transmit it a few bytes at a time, as the serial port becomes
  // available.
  //
  // This isn't a circular buffer; the beginning of the proto is always at the
  // beginning of the buffer.
  uint8_t tx_buffer_[ControllerStatus_size] = {0};
  // Index of the next byte to transmit.
  uint16_t tx_idx_{0};
  // Number of bytes remaining to transmit. tx_idx + tx_bytes_remaining equals
  // the size of the serialized ControllerStatus proto.
  uint16_t tx_bytes_remaining_{0};

  // Time when we started sending the last ControllerStatus.
  std::optional<Time> last_tx_{std::nullopt};

  // Our incoming (serialized) GuiStatus proto is incrementally buffered in
  // rx_buffer until it's complete and we can deserialize it to a proto.
  //
  // Like tx_buffer, this isn't a circular buffer; the beginning of the proto is
  // always at the beginning of the buffer.
  uint8_t rx_buffer_[GuiStatus_size] = {0};
  uint16_t rx_idx_{0};
  Time last_rx_{microsSinceStartup(0)};
  bool rx_in_progress_{false};

  // We currently lack proper message framing, so we use a timeout to determine
  // when the GUI is done sending us its message.
  Duration RxTimeout{milliseconds(1)};

  // We send a ControllerStatus every TX_INTERVAL_MS.

  // In Alpha build we use synchronized communication initiated by GUI cycle
  // controller. Since both ControllerStatus and GuiStatus take roughly 300+
  // bytes, we need at least 1/115200.*10*300=26ms to transmit.
  Duration TxInterval{milliseconds(30)};

  bool IsTimeToProcessPacket();
  void ProcessTx(const ControllerStatus &controller_status);
  void ProcessRx(GuiStatus *gui_status);
};
