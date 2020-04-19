#ifdef TEST_MODE
#include "serialIO-mock.h"
#else
#include "serialIO.h"
#include <Arduino.h>
#endif

#include "comms.h"
#include "network_protocol.pb.h"
#include "serdes.h"

#define PACKET_LEN_MAX (32)
static uint8_t tx_buffer[PACKET_LEN_MAX];
static uint16_t tx_data_length;
static bool output_buffer_ready = false;

static uint8_t rx_buffer[PACKET_LEN_MAX];
static uint8_t rx_idx = 0;
static uint64_t last_rx = 0;
static bool rx_in_progress = false;

static constexpr uint16_t RX_TIMEOUT_MS = 1;

void comms_init() { serialIO_init(); }

static bool is_time_to_process_packet() {
  return millis() - last_rx > RX_TIMEOUT_MS;
}

void comms_sendResetState() {
  // TODO
}

void comms_sendControllerStatus(ControllerStatus controller_status) {
  // TODO solve tx overflow
  if (output_buffer_ready) {
    return;
  }

  bool status = serdes_encode_status_packet(
      controller_status, tx_buffer, PACKET_LEN_MAX, (size_t *)&tx_data_length);
  if (status) {
    output_buffer_ready = true;
  }
}

// NOTE this is work in progress.
// Proper framing incomming. Afproto will be used to encode data to form that
// can be safely sent over wire - with packet start/end markers and CRC

// TODO add frame markers
// TODO add marker escaping in contents
// TODO add CRC to whole packet

// TODO run this via DMA to free up resources for control loops
void process_tx() {
  if (output_buffer_ready) {
    for (size_t i = 0; i < tx_data_length; i++) {
      serialIO_send(tx_buffer[i]);
    }
    output_buffer_ready = false;
  }
}

void process_rx() {
  while (serialIO_dataAvailable()) {
    rx_in_progress = true;
    char b;
    serialIO_readByte(&b);
    rx_buffer[rx_idx++] = (uint8_t)b;
    if (rx_idx >= PACKET_LEN_MAX) {
      rx_idx = 0;
      break;
    }
    last_rx = millis();
  }

  // TODO do away with timeout-based reception once we have framing inplace,
  // but it will work for Alpha build for now
  if (rx_in_progress && is_time_to_process_packet()) {
    serdes_decode_incomming_packet(rx_buffer, PACKET_LEN_MAX, rx_idx,
                                   gui_ack_handler, command_handler);
    rx_idx = 0;
    rx_in_progress = false;
  }
}

void comms_handler() {
  process_tx();
  process_rx();
}
