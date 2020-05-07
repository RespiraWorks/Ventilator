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
static constexpr Duration Tu = seconds(1.5);

// "No overshoot" settings from the Ziegler-Nichols Wikipedia page.  This
// avoids overpressurizing the patient's lungs.
static constexpr float Kp = 0.2 * Ku;
static constexpr float Ki = 0.4 * Ku / Tu.seconds();
static constexpr float Kd = Ku * Tu.seconds() / 15;

// DIRECT means that increases in the output should result in increases in the
// input.  DIRECT as opposed to REVERSE.
static PID myPID(Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
                 DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT,
                 // Our output is an 8-bit PWM.
                 /*output_min=*/0.f, /*output_max=*/255.f, PID_SAMPLE_PERIOD);

// TODO: VOLUME_INTEGRAL_INTERVAL was not chosen carefully.
static constexpr Duration VOLUME_INTEGRAL_INTERVAL = milliseconds(5);
static Time last_flow_measurement_time = millisSinceStartup(0);
static float last_flow_ml_per_min = NAN;

void blower_pid_init() {
}

static void update_volume(SensorReadings *readings) {
  // TODO: This calculation should be much more sophisticated.  Some possible
  // improvements.
  //
  //  - Periodically re-zero the volume (e.g. what happens if the tubes are
  //    disconnected from the patient?)
  //
  //  - Measure time with better than millisecond granularity.
  Time now = Hal.now();
  if (isnan(last_flow_ml_per_min)) { // First time
    readings->volume_ml = 0;
    last_flow_ml_per_min = readings->flow_ml_per_min;
    last_flow_measurement_time = now;
  } else if (Duration delta = now - last_flow_measurement_time;
             delta >= VOLUME_INTEGRAL_INTERVAL) {
    readings->volume_ml += delta.minutes() *
                           (last_flow_ml_per_min + readings->flow_ml_per_min) /
                           2;
    last_flow_ml_per_min = readings->flow_ml_per_min;
    last_flow_measurement_time = now;
  }
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

  float pid_output =
      myPID.Compute(/*input=*/cur_pressure.kPa(),
                    /*setpoint=*/desired_state.setpoint_pressure.kPa());
  // If the blower is not enabled, immediately shut down the fan.  But for
  // consistency, we still run the PID iteration above.
  float output =
      static_cast<int>(desired_state.blower_enabled ? pid_output : 0);
  Hal.analogWrite(PwmPin::BLOWER, output);

  // fan_power is in range [0, 1].
  *fan_power = output / 255.f;

  // Store sensor readings so they can eventually be sent to the GUI.
  // This pressure is just from the patient sensor, converted to the right
  // units.
  readings->pressure_cm_h2o = cur_pressure.cmH2O();

  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  readings->flow_ml_per_min =
      (get_volumetric_inflow() - get_volumetric_outflow()).ml_per_min();

  update_volume(readings);
}
