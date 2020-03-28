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

#include "parameters.h"

// All ventilator settings parameters
static float rr;
static float tv;
static float peep;
static float expireduration;
static float inspireDuration;
static float pip;
static float dwell;

// Calibration parameters


// PID parameters
static float Kp;
static float Ki;
static float Kd;

void parameters_init() {
    //eeprom_init();

    init_default_ventilatorParameters();
    init_default_PIDParameters();
    init_default_CalibrationParameters();
}

static void init_default_ventilatorParameters() {
    rr                = RR_DEFAULT;
    tv                = TV_DEFAULT;
    peep              = PEEP_DEFAULT;
    expireduration    = ED_DEFAULT;
    inspireDuration   = ID_DEFAULT;
    pip               = PIP_DEFAULT;
    dwell             = DWELL_DEFAULT;
}

static void init_default_PIDParameters() {
    Kp    = KP_DEFAULT;
    Ki    = KI_DEFAULT;
    Kd    = KD_DEFAULT;
}

static void init_default_CalibrationParameters() {

}

void parameters_set_Kp(float kp_value) {
    Kp = kp_value;
}

float parameters_get_Kp() {
    return Kp;
}

void parameters_set_Ki(float ki_value) {
    Ki = ki_value;
}

float parameters_get_Ki() {
    return Ki;
}

void parameters_set_Kd(float kd_value) {
    Kd = kd_value;
}

float parameters_get_Kd() {
    return Kd;
}

void parameters_set_RR(float rr_value) {
    rr = rr_value;
}

float parameters_get_RR() {
    return rr;
}

void parameters_set_TV(float tv_value) {
    tv = tv_value;
}

float parameters_get_TV() {
    return tv;
}

void parameters_set_PEEP(float peep_value) {
    peep = peep_value;
}

float parameters_get_PEEP() {
    return peep;
}

void parameters_set_ExpireDuration(float expireduration_value) {
    expireduration = expireduration_value
}

float parameters_get_ExpireDuration() {
    return expireduration;
}

void parameters_set_InspireDuration(float inspireduration_value) {
    inspireduration = inspireduration_value
}

float parameters_get_InspireDuration() {
    return inspireduration;
}

void parameters_set_PIP(float pip_value) {
    pip = pip_value;
}

float parameters_get_PIP() {
    return pip;
}

void parameters_set_Dwell(float dwell_value) {
    dwell = dwell_value;
}

float parameters_get_Dwell() {
    return dwell;
}