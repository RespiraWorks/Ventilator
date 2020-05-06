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
  PROPORTIONAL_ON_ERROR,
  PROPORTIONAL_ON_MEASUREMENT,
};

enum class DifferentialTerm {
  DIFFERENTIAL_ON_ERROR,
  DIFFERENTIAL_ON_MEASUREMENT,
};

enum class ControlDirection {
  DIRECT,
  REVERSE,
};

class PID {
public:
// Constants used in some of the functions below
#define DIRECT 0
#define REVERSE 1
#define P_ON_M 0
#define P_ON_E 1
#define D_ON_M 0
#define D_ON_E 1

  // Constructs the PID linked to the Input, Output, and Setpoint,
  // using the given parameters.
  PID(float *Input, float *Output, float *Setpoint, float Kp, float Ki,
      float Kd, bool POnE, bool DOnE, int ControllerDirection);

  // Performs one step of the PID calculation. Calculation frequency
  // can be set using SetSampleTime.
  bool Compute();

  // Clamps the output to a specific range. 0-255 by default, but
  // it's likely the user will want to change this depending on
  // the application.
  void SetOutputLimits(float Min, float Max);

  // Changes the PID parameters to the given values.
  void SetTunings(float Kp, float Ki, float Kd, bool POnE, bool DOnE);

  // Sets the frequency, in Milliseconds, with which the PID calculation
  // is performed.  Default is 100.
  void SetSampleTime(Duration NewSampleTime);

private:
  float kp; // * (P)roportional Tuning Parameter
  float ki; // * (I)ntegral Tuning Parameter
  float kd; // * (D)erivative Tuning Parameter

  int controllerDirection;

  float *myInput;    // * Pointers to the Input, Output, and Setpoint variables
  float *myOutput;   // This creates a hard link between the variables and the
  float *mySetpoint; // PID, freeing the user from having to constantly tell
                     // us what these values are.

  Duration SampleTime;
  Time nextSampleTime, lastUpdateTime;
  float outputSum, lastInput, lastError;

  float outMin, outMax;
  bool pOnE, dOnE;
};
#endif
