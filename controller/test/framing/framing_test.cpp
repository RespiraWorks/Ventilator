#include "framing.h"

#include "checksum.h"
#include "network_protocol.pb.h"
#include "uart_dma_stream.h"
#include "gtest/gtest.h"
#include <stdint.h>
#include <stdio.h>

TEST(FramingTests, DecodingDestTooSmall) {
  uint8_t source_buf[] = {FRAMING_MARK, 0, 1, 2, 3, FRAMING_MARK};
  uint8_t dest_buf[10];

  uint32_t frameLength =
      UnescapeFrame(source_buf, sizeof(source_buf), dest_buf, 3);
  ASSERT_EQ(frameLength, (uint32_t)0);
  frameLength = UnescapeFrame(source_buf, sizeof(source_buf), dest_buf, 4);
  ASSERT_GT(frameLength, (uint32_t)0);
}

static constexpr auto EncodeGuiStatusFrame = EncodeFrame<GuiStatus>;

static auto DecodeGuiStatusFrame = DecodeFrame<GuiStatus>;

static auto EncodeControllerStatusFrame = EncodeFrame<ControllerStatus>;

static constexpr auto DecodeControllerStatusFrame =
    DecodeFrame<ControllerStatus>;

UART_DMA uart_dma;
DmaStream<200> dma_stream;

uint8_t tx_buffer[ProtoTraits<ControllerStatus>::MaxFrameSize];
uint32_t tx_length = 0;
bool is_txing = false;
TxListener *tx_listener;

bool UART_DMA::isTxInProgress() { return is_txing; }
bool UART_DMA::startTX(const uint8_t *buf, uint32_t length, TxListener *txl) {
  memcpy(tx_buffer, buf, length);
  tx_length = length;
  is_txing = true;
  tx_listener = txl;
  printf("TX event, length: %d\n ", tx_length);
  for (uint32_t i = 0; i < length; i++) {
    printf("%X ", buf[i]);
  }
  printf("\n");
  return true;
};

void UART_DMA::DMA_TX_ISR() {
  is_txing = false;
  tx_listener->onTxComplete();
}

void UartDmaReset() { uart_dma.DMA_TX_ISR(); }

TEST(FramingTests, ControllerStatusCoding) {
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

  uint32_t encoded_length = EncodeControllerStatusFrame(s, dma_stream);
  printf("Encoded length: %d\n", encoded_length);
  ASSERT_GT(encoded_length, (uint32_t)0);
  EXPECT_EQ(tx_length, encoded_length);
  ControllerStatus decoded = ControllerStatus_init_zero;
  DecodeResult r =
      DecodeControllerStatusFrame(tx_buffer, encoded_length, &decoded);
  ASSERT_EQ(r, 0);
  EXPECT_EQ(s.uptime_ms, decoded.uptime_ms);
  EXPECT_EQ(s.active_params.mode, decoded.active_params.mode);
  EXPECT_EQ(s.active_params.peep_cm_h2o, decoded.active_params.peep_cm_h2o);
  EXPECT_EQ(s.sensor_readings.patient_pressure_cm_h2o,
            decoded.sensor_readings.patient_pressure_cm_h2o);
}

TEST(FramingTests, GuiStatusCoding) {
  UartDmaReset();
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

  uint32_t encoded_length = EncodeGuiStatusFrame(s, dma_stream);
  printf("Encoded length: %d\n", encoded_length);
  ASSERT_GT(encoded_length, (uint32_t)0);
  GuiStatus decoded = GuiStatus_init_zero;
  DecodeResult r = DecodeGuiStatusFrame(tx_buffer, encoded_length, &decoded);
  ASSERT_EQ(r, 0);

  EXPECT_EQ(s.uptime_ms, decoded.uptime_ms);
  EXPECT_EQ(s.desired_params.mode, decoded.desired_params.mode);
}
