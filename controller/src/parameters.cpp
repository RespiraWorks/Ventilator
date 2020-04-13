/* Copyright 2020, Edwin Chiu

  This file is part of FixMoreLungs.

  FixMoreLungs is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FixMoreLungs is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FixMoreLungs.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <Arduino.h>

#include "parameters.h"

// All ventilator settings parameters
static float rr;
static float tv;
static float peep;
static float ier;       /* Inspire / expire ratio */
static float pip;
static float dwell;

static bool periodicReadings;

static enum operatingMode operationMode;
static enum periodicMode periodicDataMode;

// Calibration parameters


// PID parameters
static float Kp_pid;
static float Ki_pid;
static float Kd_pid;

void parameters_init() {
    //eeprom_init();

    init_defaultVentilatorParameters();
    init_defaultPIDParameters();
    init_defaultCalibrationParameters();
}

static void init_defaultVentilatorParameters() {
    rr                = RR_DEFAULT;
    tv                = TV_DEFAULT;
    peep              = PEEP_DEFAULT;
    ier               = IER_DEFAULT;
    pip               = PIP_DEFAULT;
    dwell             = DWELL_DEFAULT;
    periodicReadings  = PERIODIC_READINGS;

    operationMode     = OPERATING_MODE;
    periodicDataMode  = PERIODIC_DATA_MODE;
}

static void init_defaultPIDParameters() {
    Kp_pid    = KP_DEFAULT;
    Ki_pid    = KI_DEFAULT;
    Kd_pid    = KD_DEFAULT;
}

static void init_defaultCalibrationParameters() {

}

void parameters_setKp(float kp_value) {
    Kp_pid = kp_value;
}

float parameters_getKp() {
    return Kp_pid;
}

void parameters_setKi(float ki_value) {
    Ki_pid = ki_value;
}

float parameters_getKi() {
    return Ki_pid;
}

void parameters_setKd(float kd_value) {
    Kd_pid = kd_value;
}

float parameters_getKd() {
    return Kd_pid;
}

void parameters_setRR(float rr_value) {

    // Make sure the uploaded values are within safe minimums and maximums
    // If not, clamp them
    if(rr_value > RR_MAX)
        rr = RR_MAX;
    else if (rr_value < RR_MIN)
        rr = RR_MIN;
    else
        rr = rr_value;
}

float parameters_getRR() {
    return rr;
}

void parameters_setTV(float tv_value) {

    // Make sure the uploaded values are within safe minimums and maximums
    // If not, clamp them
    if(tv_value > TV_MAX)
        tv = TV_MAX;
    else if (tv_value < TV_MIN)
        tv = TV_MIN;
    else
        tv = tv_value;
}

float parameters_getTV() {
    return tv;
}

void parameters_setPEEP(float peep_value) {

    // Make sure the uploaded values are within safe minimums and maximums
    // If not, clamp them
    if(peep_value > PEEP_MAX)
        peep = PEEP_MAX;
    else if (peep_value < PEEP_MIN)
        peep = PEEP_MIN;
    else
        peep = peep_value;
}

float parameters_getPEEP() {
    return peep;
}

void parameters_setInspireExpireRatio(float ier_value) {

    // Make sure the uploaded values are within safe minimums and maximums
    // If not, clamp them
    if(ier_value > IER_MAX)
        ier = IER_MAX;
    else if (ier_value < IER_MIN)
        ier = IER_MIN;
    else
        ier = ier_value;
}

float parameters_getInspireExpireRatio() {
    return ier;
}

void parameters_setPIP(float pip_value) {

    // Make sure the uploaded values are within safe minimums and maximums
    // If not, clamp them
    if(pip_value > PIP_MAX)
        pip = PIP_MAX;
    else if (pip_value < PIP_MIN)
        pip = PIP_MIN;
    else
        pip = pip_value;
}

float parameters_getPIP() {
    return pip;
}

void parameters_setDwell(float dwell_value) {

    // Make sure the uploaded values are within safe minimums and maximums
    // If not, clamp them
    if(dwell_value > DWELL_MAX)
        dwell = DWELL_MAX;
    else if (dwell_value < DWELL_MIN)
        dwell = DWELL_MIN;
    else
        dwell = dwell_value;
}

float parameters_getDwell() {
    return dwell;
}

void parameters_setPeriodicMode(enum periodicMode periodicMode_value) {
    periodicDataMode = periodicMode_value;
}

enum periodicMode parameters_getPeriodicMode() {
    return periodicDataMode;
}

enum operatingMode parameters_getOperatingMode() {
    return operationMode;
}

void parameters_setOperatingMode(enum operatingMode operatingMode_value) {
    operationMode = operatingMode_value;
}

void parameters_setPeriodicReadings(bool active) {
    periodicReadings = active;
}

bool parameters_getPeriodicReadings() {
    return periodicReadings;
}
