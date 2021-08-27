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

template <uint32_t RxBytesMax>
class SoftRxBuffer {
 public:
  explicit SoftRxBuffer(uint8_t match_char) : match_char_(match_char) {}

  void restart_rx(RxListener *listener) {
    rx_index_ = 0;
    rx_listener_ = listener;
  }

  bool begin(RxListener *listener) {
    restart_rx(listener);
    return true;
  };

  uint32_t received_length() { return rx_index_; }

  uint8_t *get() { return rx_buffer_; }

  void put_byte(uint8_t b) {
    if (rx_index_ < RxBytesMax) {
      rx_buffer_[rx_index_++] = b;
      if (match_char_ == b) {
        if (rx_listener_) {
          rx_listener_->on_character_match();
        }
      }
    }

    // \TODO: this never be >? or this could just be an else branch?
    if (rx_index_ >= RxBytesMax) {
      if (rx_listener_) {
        rx_listener_->on_rx_complete();
      }
    }
  }

 private:
  uint8_t rx_buffer_[RxBytesMax];
  uint32_t rx_index_{0};
  RxListener *rx_listener_{nullptr};
  uint8_t match_char_{0};
};
