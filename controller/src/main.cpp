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

/*
  Basic test and demo software for COVID-19/ARDS Ventilator

  Objective is to make a minimum-viable ARDS ventilator that can be deployed
  or constructed on-site in countries underserved by commecial global supply
  chains during the COVID-19 outbreak.

  Currently consists of a (1) CPAP-style blower with speed control.

  (2)Feedback from a differential pressure sensor with one side
  measuring delivered pressure to patient, other side ambient.


  created 16 Mar 2020
  Edwin Chiu
  Frost Methane Labs/Fix More Lungs
  Based on example code by Tom Igoe and Brett Beauregard

  Project Description: http://bit.ly/2wYqj3X
  git: https://github.com/RespiraWorks/VentilatorSoftware
  http://respira.works

  Outputs can be plotted with Cypress PSoC Programmer (Bridge Control Panel
  Tool) Download and install, connect serial Tools > Protocol Configuration >
  serial 115200:8n1 > hit OK In editor, use command RX8 [h=43] @1Key1 @0Key1
  @1Key2 @0Key2 Chart > Variable Settings Tick both Key1 and Key2, configure as
  int, and choose colors > hit OK Press >|< icon to connect to com port if
  necessary Click Repeat button, go to Chart tab both traces should now be
  plotting

*/

#include "alarm.h"
#include "blower_fsm.h"
#include "blower_pid.h"
#include "comms.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "sensors.h"
#include "solenoid.h"

// Current controller status.  Updated when we receive data from the GUI, when
// sensors read data, etc.
static ControllerStatus controller_status = ControllerStatus_init_zero;

// Last-received status from the GUI.
static GuiStatus gui_status = GuiStatus_init_zero;

// NO_GUI_DEV_MODE is a hacky development mode until we have the GUI working.
//
// Uncomment this line to get started:
//
//   #define NO_GUI_DEV_MODE
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
static void DEV_MODE_comms_handler() {
  gui_status.desired_params.mode = VentMode_PRESSURE_CONTROL;
  gui_status.desired_params.breaths_per_min = 12;
  gui_status.desired_params.peep_cm_h2o = 5;
  gui_status.desired_params.pip_cm_h2o = 15;
  gui_status.desired_params.inspiratory_expiratory_ratio = 0.66;

  static Time last_sent = Hal.now();
  Time now = Hal.now();
  if (now - last_sent < seconds(0.1)) {
    return;
  }
  last_sent = now;
  Serial.print(controller_status.fan_setpoint_cm_h2o);
  Serial.print(',');
  Serial.print(controller_status.sensor_readings.pressure_cm_h2o);
  Serial.print(',');
  Serial.print(controller_status.sensor_readings.flow_ml_per_min / 1000.0f);
  Serial.print(',');
  // Report volume in cL because because that fits on the graph better.
  Serial.print(controller_status.sensor_readings.volume_ml / 10.f);
  Serial.println();
}
#endif

static void controller_loop() {
  while (true) {
    controller_status.uptime_ms = Hal.millis();

#ifndef NO_GUI_DEV_MODE
    comms_handler(controller_status, &gui_status);
#else
    DEV_MODE_comms_handler();
#endif

    controller_status.active_params = gui_status.desired_params;

    // TODO: Add additional fields to the ControllerStatus proto:
    //   - Blower speed (0-255 PWM value)
    //   - Expiratory solenoid valve state (open/closed).
    BlowerSystemState desired_state =
        blower_fsm_desired_state(controller_status.active_params);
    controller_status.fan_setpoint_cm_h2o =
        desired_state.setpoint_pressure.cmH2O();

    blower_pid_execute(desired_state, &controller_status.sensor_readings,
                       &controller_status.fan_power);
    Hal.watchdog_handler();
  }
}

void setup() {
  // Initialize Hal first because it initializes the watchdog. See comment on
  // HalApi::init().
  Hal.init();

  comms_init();
  alarm_init();
  sensors_init();
  solenoid_init();
  blower_fsm_init();
  blower_pid_init();

  controller_loop();
}

void loop() {
  // Dummy placeholder as Arduino framework requires it to compile
}
