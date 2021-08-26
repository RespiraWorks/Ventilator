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

#include <algorithm>

PID::PID(const char* name, const char* help_supplement, float kp, float ki, float kd,
         ProportionalTerm p_term, DifferentialTerm d_term, float output_min, float output_max)
    : kp_(kp),
      ki_(ki),
      kd_(kd),
      dbg_kp_("kp", VarAccess::ReadWrite, kp, "", "Proportional gain"),
      dbg_ki_("ki", VarAccess::ReadWrite, ki, "", "Integral gain"),
      dbg_kd_("kd", VarAccess::ReadWrite, kd, "", "Derivative gain"),
      p_term_(p_term),
      d_term_(d_term),
      out_min_(output_min),
      out_max_(output_max) {
  dbg_kp_.prepend_name(name);
  dbg_kp_.append_help(help_supplement);
  dbg_ki_.prepend_name(name);
  dbg_ki_.append_help(help_supplement);
  dbg_kd_.prepend_name(name);
  dbg_kd_.append_help(help_supplement);
}

void PID::update_vars() {
  SetKP(dbg_kp_.Get());
  SetKI(dbg_ki_.Get());
  SetKD(dbg_kd_.Get());
}

float PID::Compute(Time now, float input, float setpoint) {
  if (!initialized_) {
    last_input_ = input;
    last_error_ = setpoint - input;
    last_update_time_ = now;
    output_sum_ = 0;
    initialized_ = true;
  }

  // Compute time between now and last sample.
  float delta_t = (now - last_update_time_).seconds();

  // Compute all the working error variables
  float error = setpoint - input;
  float delta_input = input - last_input_;

  output_sum_ += (ki_ * error * delta_t);

  if (p_term_ == ProportionalTerm::OnMeasurement) {
    output_sum_ -= kp_ * delta_input;
  }

  output_sum_ = std::clamp(output_sum_, out_min_, out_max_);

  float res = output_sum_;
  if (p_term_ == ProportionalTerm::OnError) {
    res += kp_ * error;
  }
  // delta_t may be 0 (e.g. on the first call to Compute()), in which case we
  // simply skip using the derivative term.
  if (delta_t > 0) {
    if (d_term_ == DifferentialTerm::OnMeasurement) {
      res -= kd_ * delta_input / delta_t;
    } else {
      res += kd_ * (error - last_error_) / delta_t;
    }
  }

  // Remember some variables for next time
  last_input_ = input;
  last_error_ = error;
  last_update_time_ = now;
  return std::clamp(res, out_min_, out_max_);
}

void PID::Observe(Time now, float input, float setpoint, float actual_output) {
  // All the observable variables are updated the same way as in Compute();
  last_input_ = input;
  last_error_ = setpoint - input;
  last_update_time_ = now;
  // Reset output_sum_ to actual_output so that the next Compute()
  // will adjust it only slightly (as if it had been computed by a current
  // Compute() call), avoiding a spike.
  output_sum_ = std::clamp(actual_output, out_min_, out_max_);
}
