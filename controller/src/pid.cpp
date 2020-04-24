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

#include "pid.h"
#include "comms.h"
#include "hal.h"
#include "sensors.h"
#include "types.h"

// Define Variables we'll be connecting to
static double Setpoint, Input, Output;

// Configure the PID
// Specify the links and initial tuning parameters
static float Kp = 2, Ki = 8, Kd = 0;
static PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// These constants won't change. They're used to give names to the pins used:
// Analog output pin that the LED is attached to
static const int analogOutPin = LED_BUILTIN;

enum class pid_fsm_state {
  reset = 0,
  inspire = 1,
  plateau = 2,
  expire = 3,
  expire_dwell = 4,

  count /* Sentinel */
};

void pid_init() {

  //@TODO: Raw access that needs to be updated to reference the new modules
  // because currently does not have the benefits of things like calibration,
  // etc. that the module methods do. I didn't do this because I have no clue
  // what the desired units are for the control system.
  // Initialize PID
  Input = map(Hal.analogRead(DPSENSOR_PIN), 0, 1023, 0, 255);
  Setpoint = BLOWER_LOW;

  // turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void pid_execute(const VentParams &params, SensorReadings *readings) {
  uint16_t cyclecounter = 0;
  enum pid_fsm_state state = pid_fsm_state::reset;

  switch (state) {
  case pid_fsm_state::reset: // reset
    cyclecounter = 0;
    Setpoint = PEEP;
    state = pid_fsm_state::inspire; // update state
    break;

  case pid_fsm_state::inspire: // Inspire
    cyclecounter++;
    // set command
    Setpoint += INSPIRE_RATE;
    if (Setpoint > PIP) {
      Setpoint = PIP;
    }
    // update state
    if (cyclecounter > INSPIRE_TIME) {
      cyclecounter = 0;
      state = pid_fsm_state::plateau;
    }
    break;

  case pid_fsm_state::plateau: // Inspiratory plateau
    cyclecounter++;
    // set command
    Setpoint = INSPIRE_DWELL_PRESSURE;
    // update state
    if (cyclecounter > INSPIRE_DWELL) {
      cyclecounter = 0;
      state = pid_fsm_state::expire;
    }
    break;

  case pid_fsm_state::expire: // Expire
    cyclecounter++;
    // set command
    Setpoint -= EXPIRE_RATE;
    if (Setpoint < PEEP) {
      Setpoint = PEEP;
    }
    // update state
    if (cyclecounter > EXPIRE_TIME) {
      cyclecounter = 0;
      state = pid_fsm_state::expire_dwell;
    }
    break;

  case pid_fsm_state::expire_dwell: // Expiratory Dwell
    cyclecounter++;
    // set command
    Setpoint = PEEP;
    // update state
    if (cyclecounter > EXPIRE_DWELL) {
      cyclecounter = 0;
      state = pid_fsm_state::reset;
    }
    break;

  default:
    state = pid_fsm_state::reset;
    break;
  }

  //@TODO: The following are raw accesses that need to be updated to reference
  // the new modules because currently they do not have the benefits of things
  // like calibration, etc. that the module methods do. I didn't do this because
  // I have no clue what the desired units are for the control system.
  // Update PID Loop
  int16_t sensorValue = Hal.analogRead(DPSENSOR_PIN); // read sensor
  // int16_t sensorValue = get_pressure_reading(PressureSensors::SomeDPPin);
  Input = map(sensorValue, 0, 1023, 0, 255); // map to output scale
  myPID.Compute();                           // computer PID command
  Hal.analogWrite(BLOWERSPD_PIN, static_cast<int>(Output)); // write output

  // Store sensor readings so they can eventually be sent to the GUI.
  // TODO(jlebar): Get and store volume and flow data.
  // TODO(jlebar/verityRF): determine what volume_ml and flow_ml_per_min really
  // means and how that is calculated from the two different venturi tubes in
  // the air loop. Most likely need to have an integrator for flow to get the
  // volume_ml.
  // TODO(someone): is pressure_cm_h2o just the patient pressure sensor
  // converted to the right unit? Convert [kPa] to [cm H2O] in place
  readings->pressure_cm_h2o =
      get_pressure_reading(PressureSensors::PATIENT_PIN) * 10.1974f;
  readings->volume_ml = 0;
  // example needs to be updated based on which of 2 DP sensors actually are
  // used in this metric
  // Convert [meters^3/s] to [mL/min] in place
  readings->flow_ml_per_min =
      convert_diff_pressure_to_volumetric_flow(
          get_pressure_reading(PressureSensors::EXHALATION_PIN)) *
      60000000.0f;
}
