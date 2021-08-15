#pragma once

#include "uart_dma.h"
extern UartDma uart_dma;

// Maintains a double buffer that is assigned to DMA for transmission over UART.
// One buffer is read by DMA and the other is written by software.
// DMA transmission will be triggered and buffers exchanged when:
// * buffer is full or
// * EndOfStream is received
// Transmissions are scheduled to re-trigger if there is an ongoing DMA
// transmission. That is done by interrupt from UartDma driver.
//
// Returns flags:
// * ErrorStreamBroken if DMA error interrupt detected
// * ErrorBufferFull if an attempt was made to write to stream while all
//   buffers were full
// * WarningBufferFull if the buffer bacame full after the last char written
// * StreamSuccess if there is space available in buffers after the char is
// written

template <int BufferLength>
class DmaStream : public OutputStream, public TxListener {

public:
  StreamResponse Put(int32_t b) override {
    if (tx_error_) {
      tx_error_ = false;
      return {.count_written = 0, .flags = ResponseFlags::ErrorStreamBroken};
    }
    // TODO thread safety
    if (EndOfStream == b) {
      return {.count_written = 0, .flags = Transmit()};
    }

    if (is_buf_full()) {
      return {.count_written = 0, .flags = ResponseFlags::ErrorBufferFull};
    } else {
      current_buffer_[index_++] = static_cast<uint8_t>(b);
      if (is_buf_full()) {
        return {.count_written = 1, .flags = Transmit()};
      } else {
        return {.count_written = 1, .flags = ResponseFlags::StreamSuccess};
      }
    }
  }

  uint32_t BytesAvailableForWrite() override {
    // TODO thread safety
    if (uart_dma.tx_in_progress()) {
      return BufferLength - index_;
    } else {
      return 2 * BufferLength - index_;
    }
  }

  void on_tx_complete() override {
    if (schedule_tx_) {
      // started flag can be safely ignored as we are in TxComplete handler that
      // gets called when there is no more active tx happening
      [[maybe_unused]] bool started =
          uart_dma.start_tx(current_buffer_, index_, this);
      SwapBuffers();
      schedule_tx_ = false;
    }
  }

  // Only Very Bad (tm) things can cause this.
  // meaning that DMA configuration is not right.
  void on_tx_error() override { tx_error_ = true; }

private:
  // Starts DMA transmission if transmission is not ongoing
  ResponseFlags Transmit() {
    if (is_buf_full() && uart_dma.tx_in_progress()) {
      schedule_tx_ = true;
      return ResponseFlags::WarningBufferFull;
    }

    if (!is_buf_full() && uart_dma.tx_in_progress()) {
      schedule_tx_ = true;
      return ResponseFlags::StreamSuccess;
    }
    // we can safely ignore this started flag as we have checked that
    // transmission is not in progress before
    [[maybe_unused]] bool started =
        uart_dma.start_tx(current_buffer_, index_, this);
    SwapBuffers();
    return ResponseFlags::StreamSuccess;
  }

  bool is_buf_full() { return index_ >= BufferLength; }

  void SwapBuffers() {
    if (buffer1_ == current_buffer_) {
      current_buffer_ = buffer2_;
    } else if (buffer2_ == current_buffer_) {
      current_buffer_ = buffer1_;
    } else {
      // halt and catch on fire
    }
    index_ = 0;
  }

  // static constexpr uint32_t BufferLength = 400;
  uint8_t buffer1_[BufferLength];
  uint8_t buffer2_[BufferLength];
  uint32_t index_{0};
  uint8_t *current_buffer_{buffer1_};
  bool schedule_tx_{false};
  bool tx_error_{false};
};
