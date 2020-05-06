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
#define LIBRARY_VERSION 1.2.1

#include "units.h"

enum class ProportionalTerm {
  ON_ERROR,
  ON_MEASUREMENT,
};

enum class DifferentialTerm {
  ON_ERROR,
  ON_MEASUREMENT,
};

enum class ControlDirection {
  DIRECT,
  REVERSE,
};

class PID {
public:
  // Constructs the PID linked to the Input, Output, and Setpoint,
  // using the given parameters.
  PID(float *input, float *output, float *setpoint, float kp, float ki,
      float kd, ProportionalTerm p_term, DifferentialTerm d_term,
      ControlDirection direction);

  // Performs one step of the PID calculation. Calculation frequency
  // can be set using SetSampleTime.
  // Returns false if this call was ignored due to being within sample time
  // of the previous call, otherwise true.
  bool Compute();

  // Clamps the output to a specific range. 0-255 by default, but
  // it's likely the user will want to change this depending on
  // the application.
  void SetOutputLimits(float min, float max);

  // Sets the frequency, in Milliseconds, with which the PID calculation
  // is performed.  Default is 100.
  void SetSampleTime(Duration sample_time);

private:
  float
      *const input_; // * Pointers to the Input, Output, and Setpoint variables
  float
      *const output_; // This creates a hard link between the variables and the
  float *const setpoint_; // PID, freeing the user from having to constantly
                          // tell us what these values are.

  const float kp_; // * (P)roportional Tuning Parameter
  const float ki_; // * (I)ntegral Tuning Parameter
  const float kd_; // * (D)erivative Tuning Parameter

  const ProportionalTerm p_term_;
  const DifferentialTerm d_term_;
  const ControlDirection direction_;

  // default output limit corresponds to the arduino pwm limits
  float out_min_ = 0;
  float out_max_ = 255;

  bool initialized_ = false;
  Duration sample_time_ = milliseconds(100);
  Time next_sample_time_ = millisSinceStartup(0);
  Time last_update_time_ = millisSinceStartup(0);
  float output_sum_;
  float last_input_;
  float last_error_;
};
#endif
