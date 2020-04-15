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

static bool periodicReadings;

// Calibration parameters


// PID parameters
static float Kp_pid;
static float Ki_pid;
static float Kd_pid;

static void init_defaultVentilatorParameters();
static void init_defaultPIDParameters();
static void init_defaultCalibrationParameters();

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
    periodicReadings  = PERIODIC_READINGS;
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

bool parameters_medicalModeActive() {
    return false; // FIXME return parameter
}

void parameters_setPeriodicReadings(bool active) {
    periodicReadings = active;
}

bool parameters_getPeriodicReadings() {
    return periodicReadings;
}
