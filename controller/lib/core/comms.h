/* Copyright 2020, Edwin Chiu

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

#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>

#include "framing_rx_fsm.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "uart_dma.h"
#include "units.h"

// This module periodically sends messages to the GUI device and receives
// messages from the GUI.  The only way it communicates with other modules is
// by modifying the gui_status pointer in comms_handler.

class Comms : public UART_DMA_TxListener {
public:
  UART_DMA &uart_dma;
  FramingRxFSM rxFSM;
  Comms(UART_DMA &uart_dma)
      : uart_dma(uart_dma), rxFSM(FramingRxFSM(uart_dma)){};
  void init();
  void onTxComplete() override;
  void onTxError() override;
  // `controller_status` should be the controller's current status.  It's sent
  // periodically to the GUI.  When we receive a message from the GUI, we update
  // gui_status accordingly.
  void handler(const ControllerStatus &controller_status,
               GuiStatus *gui_status);
#ifdef TEST_MODE
  void test_PutRxBuffer(uint8_t *buf, uint32_t len);
#endif

private:
  bool is_time_to_transmit();
  bool is_transmitting();
  void process_tx(const ControllerStatus &controller_status);
  void process_rx(GuiStatus *gui_status);
  uint32_t createFrame(const ControllerStatus &controller_status);

  // Our outgoing frame (ControllerStatus proto serialized, crc'd and escaped)
  // is stored in tx_buffer.  We then give it to DMA-UART to transmit.
  //
  // This isn't a circular buffer; the beginning of the frame is always at the
  // beginning of the buffer.
  // Size of the buffer is set asuming a corner case where EVERY
  // ControllerStatus byte and CRC32 will be escaped + two marker chars; this is
  // too big, but safe.
  static constexpr uint32_t TX_BUF_LEN = (ControllerStatus_size + 4) * 2 + 2;
  uint8_t tx_buffer[TX_BUF_LEN];

  // Time when we started sending the last ControllerStatus.
  // TODO: Change this to std::optional<Time> once that's available; then we
  // don't need this "clever" initialization.
  static constexpr Time kInvalidTime =
      millisSinceStartup(0xFFFF'FFFF'FFFF'FFFFUL);
  Time last_tx = kInvalidTime;

  // Our incoming (serialized) GuiStatus proto is incrementally buffered in
  // rx_buffer until it's complete and we can deserialize it to a proto.
  //
  // Like tx_buffer, this isn't a circular buffer; the beginning of the proto is
  // always at the beginning of the buffer.
  uint8_t rx_buffer[GuiStatus_size];
  uint16_t rx_idx = 0;
  Time last_rx = Hal.now();
  bool rx_in_progress = false;

  // We currently lack proper message framing, so we use a timeout to determine
  // when the GUI is done sending us its message.
  static constexpr Duration RX_TIMEOUT = milliseconds(1);

  // We send a ControllerStatus every TX_INTERVAL_MS.

  // In Alpha build we use synchronized communication initiated by GUI cycle
  // controller. Since both ControllerStatus and GuiStatus take roughly 300+
  // bytes, we need at least 1/115200.*10*300=26ms to transmit.
  static constexpr Duration TX_INTERVAL = milliseconds(30);
};

#endif // COMMS_H
