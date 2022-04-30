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

#include <cstddef>
#include <cstdint>

#include "serial_listeners.h"

/*! \class RxBuffer rx_buffer.h "rx_buffer.h"
 *  \brief Abstract class for serial receiver buffers.
 */
class RxBuffer {
 public:
  /// \brief Sets up underlying receive infrastructure and starts the first reception.
  virtual bool begin(RxListener *listener) = 0;

  /// \brief Restarts the ongoing reception, i.e. the buffer will be written from the beginning.
  virtual void restart_rx(RxListener *listener) = 0;

  /// \brief Returns how many bytes were written to the buffer.
  virtual size_t received_length() const = 0;

  /// \brief Returns the buffer.
  virtual uint8_t *get() = 0;
};
