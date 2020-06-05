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

#include "framing.h"

#include <cstdint>
#include <cstdio>

#include "checksum.h"
#include "gtest/gtest.h"
#include "network_protocol.pb.h"
#include "uart_dma_stream.h"

TEST(FramingTests, DecodingDestTooSmall) {
  uint8_t source_buf[] = {FramingMark, 0, 1, 2, 3, FramingMark};
  uint8_t dest_buf[10];

  uint32_t frameLength = UnescapeFrame(source_buf, sizeof(source_buf), dest_buf, 3);
  ASSERT_EQ(frameLength, (uint32_t)0);
  frameLength = UnescapeFrame(source_buf, sizeof(source_buf), dest_buf, 4);
  ASSERT_GT(frameLength, (uint32_t)0);
}

static constexpr auto EncodeGuiStatusFrame = EncodeFrame<GuiStatus>;
static auto DecodeGuiStatusFrame = DecodeFrame<GuiStatus>;
static auto EncodeControllerStatusFrame = EncodeFrame<ControllerStatus>;
static constexpr auto DecodeControllerStatusFrame = DecodeFrame<ControllerStatus>;

UartDma uart_dma;
DmaStream<200> dma_stream;

uint8_t tx_buffer[ProtoTraits<ControllerStatus>::MaxFrameSize];
uint32_t tx_length{0};
bool is_transferring{false};
TxListener *tx_listener;

bool UartDma::tx_in_progress() const { return is_transferring; }
bool UartDma::start_tx(uint8_t *buf, uint32_t length, TxListener *txl) {
  memcpy(tx_buffer, buf, length);
  tx_length = length;
  is_transferring = true;
  tx_listener = txl;
  printf("TX event, length: %d\n ", tx_length);
  for (uint32_t i = 0; i < length; i++) {
    printf("%X ", buf[i]);
  }
  printf("\n");
  return true;
};

void UartDma::DMA_tx_interrupt_handler() {
  is_transferring = false;
  tx_listener->on_tx_complete();
}

void UartDmaReset() { uart_dma.DMA_tx_interrupt_handler(); }

// \todo reintroduce additional settings once they are in buffer definitions

TEST(FramingTests, ControllerStatusCoding) {
  ControllerStatus s = ControllerStatus_init_zero;
  s.uptime_ms = 42;
  s.active_params.mode = VentMode_PRESSURE_CONTROL;
  s.active_params.peep_cm_h2o = 10;
  s.active_params.breaths_per_min = 15;
  s.active_params.pip_cm_h2o = 1;
  s.active_params.inspiratory_expiratory_ratio = 2;
  s.active_params.inspiratory_trigger_cm_h2o = 5;
  s.active_params.expiratory_trigger_ml_per_min = 9;
  s.sensor_readings.patient_pressure_cm_h2o = 11;
  s.sensor_readings.volume_ml = 800;
  s.sensor_readings.flow_ml_per_min = 1000;

  uint32_t encoded_length = EncodeControllerStatusFrame(s, dma_stream);
  printf("Encoded length: %d\n", encoded_length);
  ASSERT_GT(encoded_length, (uint32_t)0);
  EXPECT_EQ(tx_length, encoded_length);
  ControllerStatus decoded = ControllerStatus_init_zero;
  DecodeResult r = DecodeControllerStatusFrame(tx_buffer, encoded_length, &decoded);
  ASSERT_EQ(r, DecodeResult());
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
  s.desired_params.inspiratory_trigger_cm_h2o = 5;
  s.desired_params.expiratory_trigger_ml_per_min = 9;

  uint32_t encoded_length = EncodeGuiStatusFrame(s, dma_stream);
  printf("Encoded length: %d\n", encoded_length);
  ASSERT_GT(encoded_length, (uint32_t)0);
  GuiStatus decoded = GuiStatus_init_zero;
  DecodeResult r = DecodeGuiStatusFrame(tx_buffer, encoded_length, &decoded);
  ASSERT_EQ(r, DecodeResult());

  EXPECT_EQ(s.uptime_ms, decoded.uptime_ms);
  EXPECT_EQ(s.desired_params.mode, decoded.desired_params.mode);
}
