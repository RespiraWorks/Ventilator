
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
    : SampleTime(milliseconds(100)), nextSampleTime(Hal.now()),
      lastUpdateTime(Hal.now() - SampleTime) {
  myOutput = Output;
  myInput = Input;
  mySetpoint = Setpoint;

  // default output limit corresponds to the arduino pwm limits
  PID::SetOutputLimits(0, 255);

  controllerDirection = ControllerDirection;
  PID::SetTunings(Kp, Ki, Kd, POnE, DOnE);
}

/* Compute()
 *********************************************************************** This,
 *as they say, is where the magic happens.  this function should be called every
 *time "void loop()" executes.  the function will decide for itself whether a
 *new pid Output needs to be computed.  returns true when the output is
 *computed, false when nothing has been done.
 **********************************************************************************/
bool PID::Compute() {
  Time now = Hal.now();
  // compute actual samples time-difference to take jitter into account in
  // integral and derivative
  Duration effectiveSampleTime = (now - lastUpdateTime);
  float samplesTimeChangeSec = effectiveSampleTime.seconds();
  // condition to update output : 1 sample time has passed and we have new data
  if (now < nextSampleTime || samplesTimeChangeSec <= 0) {
    return false;
  }
  /*Compute all the working error variables*/
  float input = *myInput;
  float error = *mySetpoint - input;
  float dInput = 0.0;
  // Compute dInput only if needed (P_ON_M or D_ON_M)
  if (pOnE == P_ON_M || dOnE == D_ON_M) {
    dInput = (input - lastInput);
  }
  outputSum += (ki * error * samplesTimeChangeSec);

  /*Add Proportional on Measurement, if P_ON_M is specified*/
  if (pOnE == P_ON_M)
    outputSum -= kp * dInput;

  if (outputSum > outMax)
    outputSum = outMax;
  else if (outputSum < outMin)
    outputSum = outMin;

  /*Add Proportional on Error, if P_ON_E is specified*/
  float output;
  if (pOnE == P_ON_E) {
    output = kp * error;
  } else {
    output = 0;
  }
  if (dOnE == D_ON_M) {
    dInput /= samplesTimeChangeSec;
    /*Compute Rest of PID Output*/
    output += outputSum - kd * dInput;
  } else { // dOnE==D_ON_E
    float dError = (error - lastError) / samplesTimeChangeSec;
    /*Compute Rest of PID Output*/
    output += outputSum + kd * dError;
  }

  *myOutput = std::clamp(output, outMin, outMax);

  /*Remember some variables for next time*/
  lastInput = input;
  lastError = error;
  lastUpdateTime = now;
  // when should we expect to perform our next output calculation
  nextSampleTime = nextSampleTime + SampleTime;
  return true;
}

/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted.
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/
void PID::SetTunings(float Kp, float Ki, float Kd, bool POnE, bool DOnE) {
  if (Kp < 0 || Ki < 0 || Kd < 0)
    return;

  pOnE = POnE;
  dOnE = DOnE;

  kp = Kp;
  ki = Ki;
  kd = Kd;

  if (controllerDirection == REVERSE) {
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
}

/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed
 ******************************************************************************/
void PID::SetSampleTime(Duration NewSampleTime) {
  if (NewSampleTime <= milliseconds(0)) {
    return;
  }
  nextSampleTime = nextSampleTime - SampleTime + NewSampleTime;
  SampleTime = NewSampleTime;
}

/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void PID::SetOutputLimits(float Min, float Max) {
  if (Min >= Max)
    return;
  outMin = Min;
  outMax = Max;

  *myOutput = std::clamp(*myOutput, outMin, outMax);
  outputSum = std::clamp(outputSum, outMin, outMax);
}
