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

#include "uart_dma.h"
extern UartDma uart_dma;

// Maintains a double buffer that is assigned to DMA for transmission over UART.
// One buffer is read by DMA and the other is written by software.
// DMA transmission will be triggered and buffers exchanged when:
//  * buffer is full or
//  * EndOfStream is received
// Transmissions are scheduled to re-trigger if there is an ongoing DMA transmission.
// This is done by interrupt from UartDma driver.
//
// Returns flags:
//  * ErrorStreamBroken if DMA error interrupt detected
//  * ErrorBufferFull if an attempt was made to write to stream while all buffers were full
//  * WarningBufferFull if the buffer became full after the last char written
//  * StreamSuccess if there is space available in buffers after the char is written

template <uint32_t BufferLength>
class DmaStream : public OutputStream, public TxListener {
 public:
  StreamResponse put(int32_t b) override {
    if (tx_error_) {
      tx_error_ = false;
      return {.count_written = 0, .flags = ResponseFlags::ErrorStreamBroken};
    }
    // TODO thread safety
    if (EndOfStream == b) {
      return {.count_written = 0, .flags = transmit()};
    }

    if (is_buf_full()) {
      return {.count_written = 0, .flags = ResponseFlags::ErrorBufferFull};
    } else {
      current_buffer_[index_++] = static_cast<uint8_t>(b);
      if (is_buf_full()) {
        return {.count_written = 1, .flags = transmit()};
      } else {
        return {.count_written = 1, .flags = ResponseFlags::StreamSuccess};
      }
    }
  }

  uint32_t bytes_available_for_write() override {
    // TODO thread safety
    if (uart_dma.tx_in_progress()) {
      return BufferLength - index_;
    } else {
      return 2 * BufferLength - index_;
    }
  }

  void on_tx_complete() override {
    if (schedule_tx_) {
      // started flag can be safely ignored as we are in TxComplete handler that gets called when
      // there is no more active tx happening
      [[maybe_unused]] bool started = uart_dma.start_tx(current_buffer_, index_, this);
      swap_buffers();
      schedule_tx_ = false;
    }
  }

  // Only Very Bad (tm) things can cause this -- meaning that DMA configuration is not right.
  void on_tx_error() override { tx_error_ = true; }

 private:
  // Starts DMA transmission if transmission is not ongoing
  ResponseFlags transmit() {
    if (is_buf_full() && uart_dma.tx_in_progress()) {
      schedule_tx_ = true;
      return ResponseFlags::WarningBufferFull;
    }

    if (!is_buf_full() && uart_dma.tx_in_progress()) {
      schedule_tx_ = true;
      return ResponseFlags::StreamSuccess;
    }
    // we can safely ignore this started flag as we have checked that transmission is not in
    // progress before
    [[maybe_unused]] bool started = uart_dma.start_tx(current_buffer_, index_, this);
    swap_buffers();
    return ResponseFlags::StreamSuccess;
  }

  bool is_buf_full() { return index_ >= BufferLength; }

  void swap_buffers() {
    if (buffer1_ == current_buffer_) {
      current_buffer_ = buffer2_;
    } else if (buffer2_ == current_buffer_) {
      current_buffer_ = buffer1_;
    } else {
      // halt and catch on fire
    }
    index_ = 0;
  }

  uint8_t buffer1_[BufferLength];
  uint8_t buffer2_[BufferLength];
  uint32_t index_{0};
  uint8_t *current_buffer_{buffer1_};
  bool schedule_tx_{false};
  bool tx_error_{false};
};
