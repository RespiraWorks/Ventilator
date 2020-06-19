/* Copyright 2020, Edwin Chiu

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

#include "actuators.h"
#include "comms.h"
#include "controller.h"
#include "debug.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "sensors.h"
#include "trace.h"
#include <limits>
#include <optional>

// By default, the controller receives settings (on/off, pip, rr, etc.) from
// the GUI.  But you can also command the controller by setting the gui_foo
// DebugVars below.
static DebugUInt32
    gui_mode("gui_mode",
             "Mode the controller should run in; see VentMode enum in "
             "network_protocol.proto.  If this is out of range (e.g. set to "
             "-1), this and all of the other gui_foo DebugVars are ignored, "
             "and instead the controller takes orders from the GUI itself.",
             std::numeric_limits<uint32_t>::max());
static DebugUInt32 gui_bpm("gui_bpm", "Breaths/min for GUI-free testing", 15);
static DebugUInt32 gui_peep("gui_peep", "PEEP (cm/h2O) for GUI-free testing",
                            5);
static DebugUInt32 gui_pip("gui_pip", "PIP (cm/h2O) for GUI-free testing", 15);
static DebugFloat gui_ie_ratio("gui_ie_ratio", "I/E ratio for GUI-free testing",
                               0.66f);
static DebugFloat
    gui_fio2("gui_fio2", "Percent oxygen (range [21,100]) for GUI-free testing",
             21);

static Controller controller;
static ControllerStatus controller_status;
static Sensors sensors;

static SensorsProto AsSensorsProto(const SensorReadings &r,
                                   const ControllerState &c) {
  SensorsProto proto = SensorsProto_init_zero;
  proto.patient_pressure_cm_h2o = r.patient_pressure.cmH2O();
  proto.inflow_pressure_diff_cm_h2o = r.inflow_pressure_diff.cmH2O();
  proto.outflow_pressure_diff_cm_h2o = r.outflow_pressure_diff.cmH2O();
  proto.flow_ml_per_min = c.net_flow.ml_per_min();
  proto.volume_ml = c.patient_volume.ml();
  proto.breath_id = c.breath_id;
  proto.flow_correction_ml_per_min = c.flow_correction.ml_per_min();
  return proto;
}

// This function handles all the high priority tasks which need to be called
// periodically.  The HAL calls this function from a timer interrupt.
//
// NOTE - it's important that anything being called from this function executes
// quickly.  No busy waiting here.
static void high_priority_task(void *arg) {

  // Read the sensors
  SensorReadings sensor_readings = sensors.GetReadings();

  // Run our PID loop
  auto [actuators_state, controller_state] = controller.Run(
      Hal.now(), controller_status.active_params, sensor_readings);

  // TODO update pb library to replace fan_power in ControllerStatus with
  // actuators_state, and remove pressure_setpoint_cm_h2o from ControllerStatus

  // Update the outputs from the PID
  actuators_execute(actuators_state);

  // Update controller_status.  This is periodically sent back to the GUI.
  controller_status.sensor_readings =
      AsSensorsProto(sensor_readings, controller_state);
  controller_status.fan_power = actuators_state.blower_power;
  controller_status.pressure_setpoint_cm_h2o =
      controller_state.pressure_setpoint.cmH2O();

  // Sample any trace variables that are enabled
  trace.MaybeSample();

  // Pet the watchdog
  Hal.watchdog_handler();
}

// This function is the lower priority background loop which runs continuously
// after some basic system init.  Pretty much everything not time critical
// should go here.
static void background_loop() {
  // Sleep for a few seconds.  In the current iteration of the PCB, the fan
  // briefly turns on when the device starts up.  If we don't wait for the fan
  // to spin down, the sensors will miscalibrate.  This is a hardware issue
  // that will be fixed in the PCB revision after 0.2.
  // https://respiraworks.slack.com/archives/C011CJQV4Q7/p1591745893290300?thread_ts=1591745582.289600&cid=C011CJQV4Q7
  //
  // Take this opportunity while we're sleeping to home the pinch valves.  This
  // way we're guaranteed that they're ready before we start ventilating.
  Time sleepStart = Hal.now();
  while (!AreActuatorsReady() || Hal.now() - sleepStart < seconds(10)) {
    actuators_execute({
        .fio2_valve = 0,
        .blower_power = 0,
        .blower_valve = 1,
        .exhale_valve = 1,
    });
    Hal.delay(milliseconds(10));
    Hal.watchdog_handler();
    debug.Poll();
  }

  // Calibrate the sensors.
  // This needs to be done before the sensors are used.
  sensors.Calibrate();

  // Current controller status.  Updated when we receive data from the GUI, when
  // sensors read data, etc.
  controller_status = ControllerStatus_init_zero;

  // Last-received status from the GUI.
  GuiStatus gui_status = GuiStatus_init_zero;

  // After all initialization is done, ask the HAL
  // to start our high priority thread.
  Hal.startLoopTimer(controller.GetLoopPeriod(), high_priority_task, nullptr);

  while (true) {
    controller_status.uptime_ms = Hal.now().microsSinceStartup() / 1000;

    // Copy the current controller status with interrupts
    // disabled to ensure that the data we send to the
    // GUI is self consistent.
    ControllerStatus local_controller_status;
    {
      BlockInterrupts block;
      local_controller_status = controller_status;
    }

    comms_handler(local_controller_status, &gui_status);

    // Override received gui_status from the RPi with values from DebugVars iff
    // the gui_mode DebugVar has a legal value.
    if (uint32_t m = gui_mode.Get(); m >= _VentMode_MIN && m <= _VentMode_MAX) {
      auto &p = gui_status.desired_params;
      p.mode = static_cast<VentMode>(m);
      p.breaths_per_min = gui_bpm.Get();
      p.peep_cm_h2o = gui_peep.Get();
      p.pip_cm_h2o = gui_pip.Get();
      p.inspiratory_expiratory_ratio = gui_ie_ratio.Get();
      p.fio2 = gui_fio2.Get() / 100.f;
    }

    // Copy the gui_status data into our controller status
    // with interrupts disabled.  This ensures that the data
    // is copied atomically
    {
      BlockInterrupts block;
      controller_status.active_params = gui_status.desired_params;
    }

    // Handle the debug serial interface
    debug.Poll();
  }
}

int main() {
  // Initialize Hal first because it initializes the watchdog. See comment on
  // HalApi::init().
  Hal.init();

  comms_init();

  background_loop();
}
