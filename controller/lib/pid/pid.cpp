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

PID::PID(float *input, float *output, float *setpoint, float kp, float ki,
         float kd, bool p_on_e, bool d_on_e, int controller_direction)
    : sample_time_(milliseconds(100)), next_sample_time_(Hal.now()),
      last_update_time_(Hal.now() - sample_time_) {
  output_ = output;
  input_ = input;
  setpoint_ = setpoint;
  controller_direction_ = controller_direction;
  SetTunings(kp, ki, kd, p_on_e, d_on_e);
}

bool PID::Compute() {
  Time now = Hal.now();
  if (!initialized_) {
    last_input_ = *input_;
    last_error_ = *setpoint_ - *input_;
    output_sum_ = stl::clamp(*output_, out_min_, out_max_);
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
  /*Compute all the working error variables*/
  float input = *input_;
  float error = *setpoint_ - input;
  float dInput = 0.0;

  float kp, ki, kd;
  if (controller_direction_ == DIRECT) {
    kp = kp_;
    ki = ki_;
    kd = kd_;
  } else {
    kp = -kp_;
    ki = -ki_;
    kd = -kd_;
  }

  // Compute dInput only if needed (P_ON_M or D_ON_M)
  if (p_on_e_ == P_ON_M || d_on_e_ == D_ON_M) {
    dInput = (input - last_input_);
  }
  output_sum_ += (ki * error * samplesTimeChangeSec);

  /*Add Proportional on Measurement, if P_ON_M is specified*/
  if (p_on_e_ == P_ON_M)
    output_sum_ -= kp * dInput;

  output_sum_ = stl::clamp(output_sum_, out_min_, out_max_);

  /*Add Proportional on Error, if P_ON_E is specified*/
  float output;
  if (p_on_e_ == P_ON_E) {
    output = kp * error;
  } else {
    output = 0;
  }
  if (d_on_e_ == D_ON_M) {
    dInput /= samplesTimeChangeSec;
    /*Compute Rest of PID Output*/
    output += output_sum_ - kd * dInput;
  } else { // d_on_e_==D_ON_E
    float dError = (error - last_error_) / samplesTimeChangeSec;
    /*Compute Rest of PID Output*/
    output += output_sum_ + kd * dError;
  }

  *output_ = stl::clamp(output, out_min_, out_max_);

  /*Remember some variables for next time*/
  last_input_ = input;
  last_error_ = error;
  last_update_time_ = now;
  // when should we expect to perform our next output calculation
  next_sample_time_ = next_sample_time_ + sample_time_;
  return true;
}

void PID::SetTunings(float kp, float ki, float kd, bool p_on_e, bool d_on_e) {
  if (kp < 0 || ki < 0 || kd < 0)
    return;
  p_on_e_ = p_on_e;
  d_on_e_ = d_on_e;
  kp_ = kp;
  ki_ = ki;
  kd_ = kd;
}

void PID::SetSampleTime(Duration sample_time) {
  if (sample_time <= milliseconds(0))
    return;
  next_sample_time_ = next_sample_time_ - sample_time_ + sample_time;
  sample_time_ = sample_time;
}

void PID::SetOutputLimits(float min, float max) {
  if (min >= max)
    return;
  out_min_ = min;
  out_max_ = max;

  *output_ = stl::clamp(*output_, out_min_, out_max_);
  output_sum_ = stl::clamp(output_sum_, out_min_, out_max_);
}
