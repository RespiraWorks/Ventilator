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

#include "pid.h"
#include "vars.h"
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

static DebugFloat dbgSetpoint("setpoint", "Setpoint pressure, kPa");

Controller::Controller()
    : pid_(Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
           DifferentialTerm::ON_MEASUREMENT,
           // Increases in the blower fan speed should result in increased
           // pressure.
           ControlDirection::DIRECT,
           // Our output is an 8-bit PWM.
           /*output_min=*/0.f, /*output_max=*/255.f, PID_SAMPLE_PERIOD) {}

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
  // If the blower is not enabled, immediately shut down the fan.  But for
  // consistency, we still run the PID iteration above.
  float output;
  float setpoint = desired_state.setpoint_pressure.kPa();
  dbgSetpoint.Set(setpoint);

  if (desired_state.blower_enabled) {
    output = pid_.Compute(
        /*time=*/now,
        /*input=*/cmH2O(sensor_readings.patient_pressure_cm_h2o).kPa(),
        /*setpoint=*/setpoint);
  } else {
    output = 0;
    pid_.Observe(/*time=*/now,
                 /*input=*/cmH2O(sensor_readings.patient_pressure_cm_h2o).kPa(),
                 /*setpoint=*/setpoint,
                 /*output=*/output);
  }

  // fan_power is in range [0, 1].
  return output / 255.f;
}
