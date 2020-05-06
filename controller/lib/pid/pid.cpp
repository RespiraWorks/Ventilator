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
#include "hal.h"

PID::PID(float kp, float ki, float kd, ProportionalTerm p_term,
         DifferentialTerm d_term, ControlDirection direction)
    : kp_(kp), ki_(ki), kd_(kd), p_term_(p_term), d_term_(d_term),
      direction_(direction) {}

bool PID::Compute(float input, float setpoint, float *output) {
  Time now = Hal.now();
  if (!initialized_) {
    last_input_ = input;
    last_error_ = setpoint - input;
    output_sum_ = stl::clamp(*output, out_min_, out_max_);
    next_sample_time_ = now;
    // last call time defined as now - SampleTime to enable computation on first
    // call (user should call Compute() immediately after SetMode(Auto))
    last_update_time_ = now - sample_time_;
    initialized_ = true;
  }

  // compute actual samples time-difference to take jitter into account in
  // integral and derivative
  Duration effectiveSampleTime = (now - last_update_time_);
  float samplesTimeChangeSec = effectiveSampleTime.seconds();
  // condition to update output : 1 sample time has passed and we have new data
  if (now < next_sample_time_ || samplesTimeChangeSec <= 0) {
    return false;
  }
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

  if (p_term_ == ProportionalTerm::ON_MEASUREMENT)
    output_sum_ -= kp * dInput;

  output_sum_ = stl::clamp(output_sum_, out_min_, out_max_);

  float res = output_sum_;
  if (p_term_ == ProportionalTerm::ON_ERROR) {
    res += kp * error;
  }
  if (d_term_ == DifferentialTerm::ON_MEASUREMENT) {
    res -= kd * dInput / samplesTimeChangeSec;
  } else {
    res += kd * (error - last_error_) / samplesTimeChangeSec;
  }

  *output = stl::clamp(res, out_min_, out_max_);

  // Remember some variables for next time
  last_input_ = input;
  last_error_ = error;
  last_update_time_ = now;
  // when should we expect to perform our next output calculation
  next_sample_time_ = next_sample_time_ + sample_time_;
  return true;
}

void PID::SetSampleTime(Duration sample_time) {
  next_sample_time_ = next_sample_time_ - sample_time_ + sample_time;
  sample_time_ = sample_time;
}

void PID::SetOutputLimits(float min, float max) {
  out_min_ = min;
  out_max_ = max;
  output_sum_ = stl::clamp(output_sum_, out_min_, out_max_);
}
