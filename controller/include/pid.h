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
#ifndef PID_H
#define PID_H

#include <PID_v1.h>
#include <stdint.h>

#define BLOWERSPD_PIN 3
#define BLOWER_HIGH 142
#define BLOWER_LOW 130
#define DPSENSOR_PIN A0

//state machine variables
#define INSPIRE_TIME 1600
#define INSPIRE_RATE 1
#define PIP 142
#define INSPIRE_DWELL 800
#define INSPIRE_DWELL_PRESSURE 140
#define EXPIRE_TIME 1000
#define EXPIRE_RATE 1
#define PEEP 130
#define EXPIRE_DWELL 600

//not implemented yet
#define AC 0
#define RR 0
#define IE 0

void pid_execute();
void pid_init();

#endif  // PID_H
