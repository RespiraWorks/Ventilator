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
#include "observer_stream.h"

TEST(FramingTests, DecodingDestTooSmall) {
  uint8_t source_buf[] = {FramingMark, 0, 1, 2, 3, FramingMark};
  uint8_t dest_buf[10];

  size_t frameLength = UnescapeFrame(source_buf, sizeof(source_buf), dest_buf, 3);
  ASSERT_EQ(frameLength, 0);
  frameLength = UnescapeFrame(source_buf, sizeof(source_buf), dest_buf, 4);
  ASSERT_GT(frameLength, 0);
}

static constexpr auto EncodeGuiStatusFrame = EncodeFrame<GuiStatus>;
static auto DecodeGuiStatusFrame = DecodeFrame<GuiStatus>;
static auto EncodeControllerStatusFrame = EncodeFrame<ControllerStatus>;
static constexpr auto DecodeControllerStatusFrame = DecodeFrame<ControllerStatus>;

TEST(FramingTests, ControllerStatusCoding) {
  ObserverStream stream;

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

  size_t encoded_length = EncodeControllerStatusFrame(s, stream);
  printf("Encoded length: %u\n", static_cast<int>(encoded_length));
  ASSERT_GT(encoded_length, 0);
  EXPECT_EQ(stream.get_index(), encoded_length);
  ControllerStatus decoded = ControllerStatus_init_zero;
  DecodeResult r = DecodeControllerStatusFrame(stream.get_saved(), encoded_length, &decoded);
  ASSERT_EQ(r, DecodeResult());
  EXPECT_EQ(s.uptime_ms, decoded.uptime_ms);
  EXPECT_EQ(s.active_params.mode, decoded.active_params.mode);
  EXPECT_EQ(s.active_params.peep_cm_h2o, decoded.active_params.peep_cm_h2o);
  EXPECT_EQ(s.sensor_readings.patient_pressure_cm_h2o,
            decoded.sensor_readings.patient_pressure_cm_h2o);
}

TEST(FramingTests, GuiStatusCoding) {
  ObserverStream stream;

  GuiStatus s = GuiStatus_init_zero;
  s.uptime_ms = std::numeric_limits<uint32_t>::max() / 2;
  s.desired_params.mode = VentMode_PRESSURE_CONTROL;
  s.desired_params.peep_cm_h2o = 10;
  s.desired_params.breaths_per_min = 15;
  s.desired_params.pip_cm_h2o = 1;
  s.desired_params.inspiratory_expiratory_ratio = 2;
  s.desired_params.inspiratory_trigger_cm_h2o = 5;
  s.desired_params.expiratory_trigger_ml_per_min = 9;

  size_t encoded_length = EncodeGuiStatusFrame(s, stream);
  printf("Encoded length: %u\n", static_cast<int>(encoded_length));
  ASSERT_GT(encoded_length, (uint32_t)0);
  GuiStatus decoded = GuiStatus_init_zero;
  DecodeResult r = DecodeGuiStatusFrame(stream.get_saved(), encoded_length, &decoded);
  ASSERT_EQ(r, DecodeResult());

  EXPECT_EQ(s.uptime_ms, decoded.uptime_ms);
  EXPECT_EQ(s.desired_params.mode, decoded.desired_params.mode);
}
