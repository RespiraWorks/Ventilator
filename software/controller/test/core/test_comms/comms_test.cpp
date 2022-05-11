#include "comms.h"

#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include "gtest_main.h"
#include "uart_mock.h"
#include "network_protocol.pb.h"
#include "system_timer.h"

TEST(CommTests, SendControllerStatus) {
  UART::MockChannel uart;
  Comms comms{&uart};

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

  // Run comms::Handler until it stops sending data.  10 iterations should be
  // more than enough.
  for (int i = 0; i < 10; i++) {
    GuiStatus gui_status_ignored = GuiStatus_init_zero;
    comms.Handler(s, &gui_status_ignored);
  }
  uint8_t tx_buffer[ControllerStatus_size];
  uint16_t len = uart.GetTxData(tx_buffer, sizeof(tx_buffer));
  ASSERT_GT(len, 0);
  pb_istream_t stream = pb_istream_from_buffer(reinterpret_cast<unsigned char *>(tx_buffer), len);

  ControllerStatus sent = ControllerStatus_init_zero;
  ASSERT_TRUE(pb_decode(&stream, ControllerStatus_fields, &sent));
  EXPECT_EQ(s.uptime_ms, sent.uptime_ms);
  EXPECT_EQ(s.active_params.mode, sent.active_params.mode);
  EXPECT_EQ(s.active_params.peep_cm_h2o, sent.active_params.peep_cm_h2o);
  EXPECT_EQ(s.sensor_readings.patient_pressure_cm_h2o,
            sent.sensor_readings.patient_pressure_cm_h2o);
}

TEST(CommTests, CommandRx) {
  UART::MockChannel uart;
  Comms comms{&uart};

  GuiStatus s = GuiStatus_init_zero;
  s.uptime_ms = std::numeric_limits<uint32_t>::max() / 2;
  s.desired_params.mode = VentMode_PRESSURE_CONTROL;
  s.desired_params.peep_cm_h2o = 10;
  s.desired_params.breaths_per_min = 15;
  s.desired_params.pip_cm_h2o = 1;
  s.desired_params.inspiratory_expiratory_ratio = 2;
  s.desired_params.inspiratory_trigger_cm_h2o = 5;
  s.desired_params.expiratory_trigger_ml_per_min = 9;

  uint8_t rx_buffer[GuiStatus_size];
  pb_ostream_t stream = pb_ostream_from_buffer(rx_buffer, sizeof(rx_buffer));
  pb_encode(&stream, GuiStatus_fields, &s);
  EXPECT_GT(stream.bytes_written, 0u);
  uart.PutRxData(rx_buffer, static_cast<uint16_t>(stream.bytes_written));
  EXPECT_GT(uart.RxFull(), 0);

  ControllerStatus controller_status_ignored = ControllerStatus_init_zero;
  GuiStatus received = GuiStatus_init_zero;

  // Run CommsHandler until it updates GuiStatus.  10 iterations should be
  // more than enough to read the whole thing.
  for (int i = 0; i < 10; i++) {
    comms.Handler(controller_status_ignored, &received);
    // We use a timeout for framing packets, so we have to advance the time,
    // otherwise we'll never think the packet is complete!
    SystemTimer::singleton().delay(milliseconds(1));
  }
  EXPECT_EQ(s.uptime_ms, received.uptime_ms);
  EXPECT_EQ(s.desired_params.mode, received.desired_params.mode);
}
