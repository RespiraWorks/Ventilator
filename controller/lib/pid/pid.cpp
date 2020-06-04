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

PID::PID() {}

void PID::Reset(float input) {
  isum_ = 0;
  last_error_ = 0;
}

float PID::Compute(float input, float setpoint) {

  // The proportional gain is multiplied by the error
  float error = setpoint - input;
  float output = kp_ * error;

  // Find the change in error.  This is used
  // for the derivative term
  float dError = error - last_error_;
  last_error_ = error;
  output += kd_ * dError / sample_period_.seconds();

  // Update the integral sum.
  float old_isum = isum_;
  isum_ += ki_ * error * sample_period_.seconds();

  output += isum_;

  // Feed forward is simply a gain multipled by the input
  output += input * kff_;

  // Clamp the output.  If the output is clamped and
  // the error is in the same direction as the output,
  // then limit the integral sum to prevent windup
  if (output > out_max_) {
    if (error > 0)
      isum_ = old_isum;
    output = out_max_;
  }

  else if (output < out_min_) {
    if (error < 0)
      isum_ = old_isum;
    output = out_min_;
  }

  return output;
}
