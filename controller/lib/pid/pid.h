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
 * Adapted from Arduino PID Library - Version 1.2.1 under MIT License
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 * Original project hash: 9b4ca0e5b6d7bab9c6ac023e249d6af2446d99bb
 **********************************************************************************************/

#ifndef PID_H
#define PID_H

#include "units.h"

class PID {
public:
  // Constructs the PID using the given parameters.
  PID();

  void SetKP(float kp) { kp_ = kp; }
  void SetKI(float ki) { ki_ = ki; }
  void SetKD(float kd) { kd_ = kd; }
  void SetKFF(float kff) { kff_ = kff; }

  void SetSamplePeriod(Duration sample_period) {
    if (sample_period.seconds() > 0)
      sample_period_ = sample_period;
  }

  void SetLimits(float min, float max) {
    if (max > min) {
      out_min_ = min;
      out_max_ = max;
    }
  }

  // Performs one step of the PID calculation.
  // If this call was ignored due to being within sample time
  // of the previous call, returns the last returned value.
  float Compute(float input, float setpoint);

  // Reset the internal state.
  void Reset(float input = 0);

  // private:
  float kp_{0};  // (P)roportional Tuning Parameter
  float ki_{0};  // (I)ntegral Tuning Parameter
  float kd_{0};  // (D)erivative Tuning Parameter
  float kff_{0}; // Feed forward gain

  float out_min_{1.0e-9f};
  float out_max_{1.0e9f};

  Duration sample_period_ = seconds(1);

  float isum_{0};
  float last_error_{0};
};
#endif
