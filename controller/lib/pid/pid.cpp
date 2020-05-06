
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
#include "hal.h"

PID::PID(float *Input, float *Output, float *Setpoint, float Kp, float Ki,
         float Kd, bool POnE, bool DOnE, int ControllerDirection)
    : sample_time_(milliseconds(100)), next_sample_time_(Hal.now()),
      last_update_time_(Hal.now() - sample_time_) {
  output_ = Output;
  input_ = Input;
  setpoint_ = Setpoint;

  // default output limit corresponds to the arduino pwm limits
  PID::SetOutputLimits(0, 255);

  controller_direction_ = ControllerDirection;
  PID::SetTunings(Kp, Ki, Kd, POnE, DOnE);
}

bool PID::Compute() {
  Time now = Hal.now();
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
  // Compute dInput only if needed (P_ON_M or D_ON_M)
  if (p_on_e_ == P_ON_M || d_on_e_ == D_ON_M) {
    dInput = (input - last_input_);
  }
  output_sum_ += (ki_ * error * samplesTimeChangeSec);

  /*Add Proportional on Measurement, if P_ON_M is specified*/
  if (p_on_e_ == P_ON_M)
    output_sum_ -= kp_ * dInput;

  output_sum_ = std::clamp(output_sum_, out_min_, out_max_);

  /*Add Proportional on Error, if P_ON_E is specified*/
  float output;
  if (p_on_e_ == P_ON_E) {
    output = kp_ * error;
  } else {
    output = 0;
  }
  if (d_on_e_ == D_ON_M) {
    dInput /= samplesTimeChangeSec;
    /*Compute Rest of PID Output*/
    output += output_sum_ - kd_ * dInput;
  } else { // d_on_e_==D_ON_E
    float dError = (error - last_error_) / samplesTimeChangeSec;
    /*Compute Rest of PID Output*/
    output += output_sum_ + kd_ * dError;
  }

  *output_ = std::clamp(output, out_min_, out_max_);

  /*Remember some variables for next time*/
  last_input_ = input;
  last_error_ = error;
  last_update_time_ = now;
  // when should we expect to perform our next output calculation
  next_sample_time_ = next_sample_time_ + sample_time_;
  return true;
}

void PID::SetTunings(float Kp, float Ki, float Kd, bool POnE, bool DOnE) {
  if (Kp < 0 || Ki < 0 || Kd < 0)
    return;

  p_on_e_ = POnE;
  d_on_e_ = DOnE;

  kp_ = Kp;
  ki_ = Ki;
  kd_ = Kd;

  if (controller_direction_ == REVERSE) {
    kp_ = (0 - kp_);
    ki_ = (0 - ki_);
    kd_ = (0 - kd_);
  }
}

void PID::SetSampleTime(Duration NewSampleTime) {
  if (NewSampleTime <= milliseconds(0))
    return;
  next_sample_time_ = next_sample_time_ - sample_time_ + NewSampleTime;
  sample_time_ = NewSampleTime;
}

void PID::SetOutputLimits(float Min, float Max) {
  if (Min >= Max)
    return;
  out_min_ = Min;
  out_max_ = Max;

  *output_ = std::clamp(*output_, out_min_, out_max_);
  output_sum_ = std::clamp(output_sum_, out_min_, out_max_);
}
