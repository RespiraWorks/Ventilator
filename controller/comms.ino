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
#include "comms.h"

void comms_init() {
  // initialize serial communications at 115200 bps
  Serial.begin(115200);
}

void comms_send(double Setpoint, int sensorValue) {
  Serial.print("C"); //output to monitor
  Serial.write(int(map(Setpoint, 0, 255, 0, 1023)) >> 8); //output to monitor
  Serial.write(int(map(Setpoint, 0, 255, 0, 1023)) & 0xff); //output to monitor
  Serial.write(int(sensorValue) >> 8); //output to monitor
  Serial.write(int(sensorValue) & 0xff); //output to monitor
}
