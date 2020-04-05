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
static float expireduration;
static float inspireduration;
static float pip;
static float dwell;

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
    expireduration    = ED_DEFAULT;
    inspireduration   = ID_DEFAULT;
    pip               = PIP_DEFAULT;
    dwell             = DWELL_DEFAULT;
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

void parameters_set_RR(float rr_value) {
    rr = rr_value;
}

float parameters_getRR() {
    return rr;
}

void parameters_setTV(float tv_value) {
    tv = tv_value;
}

float parameters_getTV() {
    return tv;
}

void parameters_setPEEP(float peep_value) {
    peep = peep_value;
}

float parameters_getPEEP() {
    return peep;
}

void parameters_set_ExpireDuration(float expireduration_value) {
    expireduration = expireduration_value;
}

float parameters_getExpireDuration() {
    return expireduration;
}

void parameters_setInspireDuration(float inspireduration_value) {
    inspireduration = inspireduration_value;
}

float parameters_getInspireDuration() {
    return inspireduration;
}

void parameters_setPIP(float pip_value) {
    pip = pip_value;
}

float parameters_getPIP() {
    return pip;
}

void parameters_setDwell(float dwell_value) {
    dwell = dwell_value;
}

float parameters_getDwell() {
    return dwell;
}
