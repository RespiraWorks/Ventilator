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

#include "framing_streams.h"
#include "uart_base.h"

// Maintains a buffer that is assigned for transmission over UART.
// UART transmission will be triggered when:
//  * buffer is full or
//  * EndOfStream is received
// Transmissions are scheduled to re-trigger if there is an ongoing transmission.
// This is done by interrupt from Uart driver.
//
// Returns flags:
//  * ErrorStreamBroken if UART error interrupt detected
//  * ErrorBufferFull if an attempt was made to write to stream while all buffers were full
//  * WarningBufferFull if the buffer became full after the last char written
//  * StreamSuccess if there is space available in buffers after the char is written

template <size_t BufferLength>
class UartStream : public OutputStream, public TxListener {
 public:
  explicit UartStream(UART::Channel *uart) : uart_(uart){};

  StreamResponse put(int32_t b) override {
    if (tx_error_) {
      tx_error_ = false;
      return {/*count_written=*/0, ResponseFlags::ErrorStreamBroken};
    }
    // TODO thread safety
    if (EndOfStream == b) {
      return {/*count_written=*/0, transmit()};
    }

    if (buffer_full()) {
      return {/*count_written=*/0, ResponseFlags::ErrorBufferFull};
    } else {
      buffer_[index_++] = static_cast<uint8_t>(b);
      if (buffer_full()) {
        return {/*count_written=*/1, transmit()};
      }
      return {/*count_written=*/1, ResponseFlags::StreamSuccess};
    }
  }

  size_t bytes_available_for_write() override { return BufferLength - index_; }

  void on_tx_complete() override {
    if (schedule_tx_) {
      // return value can be safely ignored as we are in TxComplete handler that gets called when
      // there is no more active tx happening, meaning the uart buffer is empty.
      uart_->Write(buffer_, index_, this);
      index_ = 0;
      schedule_tx_ = false;
    }
  }

  // Only Very Bad (tm) things can cause this -- meaning that DMA configuration is not right.
  void on_tx_error() override { tx_error_ = true; }

 private:
  // Starts transmission if no transmission is ongoing
  ResponseFlags transmit() {
    size_t bytes_written = uart_->Write(buffer_, index_, this);
    if (bytes_written == index_) {
      index_ = 0;
      schedule_tx_ = false;
      return ResponseFlags::StreamSuccess;
    }
    schedule_tx_ = true;
    if (buffer_full()) {
      return ResponseFlags::WarningBufferFull;
    }
    return ResponseFlags::StreamSuccess;
  }

  bool buffer_full() { return index_ >= BufferLength; }

  uint8_t buffer_[BufferLength] = {0};
  size_t index_{0};
  bool schedule_tx_{false};
  bool tx_error_{false};
  UART::Channel *uart_;
};
