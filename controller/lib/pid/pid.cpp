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

/**********************************************************************************************
 * Adapted from Arduino PID Library - Version 1.2.1 - under MIT License
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 * Original project hash: 9b4ca0e5b6d7bab9c6ac023e249d6af2446d99bb
 **********************************************************************************************/

#include "pid.h"
#include "algorithm.h"

float PID::Compute(Time now, float input, float setpoint) {
  if (!initialized_) {
    last_input_ = input;
    last_error_ = setpoint - input;

    // last call time defined as now - SampleTime to enable computation on first
    // call (user should call Compute() immediately after SetMode(Auto))
    initialized_ = true;
  }

  float samplesTimeChangeSec = sample_period_.seconds();

  // Compute all the working error variables
  float error = setpoint - input;
  float dInput = 0.0;

  float kp, ki, kd;
  if (direction_ == ControlDirection::DIRECT) {
    kp = kp_;
    ki = ki_;
    kd = kd_;
  } else {
    kp = -kp_;
    ki = -ki_;
    kd = -kd_;
  }

  // Compute dInput only if needed.
  if (p_term_ == ProportionalTerm::ON_MEASUREMENT ||
      d_term_ == DifferentialTerm::ON_MEASUREMENT) {
    dInput = (input - last_input_);
  }
  output_sum_ += (ki * error * samplesTimeChangeSec);

  if (p_term_ == ProportionalTerm::ON_MEASUREMENT) {
    output_sum_ -= kp * dInput;
  }

  output_sum_ = std::clamp(output_sum_, out_min_, out_max_);

  float res = output_sum_;
  if (p_term_ == ProportionalTerm::ON_ERROR) {
    res += kp * error;
  }
  if (d_term_ == DifferentialTerm::ON_MEASUREMENT) {
    res -= kd * dInput / samplesTimeChangeSec;
  } else {
    res += kd * (error - last_error_) / samplesTimeChangeSec;
  }

  // Remember some variables for next time
  last_input_ = input;
  last_error_ = error;

  last_output_ = std::clamp(res, out_min_, out_max_);
  return last_output_;
}

void PID::Observe(Time now, float input, float setpoint, float actual_output) {
  // All the observable variables are updated the same way as in Compute();
  last_input_ = input;
  last_error_ = setpoint - input;
  // Reset output_sum_ to actual_output so that the next Compute()
  // will adjust it only slightly (as if it had been computed by a current
  // Compute() call), avoiding a spike.
  output_sum_ = std::clamp(actual_output, out_min_, out_max_);
}
