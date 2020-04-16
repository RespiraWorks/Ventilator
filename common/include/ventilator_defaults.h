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

#ifndef VENTILATOR_DEFAULTS_H
#define VENTILATOR_DEFAULTS_H

// Ventilator parameter defaults
// FIXME Put some default fallback values that are realistic
#define RR_DEFAULT    (10.0)
#define TV_DEFAULT    (10.0)
#define PEEP_DEFAULT  (10.0)
#define ED_DEFAULT    (10.0)
#define ID_DEFAULT    (10.0)
#define PIP_DEFAULT   (10.0)
#define DWELL_DEFAULT (10.0)
#define IER_DEFAULT   (1.0)

#define PERIODIC_READINGS_DEFAULT   false

#define OPERATING_MODE_DEFAULT      operatingMode::medical
#define PERIODIC_DATA_MODE_DEFAULT  periodicMode::off
#define VENTILATOR_MODE_DEFAULT     ventilatorMode::PRVC
#define SOLENOID_NORMAL_STATE       solenoidNormaleState::normally_open

// Calibration parameter defaults

// PID parameter defaults

#define KP_DEFAULT   (2.0)
#define KI_DEFAULT   (8.0)
#define KD_DEFAULT   (0.0)

#endif // VENTILATOR_DEFAULTS_H
