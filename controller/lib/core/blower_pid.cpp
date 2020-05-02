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

#include "blower_pid.h"

#include "comms.h"
#include "hal.h"
#include "sensors.h"
#include "types.h"

static double Setpoint;
static double Input;
static double Output;

// PID-tuning were chosen by following the Ziegler-Nichols method,
// https://en.wikipedia.org/wiki/Ziegler%E2%80%93Nichols_method.
//
// Here's an achieved pressure vs. desired pressure trace collected on Alpha
// build, 2020-05-01, with the following settings.
//
//   PEEP = 10 cm H2O,
//   PIP = 30 cm H2O,
//   RR = 10 breaths/min
//   I:E = 2
//
// https://drive.google.com/file/d/13QyvO1ptUYYUAs8qT0dCyW9f1XZ8w7iw
//
// Note that Ku and Tu only seem to work with this particular sample time.
static constexpr Duration SampleTime = milliseconds(10);
static constexpr float Ku = 600;
static constexpr Duration Tu = seconds(2);

// "Classic PID" settings.  Wikipedia says to expect aggressive overshoot with
// these params, but I don't observe any overshoot.
static constexpr float Kp = 0.6 * Ku;
static constexpr float Ki = 1.2 * Ku / Tu.seconds();
static constexpr float Kd = 3 * Ku * Tu.seconds() / 40;

// DIRECT means that increases in the output should result in increases in the
// input.  DIRECT as opposed to REVERSE.
static PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void blower_pid_init() {
  Setpoint = 0;
  Input = 0;
  Output = 0;
  myPID.SetSampleTime(SampleTime.milliseconds());

  // Our output is an 8-bit PWM.
  myPID.SetOutputLimits(0, 255);

  // Turn the PID on.
  myPID.SetMode(AUTOMATIC);
}

void blower_pid_execute(const BlowerSystemState &desired_state,
                        SensorReadings *readings, float *fan_power) {
  // Open/close the solenoid as appropriate.
  //
  // Our solenoid is "normally open", so low voltage means open and high
  // voltage means closed.  Hardware spec: https://bit.ly/3aERr69
  Hal.digitalWrite(BinaryPin::SOLENOID,
                   desired_state.expire_valve_state == ValveState::OPEN
                       ? VoltageLevel::HAL_LOW
                       : VoltageLevel::HAL_HIGH);

  Pressure cur_pressure = get_patient_pressure();

  Setpoint = desired_state.setpoint_pressure.kPa();
  Input = cur_pressure.kPa();
  myPID.Compute();

  // If the blower is not enabled, immediately shut down the fan.  But for
  // consistency, we still run the PID iteration above.
  if (!desired_state.blower_enabled) {
    Output = 0;
  }
  Hal.analogWrite(PwmPin::BLOWER, static_cast<int>(Output));

  // fan_power is in range [0, 1].
  *fan_power = stl::min(stl::max(Output, 0.), 255.) / 255.;

  // Store sensor readings so they can eventually be sent to the GUI.
  // This pressure is just from the patient sensor, converted to the right
  // units.
  readings->pressure_cm_h2o = cur_pressure.cmH2O();
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
