#include "comms.h"

#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include "checksum.h"
#include "frame_detector.h"
#include "framing_streams.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "proto_traits.h"
#include "units.h"
#include "gtest/gtest.h"

uint8_t tx_buffer[ProtoTraits<ControllerStatus>::MaxFrameSize];
uint32_t tx_length = 0;
bool is_txing = false;

void UartDma::init(uint32_t baud){};
bool UartDma::tx_in_progress() const { return is_txing; }
bool UartDma::rx_in_progress() const { return false; }
bool UartDma::start_tx(uint8_t *buf, uint32_t length, TxListener *txl) {
  memcpy(tx_buffer, buf, length);
  tx_length = length;
  is_txing = true;
  return true;
};

void UartDma::stop_tx(){};
void UartDma::stop_rx(){};
void UartDma::enable_char_match(){};

UartDma uart_dma;

uint32_t UnescapeFrame(uint8_t *source, uint32_t sourceLength, uint8_t *dest,
                       uint32_t destLength);
TEST(CommTests, SendControllerStatus) {
  Comms comms;
  // Initialize a large ControllerStatus so as to force multiple calls to
  // CommsHandler to send it.
  ControllerStatus s = ControllerStatus_init_zero;
  s.uptime_ms = 42;
  s.active_params.mode = VentMode_PRESSURE_CONTROL;
  s.active_params.peep_cm_h2o = 10;
  s.active_params.breaths_per_min = 15;
  s.active_params.pip_cm_h2o = 1;
  s.active_params.inspiratory_expiratory_ratio = 2;
  s.active_params.inspiratory_trigger_cm_h2o = 5;
  s.active_params.expiratory_trigger_ml_per_min = 9;
  s.sensor_readings.volume_ml = 800;
  s.sensor_readings.flow_ml_per_min = 1000;

  // Run CommsHandler until it stops sending data.  10 iterations should be
  // more than enough.
  for (int i = 0; i < 10; i++) {
    GuiStatus gui_status_ignored = GuiStatus_init_zero;
    comms.handler(s, &gui_status_ignored);
  }
  uint8_t decoded_buf[ControllerStatus_size + 4];
  uint32_t decoded_length = UnescapeFrame(tx_buffer, tx_length, decoded_buf,
                                          ControllerStatus_size + 4);
  printf("Decoded len: %d\n", decoded_length);
  EXPECT_TRUE(crc_ok(decoded_buf, decoded_length));
  ASSERT_GT(decoded_length, static_cast<uint32_t>(0));

  pb_istream_t stream = pb_istream_from_buffer(
      reinterpret_cast<unsigned char *>(decoded_buf), decoded_length - 4);

  ControllerStatus sent = ControllerStatus_init_zero;
  ASSERT_TRUE(pb_decode(&stream, ControllerStatus_fields, &sent));
  EXPECT_EQ(s.uptime_ms, sent.uptime_ms);
  EXPECT_EQ(s.active_params.mode, sent.active_params.mode);
  EXPECT_EQ(s.active_params.peep_cm_h2o, sent.active_params.peep_cm_h2o);
  EXPECT_EQ(s.sensor_readings.patient_pressure_cm_h2o,
            sent.sensor_readings.patient_pressure_cm_h2o);
}

uint8_t fake_frame[(GuiStatus_size + 4) * 2 + 2];
RxListener *rx_listener;
uint32_t rx_index = 0;
uint32_t rx_length = 0;

uint32_t UartDma::rx_bytes_left() { return rx_length - rx_index; };

bool UartDma::start_rx(uint8_t *buf, uint32_t length, RxListener *rxl) {
  rx_length = length;
  rx_listener = rxl;
  rx_index = 0;
  return true;
};

void fake_rx(uint32_t encoded_length) {
  for (uint32_t i = 0; i < encoded_length; i++) {
    rx_buffer.test_put_byte(fake_frame[i]);
    if (FramingMark == fake_frame[i]) {
      rx_listener->on_character_match();
    }
  }
  rx_listener->on_rx_complete();
}

static bool shouldEscape(uint8_t b) {
  return FramingMark == b || FramingEscape == b;
}

uint32_t EscapeFrame(uint8_t *source, uint32_t sourceLength, uint8_t *dest,
                     uint32_t destLength) {
  uint32_t i = 0;
  dest[i++] = FramingMark;
  for (uint32_t j = 0; j < sourceLength; j++) {
    if (shouldEscape(source[j])) {
      dest[i++] = FramingEscape;
      dest[i++] = source[j] ^ 0x20;
    } else {
      dest[i++] = source[j];
    }
    if (i >= destLength) {
      return 0;
    }
  }
  dest[i++] = FramingMark;
  return i;
}

// Adds CRC of the dataLength of data bytes in the buf at the end of the buf
bool append_crc(uint8_t *buf, uint32_t dataLength, uint32_t bufLength,
                uint32_t crc32) {
  if (dataLength + 4 > bufLength) {
    return false;
  }
  buf[dataLength] = static_cast<uint8_t>((crc32 >> 24) & 0x000000FF);
  buf[dataLength + 1] = static_cast<uint8_t>((crc32 >> 16) & 0x000000FF);
  buf[dataLength + 2] = static_cast<uint8_t>((crc32 >> 8) & 0x000000FF);
  buf[dataLength + 3] = static_cast<uint8_t>(crc32 & 0x000000FF);
  return true;
}

TEST(CommTests, CommandRx) {
  Comms comms;
  GuiStatus s = GuiStatus_init_zero;
  s.uptime_ms = std::numeric_limits<uint32_t>::max() / 2;
  s.desired_params.mode = VentMode_PRESSURE_CONTROL;
  s.desired_params.peep_cm_h2o = 10;
  s.desired_params.breaths_per_min = 15;
  s.desired_params.pip_cm_h2o = 1;
  s.desired_params.inspiratory_expiratory_ratio = 2;
  s.desired_params.inspiratory_trigger_cm_h2o = 5;
  s.desired_params.expiratory_trigger_ml_per_min = 9;

  uint8_t pb_buf[GuiStatus_size + 4];
  pb_ostream_t stream = pb_ostream_from_buffer(pb_buf, GuiStatus_size);
  pb_encode(&stream, GuiStatus_fields, &s);

  uint32_t len = (uint32_t)(stream.bytes_written);
  EXPECT_GT(len, 0u);
  uint32_t crc32 = soft_crc32(pb_buf, reinterpret_cast<uint32_t>(len));
  bool crc_appened = append_crc(pb_buf, len, GuiStatus_size + 4, crc32);
  EXPECT_TRUE(crc_appened);
  uint32_t encoded_length =
      EscapeFrame(pb_buf, len + 4, fake_frame, sizeof(fake_frame));
  EXPECT_GT(encoded_length, (uint32_t)0);

  ControllerStatus controller_status_ignored = ControllerStatus_init_zero;
  GuiStatus received = GuiStatus_init_zero;
  EXPECT_TRUE(comms.init());
  fake_rx(encoded_length);
  comms.handler(controller_status_ignored, &received);
  EXPECT_EQ(s.uptime_ms, received.uptime_ms);
  EXPECT_EQ(s.desired_params.mode, received.desired_params.mode);
}
