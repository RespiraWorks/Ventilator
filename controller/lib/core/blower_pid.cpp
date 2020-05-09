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

#include <math.h>

#include "comms.h"
#include "hal.h"
#include "sensors.h"
#include "types.h"

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

// DIRECT means that increases in the output should result in increases in the
// input.  DIRECT as opposed to REVERSE.
static PID myPID(Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
                 DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT,
                 // Our output is an 8-bit PWM.
                 /*output_min=*/0.f, /*output_max=*/255.f, PID_SAMPLE_PERIOD);

void blower_pid_init() {
}

float blower_pid_execute(Time now, const BlowerSystemState &desired_state,
                         float current_pressure_cm_h2o) {

  // If the blower is not enabled, immediately shut down the fan.  But for
  // consistency, we still run the PID iteration above.
  float output;
  if (desired_state.blower_enabled) {
    output = myPID.Compute(now, /*input=*/cmH2O(current_pressure_cm_h2o).kPa(),
                           /*setpoint=*/desired_state.setpoint_pressure.kPa());
  } else {
    output = 0;
    myPID.Observe(now, /*input=*/cmH2O(current_pressure_cm_h2o).kPa(),
                  /*setpoint=*/desired_state.setpoint_pressure.kPa(),
                  /*actual_output=*/output);
  }

  // fan_power is in range [0, 1].
  return output / 255.f;
}
