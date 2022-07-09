/* Copyright 2020-2022, RespiraWorks

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

#pragma once

#include "actuators.h"
#include "nvparams.h"
#include "sensors.h"
#include "uart_base.h"

class AbstractSystem {
 public:
  virtual void init() = 0;
  virtual UART::Channel* comms_channel() = 0;
  virtual UART::Channel* debug_channel() = 0;
  virtual I2Ceeprom* eeprom() = 0;
  virtual NVParams::Handler* non_volatile_memory() = 0;
  virtual AbstractSensors* sensors() = 0;
  virtual AbstractActuators* actuators() = 0;
};
