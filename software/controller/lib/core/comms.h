/* Copyright 2020-2021, RespiraWorks

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

#include "frame_detector.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "proto_traits.h"
#include "rx_buf_uart_dma.h"
#include "uart_dma.h"
#include "units.h"

// This module periodically sends messages to the GUI device and receives
// messages from the GUI.  The only way it communicates with other modules is
// by modifying the gui_status pointer in CommsHandler.

// Size of the rx buffer is set assuming a corner case where EVERY GuiStatus
// byte and CRC32 will be escaped + two marker chars; this is too big, but
// safe.
static constexpr uint32_t RxFrameLengthMax{ProtoTraits<GuiStatus>::MaxFrameSize};

extern UartDma uart_dma;

extern RxBufferUartDma<RxFrameLengthMax> rx_buffer;
extern FrameDetector<RxBufferUartDma<RxFrameLengthMax>, RxFrameLengthMax> frame_detector;

class Comms : public TxListener {
  UartDma &uart_dma_;
  FrameDetector<RxBufferUartDma<RxFrameLengthMax>, RxFrameLengthMax> &frame_detector_;

 public:
  Comms();
  // Returns true on successful initialization, false otherwise
  [[nodiscard]] bool init();
  void on_tx_complete() override;
  void on_tx_error() override;
  // `controller_status` should be the controller's current status.  It's sent
  // periodically to the GUI.  When we receive a message from the GUI, we update
  // gui_status accordingly.
  void handler(const ControllerStatus &controller_status, GuiStatus *gui_status);

 private:
  bool is_time_to_transmit();
  bool is_transmitting();
  void process_tx(const ControllerStatus &controller_status);
  void process_rx(GuiStatus *gui_status);

  // Our outgoing frame (ControllerStatus proto serialized, CRC'ed and escaped)
  // is stored in tx_buffer.  We then give it to DMA-UART to transmit.
  //
  // This isn't a circular buffer; the beginning of the frame is always at the
  // beginning of the buffer.
  // Size of the buffer is set assuming a corner case where EVERY
  // ControllerStatus byte and CRC32 will be escaped + two marker chars; this is
  // too big, but safe.
  static constexpr uint32_t TxBufferLength{ProtoTraits<ControllerStatus>::MaxFrameSize};
  uint8_t tx_buffer_[TxBufferLength] = {0};

  // Time when we started sending the last ControllerStatus.
  // TODO: Change this to std::optional<Time> once that's available; then we
  // don't need this "clever" initialization.
  static constexpr Time InvalidTime{microsSinceStartup(0xFFFF'FFFF'FFFF'FFFFUL)};
  Time last_tx_{InvalidTime};

  // Our incoming (serialized) GuiStatus proto is incrementally buffered in
  // rx_buffer until it's complete, and we can deserialize it to a proto
  //
  // Like tx_buffer, this isn't a circular buffer; the beginning of the proto is
  // always at the beginning of the buffer.
  uint8_t rx_buffer_[GuiStatus_size] = {0};
  uint16_t rx_index_{0};
  bool rx_in_progress_{false};
  Time last_rx_;

  // We currently lack proper message framing, so we use a timeout to determine
  // when the GUI is done sending us its message.
  static constexpr Duration RxTimeout{milliseconds(1)};

  // We send a ControllerStatus every kTxInterval.

  // In Alpha build we use synchronized communication initiated by GUI cycle
  // controller. Since both ControllerStatus and GuiStatus take roughly 300+
  // bytes, we need at least 1/115200.*10*300=26ms to transmit.
  static constexpr Duration TxInterval{milliseconds(30)};
};
