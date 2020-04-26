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

#ifndef PRVC_H
#define PRVC_H

#include "network_protocol.pb.h"

void PRVC_init(double *Setpoint);
void PRVC_handler(const SensorReadings &readings, double *Setpoint);
void PRVC_start(double *Setpoint);
void PRVC_stop(double *Setpoint);

#endif // PRVC_H
