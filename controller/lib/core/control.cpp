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
// Specify the links and initial tuning parameters
static float Kp = 2, Ki = 8, Kd = 0;

// DIRECT means that increases in the output should result in increases in the
// input.  DIRECT as opposed to REVERSE.
static PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

enum class pid_fsm_state {
  reset = 0,
  inspire = 1,
  plateau = 2,
  expire = 3,
  expire_dwell = 4,
};

// TODO: move this into a mathematical library
// This function is originally from the Arduino library
long map(long x, long in_min, long in_max, long out_min, long out_max) {
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
  static enum pid_fsm_state state = pid_fsm_state::reset;

  // The state machine drives PID to blow a trapezoid.  We start at the PEEP
  // pressure, then gradually ramp the setpoint up to PIP.  Then we dwell there
  // for a while, ramp down to PEEP, and dwell there for a bit.
  //
  // TODO(jlebar): Per Edwin, we should simplify this by removing the ramp-up
  // and ramp-down states.  Instead, just modify the setpoint and let PID ramp
  // us up/down.  The timescales for the ramp-up and ramp-down are short, and
  // PID only moves us so fast anyway.
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

  // Read pressure sensor and update PID input.
  //
  // The Arduino analog-to-digital converter yields a 10-bit value.  As of
  // 2020-04-25, we're using exclusively MPXV5004 pressure sensors:
  // https://docs.google.com/spreadsheets/d/1EOa5USxCaV1uuK5RdZMPWIBl7iGKPtVBh_DIlNusR50/edit#gid=0
  //
  // Edwin says IRL that these pressure sensors should never return negative
  // values.

  int sensorValue = Hal.analogRead(AnalogPin::PATIENT_PRESSURE); // read sensor
  // int16_t sensorValue = get_pressure_reading(PressureSensors::SomeDPPin);
  Input = map(sensorValue, 0, 1023, 0, 255); // map to output scale
  myPID.Compute();                           // computer PID command
  Hal.analogWrite(PwmPin::BLOWER, static_cast<int>(Output)); // write output

  // Store sensor readings so they can eventually be sent to the GUI.
  // This pressure is just from the patient sensor, converted to the right
  // units.
  // Convert [kPa] to [cm H2O] in place
  readings->pressure_cm_h2o = get_patient_pressure_kpa() * 10.1974f;
  // TODO(anyone): This value is to be the integration over time of the below
  // flow_ml_per_min. That is, you take the value calculated for flow_ml_per_min
  // and integrate it over time to get total volume at the current time. Don't
  // think this necessarily has to be done on the controller though?
  readings->volume_ml = 0;
  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  // Convert [meters^3/s] to [mL/min] in place
  readings->flow_ml_per_min =
      (get_volumetric_inflow_m3ps() - get_volumetric_outflow_m3ps()) * 1000.0f *
      1000.0f * 60.0f;
}
