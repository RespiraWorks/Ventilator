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

// NO_GUI_DEV_MODE is a hacky development mode until we have the GUI working.
//
// Uncomment this line to get started:
//
#define NO_GUI_DEV_MODE
//
// Then see comment on DEV_MODE_comms_handler below.
//
// TODO: Remove NO_GUI_DEV_MODE once the GUI is working.
#ifdef NO_GUI_DEV_MODE
// Fake "development mode" version of sending and receiving data over the
// network.
//
// "Receives" data from the "GUI" just by setting gui_status directly.  Change
// these params and reflash to simulate the GUI changing its settings.
//
// "Sends" data to the "GUI" via a simple serial protocol.  This can be parsed
// and graphed by e.g. the Arduino IDE (tools -> serial plotter).
static DebugUInt32
    gui_mode("gui_mode", "Mode setting for GUI free testing",
             static_cast<uint32_t>(VentMode::VentMode_PRESSURE_CONTROL));
static DebugUInt32 gui_bpm("gui_bpm", "Breaths/min for GUI free testing", 15);
static DebugUInt32 gui_peep("gui_peep", "PEEP (cm/h2O) for GUI free testing",
                            5);
static DebugUInt32 gui_pip("gui_pip", "PIP (cm/h2O) for GUI free testing", 15);
static DebugFloat gui_ie_ratio("gui_ie_ratio", "I/E ratio for GUI free testing",
                               0.66f);
static void DEV_MODE_comms_handler(const ControllerStatus &controller_status,
                                   GuiStatus *gui_status) {
  gui_status->desired_params.mode = static_cast<VentMode>(gui_mode.Get());
  gui_status->desired_params.breaths_per_min = gui_bpm.Get();
  gui_status->desired_params.peep_cm_h2o = gui_peep.Get();
  gui_status->desired_params.pip_cm_h2o = gui_pip.Get();
  gui_status->desired_params.inspiratory_expiratory_ratio = gui_ie_ratio.Get();

  static Time last_sent = millisSinceStartup(0);
  Time now = Hal.now();
  if (now - last_sent < seconds(0.1f)) {
    return;
  }
  last_sent = now;

  auto &r = controller_status.sensor_readings;
  debug.Print("%.2f, ", controller_status.fan_setpoint_cm_h2o);
  debug.Print("%.2f, ", r.patient_pressure_cm_h2o);
  debug.Print("%.2f, ", r.inflow_pressure_diff_cm_h2o);
  debug.Print("%.2f, ", r.outflow_pressure_diff_cm_h2o);
  debug.Print("%.2f, ", r.flow_ml_per_min / 1000.0f);
  // debugPrint("%.2f, ", r.volume_ml / 10.f);
  debug.Print("\n");
}
#endif

static ControllerStatus controller_status;
static Sensors sensors;

static VentModeNone vmode_none;
static VentModePresCtrl vmode_pressure_ctrl;

Duration Controller::GetLoopPeriod() { return milliseconds(1); }

// This function handles all the high priority tasks which need to be called
// periodically.  The HAL calls this function from a timer interrupt.
//
// NOTE - it's important that anything being called from this function executes
// quickly.  No busy waiting here.
static void high_priority_task(void *arg) {
  static VentMode prevMode = VentMode::VentMode_OFF;

  // Read the sensors
  controller_status.sensor_readings = sensors.GetSensorReadings();

  Controller *controller = nullptr;

  switch (controller_status.active_params.mode) {
  case VentMode::VentMode_OFF:
    controller = &vmode_none;
    break;

  case VentMode::VentMode_PRESSURE_CONTROL:
    controller = &vmode_pressure_ctrl;
    break;
  }

  // If we switched modes, give the controller for the new
  // mode a chance to initialize itself.
  if (controller_status.active_params.mode != prevMode) {
    prevMode = controller_status.active_params.mode;
    controller->Enter();
  }

  // Run the controller state machine
  ActuatorsState actuators_state = controller->Run(
      controller_status.active_params, controller_status.sensor_readings);

  // Update the outputs from the PID
  actuators_execute(actuators_state);

  // Update some status info
  controller_status.fan_power = actuators_state.fan_power;

  // Sample any trace variables that are enabled
  trace.MaybeSample();

  // Pet the watchdog
  Hal.watchdog_handler();
}

// This function is the lower priority background loop which runs continuously
// after some basic system init.  Pretty much everything not time critical
// should go here.
static void background_loop() {

  // My initial mode is the disabled state.
  // Normally modes are given a chance to initialize themselves on a mode
  // switch, but since there won't be a switch into this mode we call its
  // initializer at startup.
  vmode_none.Enter();

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
  Hal.startLoopTimer(Controller::GetLoopPeriod(), high_priority_task, 0);

  while (true) {
    controller_status.uptime_ms = Hal.now().millisSinceStartup();

    // Copy the current controller status with interrupts
    // disabled to ensure that the data we send to the
    // GUI is self consistent.
    ControllerStatus local_controller_status;
    {
      BlockInterrupts block;
      local_controller_status = controller_status;
    }

#ifndef NO_GUI_DEV_MODE
    comms_handler(local_controller_status, &gui_status);
#else
    DEV_MODE_comms_handler(local_controller_status, &gui_status);
#endif

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
