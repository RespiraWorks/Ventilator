/* Copyright 2020-2021, RespiraWorks

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

#include <cstdint>

#include "serial_listeners.h"

class RxBuffer {
 public:
  // Sets up underlying receive infrastructure and starts the first reception
  virtual bool begin(RxListener *listener) = 0;

  // Restarts the ongoing reception, i.e. the buffer will be written from the beginning
  virtual void restart_rx(RxListener *listener) = 0;

  // Returns how many bytes were written to the buffer
  virtual size_t received_length() const = 0;

  // Returns the buffer
  virtual const uint8_t *get() const = 0;
};
