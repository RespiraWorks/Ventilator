#include "comms.h"

#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include "checksum.h"
#include "framing.h"
#include "framing_rx_fsm.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "gtest/gtest.h"

uint8_t tx_buffer[(ControllerStatus_size + 4) * 2 + 2];
uint32_t tx_length = 0;
bool is_txing = false;
uint8_t rx_buffer[(GuiStatus_size + 4) * 2 + 2];
UART_DMA_RxListener *rx_listener;
uint32_t rx_i = 0;
uint32_t rx_length = 0;

void UART_DMA::init(int baud){};
bool UART_DMA::isTxInProgress() { return false; }
bool UART_DMA::isRxInProgress() { return false; }
bool UART_DMA::startTX(const uint8_t *buf, uint32_t length,
                       UART_DMA_TxListener *txl) {
  memcpy(tx_buffer, buf, length);
  tx_length = length;
  is_txing = true;
  return true;
};

void UART_DMA::stopTX(){};

uint32_t UART_DMA::getRxBytesLeft() { return rx_length - rx_i - 1; };

bool UART_DMA::startRX(const uint8_t *buf, const uint32_t length,
                       const uint32_t timeout, UART_DMA_RxListener *rxl) {
  rx_length = length;
  rx_listener = rxl;
  return true;
};

void fakeRx() {
  for (rx_i = 0; rx_i < rx_length; rx_i++) {
    if (MARK == rx_buffer[rx_i]) {
      rx_listener->onCharacterMatch();
    }
  }
}

void HalTransport::test_PutRxBuffer(uint8_t *buf, uint32_t len) {
  memcpy(rx_buf, buf, len);
}

void Comms::test_PutRxBuffer(uint8_t *buf, uint32_t len) {
  rxFSM.test_PutRxBuffer(buf, len);
}

void UART_DMA::stopRX(){};
void UART_DMA::charMatchEnable(){};

UART_DMA uart_dma = UART_DMA();
HalTransport hal_transport(uart_dma);
FramingRxFSM<HalTransport> rxFSM(hal_transport);
Comms comms(uart_dma, rxFSM);

TEST(CommTests, SendControllerStatus) {
  // Initialize a large ControllerStatus so as to force multiple calls to
  // comms_handler to send it.
  ControllerStatus s = ControllerStatus_init_zero;
  s.uptime_ms = 42;
  s.active_params.mode = VentMode_PRESSURE_CONTROL;
  s.active_params.peep_cm_h2o = 10;
  s.active_params.breaths_per_min = 15;
  s.active_params.pip_cm_h2o = 1;
  s.active_params.inspiratory_expiratory_ratio = 2;
  s.active_params.rise_time_ms = 100;
  s.active_params.inspiratory_trigger_cm_h2o = 5;
  s.active_params.expiratory_trigger_ml_per_min = 9;
  // Set very large values here because they take up more space in the encoded
  // proto, and our goal is to make it big.
  s.active_params.alarm_lo_tidal_volume_ml =
      std::numeric_limits<uint32_t>::max();
  s.active_params.alarm_hi_tidal_volume_ml =
      std::numeric_limits<uint32_t>::max();
  s.active_params.alarm_lo_breaths_per_min =
      std::numeric_limits<uint32_t>::max();
  s.active_params.alarm_hi_breaths_per_min =
      std::numeric_limits<uint32_t>::max();
  s.sensor_readings.patient_pressure_cm_h2o = 11;
  s.sensor_readings.volume_ml = 800;
  s.sensor_readings.flow_ml_per_min = 1000;

  // Run comms_handler until it stops sending data.  10 iterations should be
  // more than enough.
  for (int i = 0; i < 10; i++) {
    GuiStatus gui_status_ignored = GuiStatus_init_zero;
    comms.handler(s, &gui_status_ignored);
  }
  uint8_t decoded_buf[ControllerStatus_size + 4];
  uint32_t decoded_length =
      decodeFrame(tx_buffer, tx_length, decoded_buf, ControllerStatus_size + 4);

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

TEST(CommTests, CommandRx) {
  GuiStatus s = GuiStatus_init_zero;
  s.uptime_ms = std::numeric_limits<uint32_t>::max() / 2;
  s.desired_params.mode = VentMode_PRESSURE_CONTROL;
  s.desired_params.peep_cm_h2o = 10;
  s.desired_params.breaths_per_min = 15;
  s.desired_params.pip_cm_h2o = 1;
  s.desired_params.inspiratory_expiratory_ratio = 2;
  s.desired_params.rise_time_ms = 100;
  s.desired_params.inspiratory_trigger_cm_h2o = 5;
  s.desired_params.expiratory_trigger_ml_per_min = 9;
  // Set very large values here because they take up more space in the encoded
  // proto, and our goal is to make it big.
  s.desired_params.alarm_lo_tidal_volume_ml =
      std::numeric_limits<uint32_t>::max();
  s.desired_params.alarm_hi_tidal_volume_ml =
      std::numeric_limits<uint32_t>::max();
  s.desired_params.alarm_lo_breaths_per_min =
      std::numeric_limits<uint32_t>::max();
  s.desired_params.alarm_hi_breaths_per_min =
      std::numeric_limits<uint32_t>::max();

  uint8_t pb_buf[GuiStatus_size + 4];
  pb_ostream_t stream = pb_ostream_from_buffer(pb_buf, GuiStatus_size);
  pb_encode(&stream, GuiStatus_fields, &s);
  uint32_t len = (uint32_t)(stream.bytes_written);
  EXPECT_GT(len, 0u);
  uint32_t crc32 = soft_crc32(pb_buf, reinterpret_cast<uint32_t>(len));
  bool crc_appened = append_crc(pb_buf, len, GuiStatus_size + 4, crc32);
  EXPECT_TRUE(crc_appened);
  uint32_t encoded_length =
      encodeFrame(pb_buf, len + 4, rx_buffer, sizeof(rx_buffer));
  EXPECT_GT(encoded_length, (uint32_t)0);

  ControllerStatus controller_status_ignored = ControllerStatus_init_zero;
  GuiStatus received = GuiStatus_init_zero;
  comms.init();
  comms.test_PutRxBuffer(rx_buffer, sizeof(rx_buffer));
  fakeRx();
  comms.handler(controller_status_ignored, &received);

  // Run comms_handler until it updates GuiStatus.  10 iterations should be
  // more than enough to read the whole thing.
  //   for (int i = 0; i < 10; i++) {
  //     comms.handler(controller_status_ignored, &received);
  //     // We use a timeout for framing packets, so we have to advance the
  //     time,
  //     // otherwise we'll never think the packet is complete!
  //     Hal.delay(milliseconds(1));
  //   }
  EXPECT_EQ(s.uptime_ms, received.uptime_ms);
  EXPECT_EQ(s.desired_params.mode, received.desired_params.mode);
}
