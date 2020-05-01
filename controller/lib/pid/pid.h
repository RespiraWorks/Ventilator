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

class PID {

public:
// Constants used in some of the functions below
#define AUTOMATIC 1
#define MANUAL 0
#define DIRECT 0
#define REVERSE 1
#define P_ON_M 0
#define P_ON_E 1

  // commonly used functions
  // **************************************************************************
  PID(double *, double *,
      double *, // * constructor.  links the PID to the Input, Output, and
      double, double, double, int,
      int); //   Setpoint.  Initial tuning parameters are also set here.
            //   (overload for specifying proportional mode)

  PID(double *, double *,
      double *, // * constructor.  links the PID to the Input, Output, and
      double, double, double,
      int); //   Setpoint.  Initial tuning parameters are also set here

  void SetMode(int Mode); // * sets PID to either Manual (0) or Auto (non-0)

  bool Compute(); // * performs the PID calculation.  it should be
                  //   called every time loop() cycles. ON/OFF and
                  //   calculation frequency can be set using SetMode
                  //   SetSampleTime respectively

  void SetOutputLimits(
      double,
      double); // * clamps the output to a specific range. 0-255 by default, but
               //   it's likely the user will want to change this depending on
               //   the application

  // available but not commonly used functions
  // ********************************************************
  void SetTunings(
      double, double, // * While most users will set the tunings once in the
      double);        //   constructor, this function gives the user the option
               //   of changing tunings during runtime for Adaptive control
  void SetTunings(double, double, // * overload for specifying proportional mode
                  double, int);

  void SetControllerDirection(
      int); // * Sets the Direction, or "Action" of the controller. DIRECT
            //   means the output will increase when error is positive. REVERSE
            //   means the opposite.  it's very unlikely that this will be
            //   needed once it is set in the constructor.

  // Sets the frequency with which the PID calculation is performed.  Default
  // is 100ms.
  void SetSampleTime(Duration);

  // Display functions
  // ****************************************************************
  double GetKp();     // These functions query the pid for interal values.
  double GetKi();     //  they were created mainly for the pid front-end,
  double GetKd();     // where it's important to know what is actually
  int GetSampleTime();
  int GetMode();      //  inside the PID.
  int GetDirection(); //

private:
  void Initialize();

  double kp; // * (P)roportional Tuning Parameter
  double ki; // * (I)ntegral Tuning Parameter
  double kd; // * (D)erivative Tuning Parameter

  int controllerDirection;
  int pOn;

  double *myInput;  // * Pointers to the Input, Output, and Setpoint variables
  double *myOutput; //   This creates a hard link between the variables and the
  double
      *mySetpoint; //   PID, freeing the user from having to constantly tell us
                   //   what these values are.  with pointers we'll just know.

  Duration lastTime, lastSampleTime;
  double outputSum, lastInput;

  double outMin, outMax;
  bool inAuto, pOnE;
};
#endif
