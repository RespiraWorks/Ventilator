/* Copyright 2020-2021, RespiraWorks

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

PID::PID(const char *name, const char *help_supplement, float initial_kp, float initial_ki,
         float initial_kd, TermApplication p_term, TermApplication d_term, float output_min,
         float output_max)
    : kp_("kp", Debug::Variable::Access::ReadWrite, initial_kp, "", "Proportional gain"),
      ki_("ki", Debug::Variable::Access::ReadWrite, initial_ki, "", "Integral gain"),
      kd_("kd", Debug::Variable::Access::ReadWrite, initial_kd, "", "Derivative gain"),
      proportional_term_(p_term),
      differential_term_(d_term),
      out_min_(output_min),
      out_max_(output_max) {
  kp_.prepend_name(name);
  kp_.append_help(help_supplement);
  ki_.prepend_name(name);
  ki_.append_help(help_supplement);
  kd_.prepend_name(name);
  kd_.append_help(help_supplement);
}

void PID::kp(float new_kp) { kp_.set(new_kp); }

void PID::ki(float new_ki) { ki_.set(new_ki); }

void PID::kd(float new_kd) { kd_.set(new_kd); }

float PID::kp() const { return kp_.get(); }

float PID::ki() const { return ki_.get(); }

float PID::kd() const { return kd_.get(); }

void PID::reset() { initialized_ = false; }

float PID::compute(Time now, float input, float set_point) {
  if (!initialized_) {
    last_input_ = input;
    last_error_ = set_point - input;
    last_update_time_ = now;
    output_sum_ = 0;
    initialized_ = true;
  }

  // Compute time between now and last sample.
  float delta_t = (now - last_update_time_).seconds();

  // Compute all the working error variables
  float error = set_point - input;
  float delta_input = input - last_input_;

  output_sum_ += (ki_.get() * error * delta_t);

  if (proportional_term_ == TermApplication::OnMeasurement) {
    output_sum_ -= kp_.get() * delta_input;
  }

  output_sum_ = std::clamp(output_sum_, out_min_, out_max_);

  float res = output_sum_;
  if (proportional_term_ == TermApplication::OnError) {
    res += kp_.get() * error;
  }
  // delta_t may be 0 (e.g. on the first call to compute()), in which case we
  // simply skip using the derivative term.
  if (delta_t > 0) {
    if (differential_term_ == TermApplication::OnMeasurement) {
      res -= kd_.get() * delta_input / delta_t;
    } else {
      res += kd_.get() * (error - last_error_) / delta_t;
    }
  }

  // Remember some variables for next time
  last_input_ = input;
  last_error_ = error;
  last_update_time_ = now;
  return std::clamp(res, out_min_, out_max_);
}

void PID::observe(Time now, float input, float set_point, float actual_output) {
  // All the observable variables are updated the same way as in compute();
  last_input_ = input;
  last_error_ = set_point - input;
  last_update_time_ = now;
  // Reset output_sum_ to actual_output so that the next compute()
  // will adjust it only slightly (as if it had been computed by a current
  // compute() call), avoiding a spike.
  output_sum_ = std::clamp(actual_output, out_min_, out_max_);
}
