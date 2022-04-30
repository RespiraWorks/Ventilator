/* Copyright 2022, RespiraWorks
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

#include "comms.h"

#include "framing.h"
#include "gtest_main.h"
#include "network_protocol.pb.h"
#include "observer_stream.h"
#include "system_timer.h"
#include "uart_mock.h"

TEST(CommTests, SendControllerStatus) {
  UART::MockChannel uart;
  Comms comms{&uart};

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

  // Run comms::Handler to send data
  GuiStatus gui_status_ignored = GuiStatus_init_zero;
  comms.Handler(s, &gui_status_ignored);

  // Retrieve data that was copied to UART
  uint8_t tx_buffer[(ControllerStatus_size + 4) * 2 + 2];
  size_t length = uart.GetTxData(tx_buffer, sizeof(tx_buffer));
  ASSERT_GT(length, 0);

  // Decode and compare
  ControllerStatus sent = ControllerStatus_init_zero;
  EXPECT_EQ(DecodeFrame<ControllerStatus>(tx_buffer, length, &sent), DecodeResult::Success);
  EXPECT_EQ(s.uptime_ms, sent.uptime_ms);
  EXPECT_EQ(s.active_params.mode, sent.active_params.mode);
  EXPECT_EQ(s.active_params.peep_cm_h2o, sent.active_params.peep_cm_h2o);
  EXPECT_EQ(s.sensor_readings.patient_pressure_cm_h2o,
            sent.sensor_readings.patient_pressure_cm_h2o);
}

TEST(CommTests, CommandRx) {
  UART::MockChannel uart{FramingMark};
  Comms comms{&uart};
  comms.Initialize();

  GuiStatus s = GuiStatus_init_zero;
  s.uptime_ms = std::numeric_limits<uint32_t>::max() / 2;
  s.desired_params.mode = VentMode_PRESSURE_CONTROL;
  s.desired_params.peep_cm_h2o = 10;
  s.desired_params.breaths_per_min = 15;
  s.desired_params.pip_cm_h2o = 1;
  s.desired_params.inspiratory_expiratory_ratio = 2;
  s.desired_params.inspiratory_trigger_cm_h2o = 5;
  s.desired_params.expiratory_trigger_ml_per_min = 9;

  ObserverStream stream;
  size_t bytes_written = EncodeFrame<GuiStatus>(s, stream);
  EXPECT_GT(bytes_written, 0);
  uart.PutRxData(stream.get_saved(), bytes_written);

  ControllerStatus controller_status_ignored = ControllerStatus_init_zero;
  GuiStatus received = GuiStatus_init_zero;

  comms.Handler(controller_status_ignored, &received);

  EXPECT_EQ(s.uptime_ms, received.uptime_ms);
  EXPECT_EQ(s.desired_params.mode, received.desired_params.mode);
}
