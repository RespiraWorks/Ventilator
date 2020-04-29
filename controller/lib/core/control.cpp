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

#include "control.h"
#include "comms.h"
#include "hal.h"
#include "sensors.h"
#include "types.h"

// Define Variables we'll be connecting to
static double Setpoint;
static double Input;
static double Output;

// Configure the PID
// TODO: Tune these params.
static float Kp = 2, Ki = 8, Kd = 0;

// DIRECT means that increases in the output should result in increases in the
// input.  DIRECT as opposed to REVERSE.
static PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

enum class pid_fsm_state {
  INSPIRE,
  EXPIRE,
};

// Rescales x from range [in_min, in_max] to [out_min, out_max] using integer
// math.  (This is a replacement for Arduino map(), because we don't want to
// include Arduino.h outside of hal.)
static long rescale(long x, long in_min, long in_max, long out_min,
                    long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void pid_init() {
  Setpoint = BLOWER_LOW;
  Input = 0;
  Output = 0;

  // Turn the PID on.
  myPID.SetMode(AUTOMATIC);
}

void pid_execute(const VentParams &params, SensorReadings *readings) {
  static uint16_t cyclecounter = 0;
  static enum pid_fsm_state state = pid_fsm_state::EXPIRE;

  // The state machine drives PID to blow a square wave.  The PID setpoint
  // moves from PEEP pressure (EXPIRE state) to PIP pressure (INSPIRE state)
  // and back to PEEP.
  switch (state) {
  case pid_fsm_state::INSPIRE:
    cyclecounter++;
    Setpoint = INSPIRE_DWELL_PRESSURE;
    if (cyclecounter > INSPIRE_DWELL) {
      cyclecounter = 0;
      state = pid_fsm_state::EXPIRE;
    }
    break;

  case pid_fsm_state::EXPIRE:
    cyclecounter++;
    Setpoint = PEEP;
    if (cyclecounter > EXPIRE_DWELL) {
      cyclecounter = 0;
      state = pid_fsm_state::INSPIRE;
    }
    break;
  }

  // Read pressure sensor and update PID input.
  // TODO: Read pressure in kPa from sensors module instead of reading the raw
  // pin value.
  int sensorValue = Hal.analogRead(AnalogPin::PATIENT_PRESSURE);
  Input = rescale(sensorValue, 0, 1023, 0, 255); // map to output scale
  myPID.Compute();                           // computer PID command
  Hal.analogWrite(PwmPin::BLOWER, static_cast<int>(Output)); // write output

  // Store sensor readings so they can eventually be sent to the GUI.
  // This pressure is just from the patient sensor, converted to the right
  // units.
  readings->pressure_cm_h2o = get_patient_pressure().cmH2O();
  // TODO(anyone): This value is to be the integration over time of the below
  // flow_ml_per_min. That is, you take the value calculated for flow_ml_per_min
  // and integrate it over time to get total volume at the current time. Don't
  // think this necessarily has to be done on the controller though?
  readings->volume_ml = 0;
  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  readings->flow_ml_per_min =
      (get_volumetric_inflow() - get_volumetric_outflow()).ml_per_min();
}
