/* Copyright 2020, RespiraWorks

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

#include "controller.h"

#include <math.h>

// PID-tuning were chosen by following the Ziegler-Nichols method,
// https://en.wikipedia.org/wiki/Ziegler%E2%80%93Nichols_method
//
// Note that Ku and Tu only seem to work with this particular sample time.
static constexpr Duration PID_SAMPLE_PERIOD = milliseconds(10);
static constexpr float Ku = 200;
static constexpr Duration Tu = seconds(1.5f);

// "No overshoot" settings from the Ziegler-Nichols Wikipedia page.  This
// avoids overpressurizing the patient's lungs.
static constexpr float Kp = 0.2f * Ku;
static constexpr float Ki = 0.4f * Ku / Tu.seconds();
static constexpr float Kd = Ku * Tu.seconds() / 15;

Controller::Controller() {
  prevError = 0;
  integral = 0;
}

Duration Controller::GetLoopPeriod() { return PID_SAMPLE_PERIOD; }

ActuatorsState Controller::Run(Time now, const VentParams &params,
                               const SensorReadings &readings) {
  BlowerSystemState desired_state = fsm_.DesiredState(now, params);

  return {.fan_setpoint_cm_h2o = desired_state.setpoint_pressure.cmH2O(),
          .expire_valve_state = desired_state.expire_valve_state,
          .fan_power = ComputeFanPower(now, desired_state, readings)};
}

float Controller::ComputeFanPower(Time now,
                                  const BlowerSystemState &desired_state,
                                  const SensorReadings &sensor_readings) {

  // Calculate the error in our system.
  // The error is the difference between the desired pressure and
  // the actual pressure.

  float error = desired_state.setpoint_pressure.kPa() -
                cmH2O(sensor_readings.patient_pressure_cm_h2o).kPa();

  // Find the change in error since the last loop cycle
  float deltaErr = error - prevError;
  prevError = error;

  // If the blower is disabled, just zero my integrator
  // and return 0 as the blower command.
  if (!desired_state.blower_enabled) {
    integral = 0;
    return 0;
  }

  // Keep track of the previous integral value.
  // If I end up saturating then I won't increase
  // the integral sum.  This avoids integral windup
  // and is more flexible then just clipping the
  // integral sum to an arbitrary value
  float oldIntegral = integral;

  // Increase the integral sum by Ki * error.
  // Ki is currently scaled by the loop update rate
  integral += error * Ki * PID_SAMPLE_PERIOD.seconds();

  // The D term is the change in error times a constant
  // The constant is currently scaled by the loop frequency
  float dterm = deltaErr * Kd / PID_SAMPLE_PERIOD.seconds();

  // Find the output based on all three gains
  float output = Kp * error + dterm + integral;

  // Clip my output to the range 0 to 1
  if (output < 0) {
    output = 0;

    // Prevent the integral from getting any smaller
    // if we're clipping on the lower limit
    if (integral < oldIntegral)
      integral = oldIntegral;
  }

  else if (output > 1) {
    output = 1;

    // Prevent the integral from increasing
    if (integral > oldIntegral)
      integral = oldIntegral;
  }

  return output;
}
