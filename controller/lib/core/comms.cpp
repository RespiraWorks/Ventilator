#include "comms.h"

#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include "algorithm.h"
#include "checksum.h"
#include "framing.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "uart_dma.h"

// extern UART_DMA dmaUART;
// extern FramingRxFSM rxFSM;

// Note that the initial value of last_tx has to be invalid; changing it to 0
// wouldn't work.  We immediately transmit on boot, and after
// we do that, we want to wait a full TX_INTERVAL_MS.  If we initialized
// last_tx to 0 and our first transmit happened at time millis() == 0, we
// would set last_tx back to 0 and then retransmit immediately.
inline bool Comms::is_time_to_transmit() {
  return (last_tx == kInvalidTime || Hal.now() - last_tx > TX_INTERVAL);
}

inline bool Comms::is_transmitting() { return uart_dma.isTxInProgress(); }

void Comms::onTxComplete() {}

void Comms::onTxError() {}

// Serializes current controller status, adds crc and escapes it.
// The resulting frame is written into tx buffer.
// Returns the length of the resulting frame.
uint32_t Comms::createFrame(const ControllerStatus &controller_status) {
  uint8_t pb_buffer[ControllerStatus_size + 4];

  pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));
  if (!pb_encode(&stream, ControllerStatus_fields, &controller_status)) {
    // TODO: Serialization failure; log an error or raise an alert.
    return 0;
  }
  uint32_t pb_data_len = (uint32_t)(stream.bytes_written);

  uint32_t crc32 = Hal.crc32(pb_buffer, pb_data_len);
  if (!append_crc(pb_buffer, pb_data_len, sizeof(pb_buffer), crc32)) {
    // TODO log an error, output buffer too small
  }

  return encodeFrame(pb_buffer, pb_data_len + 4, tx_buffer, TX_BUF_LEN);
}

void Comms::process_tx(const ControllerStatus &controller_status) {
  // Serialize our current state into the buffer if
  //  - we're not currently transmitting,
  //  - it's been a while since we last transmitted.

  if (!is_transmitting() && is_time_to_transmit()) {
    uint32_t frame_len = createFrame(controller_status);
    if (frame_len > 0) {
      uart_dma.startTX(tx_buffer, frame_len, this);
      last_tx = Hal.now();
    } else {
      // TODO log an error
    }
  }

  // TODO: Alarm if we haven't been able to send a status in a certain amount
  // of time.
}

inline bool is_crc_pass(uint8_t *buf, uint32_t len) {
  return Hal.crc32(buf, len - 4) == extract_crc(buf, len);
}
#include <stdio.h>
void Comms::process_rx(GuiStatus *gui_status) {
  if (rxFSM.isDataAvailable()) {
    uint8_t *buf = rxFSM.getReceivedBuf();
    uint32_t len = rxFSM.getReceivedLength();
    uint32_t decoded_length = decodeFrame(buf, len, buf, len);
    if (is_crc_pass(buf, decoded_length)) {
      pb_istream_t stream = pb_istream_from_buffer(buf, decoded_length - 4);
      GuiStatus new_gui_status = GuiStatus_init_zero;
      if (pb_decode(&stream, GuiStatus_fields, &new_gui_status)) {
        *gui_status = new_gui_status;
      } else {
        // TODO: Log an error.
      }
      last_rx = Hal.now();
    } else {
      // TODO CRC mismatch; log an error
    }
  }
}

void Comms::init() { rxFSM.begin(); }

void Comms::handler(const ControllerStatus &controller_status,
                    GuiStatus *gui_status) {
  process_tx(controller_status);
  process_rx(gui_status);
}
