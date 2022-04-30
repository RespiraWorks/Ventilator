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

#include "rx_buffer.h"

/// \TODO: This class will be used for GUI only? Should it live with that code?
/// \TODO: Class is not explicitly tested, only as part of FrameDetector

/*! \class SoftRxBuffer soft_rx_buffer.h "soft_rx_buffer.h"
 *  \brief This class is used for GUI but is also convenient for testing
 *     the FrameDetector class since bytes can be manually added to buffer.
 */
template <size_t RxBytesMax>
class SoftRxBuffer : public RxBuffer {
 public:
  explicit SoftRxBuffer(uint8_t match_char);

  void restart_rx(RxListener* listener) override;
  bool begin(RxListener* listener) override;
  size_t received_length() const override;
  uint8_t* get() override;

  void put_byte(uint8_t byte);

 private:
  uint8_t rx_buffer_[RxBytesMax] = {0};
  size_t rx_index_{0};
  RxListener* rx_listener_{nullptr};
  uint8_t match_char_{0};
};

/// Includes function implementations so they don't have to live in header
#include "soft_rx_buffer.tpp"
