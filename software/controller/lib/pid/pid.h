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
 * Adapted from Arduino PID Library - Version 1.2.1 under MIT License
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 * Original project hash: 9b4ca0e5b6d7bab9c6ac023e249d6af2446d99bb
 **********************************************************************************************/

#pragma once

#include "units.h"
#include "vars.h"

class PID {
 public:
  enum class TermApplication {
    OnError,
    OnMeasurement,
  };

  // Constructs the PID using the given parameters.
  PID(const char *name, const char *help_supplement, float initial_kp, float initial_ki,
      float initial_kd, TermApplication p_term, TermApplication d_term, float output_min,
      float output_max);

  // Performs one step of the PID calculation.
  float compute(Time now, float input, float set_point);

  // Call this instead of Compute in case on this step of the control loop
  // you intend apply different control logic instead of the PID.
  // "actual_output" contains the value of output you plan to act on.
  //
  // This is a variation on the "manual" mode:
  // http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-onoff/
  // http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-initialization/
  void observe(Time now, float input, float set_point, float actual_output);

  // Setters
  void kp(float);
  void ki(float);
  void kd(float);

  // Getters
  float kp() const;
  float ki() const;
  float kd() const;

  void reset();

 private:
  Debug::Variable::Float kp_;  // * (P)roportional Tuning Parameter
  Debug::Variable::Float ki_;  // * (I)ntegral Tuning Parameter
  Debug::Variable::Float kd_;  // * (D)erivative Tuning Parameter

  const TermApplication proportional_term_;
  const TermApplication differential_term_;

  const float out_min_;
  const float out_max_;

  bool initialized_{false};
  Time last_update_time_{microsSinceStartup(0)};
  float output_sum_{0};
  float last_input_{0};
  float last_error_{0};
};
