#include "comms.h"

#include "algorithm.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

// Our outgoing (serialized) ControllerStatus proto is stored in tx_buffer.  We
// then transmit it a few bytes at a time, as the serial port becomes
// available.
//
// This isn't a circular buffer; the beginning of the proto is always at the
// beginning of the buffer.
static uint8_t tx_buffer[ControllerStatus_size];
// Index of the next byte to transmit.
static uint16_t tx_idx = 0;
// Number of bytes remaining to transmit. tx_idx + tx_bytes_remaining equals
// the size of the serialized ControllerStatus proto.
static uint16_t tx_bytes_remaining = 0;

// Time when we started sending the last ControllerStatus.
// TODO: Change this to std::optional<Time> once that's available; then we
// don't need this "clever" initialization.
constexpr Time kInvalidTime = millisSinceStartup(0xFFFF'FFFF'FFFF'FFFFUL);
static Time last_tx = kInvalidTime;

// Our incoming (serialized) GuiStatus proto is incrementally buffered in
// rx_buffer until it's complete and we can deserialize it to a proto.
//
// Like tx_buffer, this isn't a circular buffer; the beginning of the proto is
// always at the beginning of the buffer.
static uint8_t rx_buffer[GuiStatus_size];
static uint16_t rx_idx = 0;
static Time last_rx = Hal.now();
static bool rx_in_progress = false;

// We currently lack proper message framing, so we use a timeout to determine
// when the GUI is done sending us its message.
static constexpr Duration RX_TIMEOUT = milliseconds(1);

// We send a ControllerStatus every TX_INTERVAL_MS.

// In Alpha build we use synchronized communication initiated by GUI cycle
// controller. Since both ControllerStatus and GuiStatus take roughly 300+
// bytes, we need at least 1/115200.*10*300=26ms to transmit.
static constexpr Duration TX_INTERVAL = milliseconds(30);

void comms_init() {}

static bool is_time_to_process_packet() {
  return Hal.now() - last_rx > RX_TIMEOUT;
}

// NOTE this is work in progress.
// Proper framing incomming. Afproto will be used to encode data to form that
// can be safely sent over wire - with packet start/end markers and CRC

// TODO add frame markers
// TODO add marker escaping in contents
// TODO add CRC to whole packet

// TODO run this via DMA to free up resources for control loops
static void process_tx(const ControllerStatus &controller_status) {
  auto bytes_avail = Hal.serialBytesAvailableForWrite();
  if (bytes_avail == 0) {
    return;
  }

  // Serialize our current state into the buffer if
  //  - we're not currently transmitting,
  //  - we can transmit at least one byte now, and
  //  - it's been a while since we last transmitted.
  //
  // Note that the initial value of last_tx has to be invalid; changing it to 0
  // wouldn't work.  We immediately transmit on boot, and after
  // we do that, we want to wait a full TX_INTERVAL_MS.  If we initialized
  // last_tx to 0 and our first transmit happened at time millis() == 0, we
  // would set last_tx back to 0 and then retransmit immediately.
  if (tx_bytes_remaining == 0 &&
      (last_tx == kInvalidTime || Hal.now() - last_tx > TX_INTERVAL)) {
    // Serialize current status into output buffer.
    //
    // TODO: Frame the message bytes.
    // TODO: Add a checksum to the message.
    pb_ostream_t stream = pb_ostream_from_buffer(tx_buffer, sizeof(tx_buffer));
    if (!pb_encode(&stream, ControllerStatus_fields, &controller_status)) {
      // TODO: Serialization failure; log an error or raise an alert.
      return;
    }
    tx_idx = 0;
    tx_bytes_remaining = static_cast<uint16_t>(stream.bytes_written);
    last_tx = Hal.now();
  }

  // TODO: Alarm if we haven't been able to send a status in a certain amount
  // of time.

  // Send bytes over the wire if any are in our buffer.
  if (tx_bytes_remaining > 0) {
    // TODO(jlebar): Change serialWrite to take a uint8* instead of a char*, so
    // it matches nanopb.
    auto bytes_written =
        Hal.serialWrite(reinterpret_cast<char *>(tx_buffer) + tx_idx,
                        stl::min(bytes_avail, tx_bytes_remaining));
    // TODO: How paranoid should we be about this underflowing?  Perhaps we
    // should reset the device if this or other invariants are violated?
    tx_bytes_remaining -= bytes_written;
    tx_idx += bytes_written;
  }
}

static void process_rx(GuiStatus *gui_status) {
  while (Hal.serialBytesAvailableForRead() > 0) {
    rx_in_progress = true;
    char b;
    uint16_t bytes_read = Hal.serialRead(&b, 1);
    if (bytes_read == 1) {
      rx_buffer[rx_idx++] = (uint8_t)b;
      if (rx_idx >= sizeof(rx_buffer)) {
        rx_idx = 0;
        break;
      }
      last_rx = Hal.now();
    }
  }

  // TODO do away with timeout-based reception once we have framing in place,
  // but it will work for Alpha build for now
  if (rx_in_progress && is_time_to_process_packet()) {
    pb_istream_t stream = pb_istream_from_buffer(rx_buffer, rx_idx);
    GuiStatus new_gui_status = GuiStatus_init_zero;
    if (pb_decode(&stream, GuiStatus_fields, &new_gui_status)) {
      *gui_status = new_gui_status;
    } else {
      // TODO: Log an error.
    }
    rx_idx = 0;
    rx_in_progress = false;
  }
}

void comms_handler(const ControllerStatus &controller_status,
                   GuiStatus *gui_status) {
  process_tx(controller_status);
  process_rx(gui_status);
}
