#include "comms.h"

#include "algorithm.h"
#include "framing.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

extern UART_DMA dmaUART;

void Comms::init() {}

bool Comms::is_time_to_process_packet() {
  return Hal.now() - last_rx > RX_TIMEOUT;
}

// NOTE this is work in progress.
// Proper framing incomming. Afproto will be used to encode data to form that
// can be safely sent over wire - with packet start/end markers and CRC

// TODO add frame markers
// TODO add marker escaping in contents
// TODO add CRC to whole packet

// Note that the initial value of last_tx has to be invalid; changing it to 0
// wouldn't work.  We immediately transmit on boot, and after
// we do that, we want to wait a full TX_INTERVAL_MS.  If we initialized
// last_tx to 0 and our first transmit happened at time millis() == 0, we
// would set last_tx back to 0 and then retransmit immediately.
inline bool Comms::is_time_to_transmit() {
  return (last_tx == kInvalidTime || Hal.now() - last_tx > TX_INTERVAL);
}

inline bool Comms::is_transmitting() { return dmaUART.isTxInProgress(); }

void onTxComplete() {}

void onTxError() {}

// Adds CRC of the dataLength of data bytes in the buf at the end of the buf
inline void add_crc(uint8_t *buf, uint32_t dataLength) {
  uint32_t crc32 = Hal.crc32(buf, dataLength);
  buf[dataLength] = static_cast<uint8_t>((crc32 >> 24) & 0x000000FF);
  buf[dataLength + 1] = static_cast<uint8_t>((crc32 >> 16) & 0x000000FF);
  buf[dataLength + 2] = static_cast<uint8_t>((crc32 >> 8) & 0x000000FF);
  buf[dataLength + 3] = static_cast<uint8_t>(crc32 & 0x000000FF);
}

void Comms::process_tx(const ControllerStatus &controller_status) {
  // Serialize our current state into the buffer if
  //  - we're not currently transmitting,
  //  - we can transmit at least one byte now, and
  //  - it's been a while since we last transmitted.
  //
  if (!is_transmitting() && is_time_to_transmit()) {
    // Serialize current status into output buffer.

    uint8_t pb_buffer[ControllerStatus_size + 4];
    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));
    if (!pb_encode(&stream, ControllerStatus_fields, &controller_status)) {
      // TODO: Serialization failure; log an error or raise an alert.
      return;
    }
    add_crc(pb_buffer, stream.bytes_written);

    uint32_t encodedLength =
        encodeFrame(pb_buffer, stream.bytes_written + 4, tx_buffer, TX_BUF_LEN);
    if (encodedLength > 0) {
      dmaUART.startTX(tx_buffer, encodedLength);
    }
    last_tx = Hal.now();
  }

  // TODO: Alarm if we haven't been able to send a status in a certain amount
  // of time.
}

void Comms::process_rx(GuiStatus *gui_status) {
  // while (Hal.serialBytesAvailableForRead() > 0) {
  //   rx_in_progress = true;
  //   char b;
  //   uint16_t bytes_read = Hal.serialRead(&b, 1);
  //   if (bytes_read == 1) {
  //     rx_buffer[rx_idx++] = (uint8_t)b;
  //     if (rx_idx >= sizeof(rx_buffer)) {
  //       rx_idx = 0;
  //       break;
  //     }
  //     last_rx = Hal.now();
  //   }
  // }

  // TODO do away with timeout-based reception once we have framing in place,
  // but it will work for Alpha build for now
  // if (rx_in_progress && is_time_to_process_packet()) {
  //   pb_istream_t stream = pb_istream_from_buffer(rx_buffer, rx_idx);
  //   GuiStatus new_gui_status = GuiStatus_init_zero;
  //   if (pb_decode(&stream, GuiStatus_fields, &new_gui_status)) {
  //     *gui_status = new_gui_status;
  //   } else {
  //     // TODO: Log an error.
  //   }
  //   rx_idx = 0;
  //   rx_in_progress = false;
  // }
}

void Comms::handler(const ControllerStatus &controller_status,
                    GuiStatus *gui_status) {
  process_tx(controller_status);
  process_rx(gui_status);
}
