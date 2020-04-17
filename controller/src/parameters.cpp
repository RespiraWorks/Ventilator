/* Copyright 2020, Edwin Chiu

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
#include "parameters.h"

/****************************************************************************************
 *    PRIVATE FUNCTION PROTOTYPES
 ****************************************************************************************/

static void init_defaultVentilatorParameters();
static void init_defaultPIDParameters();
static void init_defaultCalibrationParameters();

/****************************************************************************************
 *    PRIVATE VARIABLES
 ****************************************************************************************/

// All ventilator settings parameters
static float rr;
static float tv;
static float peep;
static float ier; /* Inspire / expire ratio */
static float pip;
static float dwell;

static bool periodicReadings;

static enum operatingMode operationMode;
static enum periodicMode periodicDataMode;
static enum ventilatorMode ventilatorOperatingMode;
static enum solenoidNormaleState normalState;

// Calibration parameters

// PID parameters
static float Kp_pid;
static float Ki_pid;
static float Kd_pid;

/****************************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************************/

void parameters_init() {
  // eeprom_init();

  init_defaultVentilatorParameters();
  init_defaultPIDParameters();
  init_defaultCalibrationParameters();
}

void parameters_setKp(float kp_value) { Kp_pid = kp_value; }

float parameters_getKp() { return Kp_pid; }

void parameters_setKi(float ki_value) { Ki_pid = ki_value; }

float parameters_getKi() { return Ki_pid; }

void parameters_setKd(float kd_value) { Kd_pid = kd_value; }

float parameters_getKd() { return Kd_pid; }

void parameters_setRR(float rr_value) {

  // Make sure the uploaded values are within safe minimums and maximums
  // If not, clamp them
  if (rr_value > RR_MAX)
    rr = RR_MAX;
  else if (rr_value < RR_MIN)
    rr = RR_MIN;
  else
    rr = rr_value;
}

float parameters_getRR() { return rr; }

void parameters_setTV(float tv_value) {

  // Make sure the uploaded values are within safe minimums and maximums
  // If not, clamp them
  if (tv_value > TV_MAX)
    tv = TV_MAX;
  else if (tv_value < TV_MIN)
    tv = TV_MIN;
  else
    tv = tv_value;
}

float parameters_getTV() { return tv; }

void parameters_setPEEP(float peep_value) {

  // Make sure the uploaded values are within safe minimums and maximums
  // If not, clamp them
  if (peep_value > PEEP_MAX)
    peep = PEEP_MAX;
  else if (peep_value < PEEP_MIN)
    peep = PEEP_MIN;
  else
    peep = peep_value;
}

float parameters_getPEEP() { return peep; }

void parameters_setInspireExpireRatio(float ier_value) {

  // Make sure the uploaded values are within safe minimums and maximums
  // If not, clamp them
  if (ier_value > IER_MAX)
    ier = IER_MAX;
  else if (ier_value < IER_MIN)
    ier = IER_MIN;
  else
    ier = ier_value;
}

float parameters_getInspireExpireRatio() { return ier; }

void parameters_setPIP(float pip_value) {

  // Make sure the uploaded values are within safe minimums and maximums
  // If not, clamp them
  if (pip_value > PIP_MAX)
    pip = PIP_MAX;
  else if (pip_value < PIP_MIN)
    pip = PIP_MIN;
  else
    pip = pip_value;
}

float parameters_getPIP() { return pip; }

void parameters_setDwell(float dwell_value) {

  // Make sure the uploaded values are within safe minimums and maximums
  // If not, clamp them
  if (dwell_value > DWELL_MAX)
    dwell = DWELL_MAX;
  else if (dwell_value < DWELL_MIN)
    dwell = DWELL_MIN;
  else
    dwell = dwell_value;
}

float parameters_getDwell() { return dwell; }

void parameters_setPeriodicMode(enum periodicMode periodicMode_value) {
  periodicDataMode = periodicMode_value;
}

enum periodicMode parameters_getPeriodicMode() { return periodicDataMode; }

void parameters_setOperatingMode(enum operatingMode operatingMode_value) {
  operationMode = operatingMode_value;
}

enum operatingMode parameters_getOperatingMode() { return operationMode; }

void parameters_setPeriodicReadings(bool active) { periodicReadings = active; }

bool parameters_getPeriodicReadings() { return periodicReadings; }

void parameters_setVentilatorMode(enum ventilatorMode ventilatorMode_value) {
  ventilatorOperatingMode = ventilatorMode_value;
}

enum ventilatorMode parameters_getVentilatorMode() {
  return ventilatorOperatingMode;
}

void parameters_setSolenoidNormalState(
    enum solenoidNormaleState normalState_value) {
  normalState = normalState_value;
}

enum solenoidNormaleState parameters_getSolenoidNormalState() {
  return normalState;
}

/****************************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************************/

static void init_defaultVentilatorParameters() {
  rr = RR_DEFAULT;
  tv = TV_DEFAULT;
  peep = PEEP_DEFAULT;
  ier = IER_DEFAULT;
  pip = PIP_DEFAULT;
  dwell = DWELL_DEFAULT;
  periodicReadings = PERIODIC_READINGS_DEFAULT;

  operationMode = OPERATING_MODE_DEFAULT;
  periodicDataMode = PERIODIC_DATA_MODE_DEFAULT;
  ventilatorOperatingMode = VENTILATOR_MODE_DEFAULT;
  normalState = SOLENOID_NORMAL_STATE;
}

static void init_defaultPIDParameters() {
  Kp_pid = KP_DEFAULT;
  Ki_pid = KI_DEFAULT;
  Kd_pid = KD_DEFAULT;
}

static void init_defaultCalibrationParameters() {}
