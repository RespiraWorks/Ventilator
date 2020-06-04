#ifndef __UART_DMA_STREAM__
#define __UART_DMA_STREAM__
#include "uart_dma.h"
extern UART_DMA uart_dma;

// Maintains a double buffer that is assigned to DMA for transmission over UART.
// One buffer is read by DMA and the other is written by software.
// DMA transmission will be triggered and buffers exchanged when:
// * buffer is full or
// * END_OF_STREAM is received
// Transmissions are scheduled to re-trigger if there is an ongoing DMA
// transmission. That is done by interrupt from UART_DMA driver.
//
// Returns flags:
// * ERROR_STREAM_BROKEN if DMA error interrupt detected
// * ERROR_BUFFER_FULL if an attempt was made to write to stream while all
//   buffers were full
// * WARNING_BUFFER_FULL if the buffer bacame full after the last char written
// * STREAM_SUCCESS if there is space available in buffers after the char is
// written

template <int BUF_LEN>
class DmaStream : public OutputStream, public TxListener {
  // static constexpr uint32_t BUF_LEN = 400;
  uint8_t buf1[BUF_LEN];
  uint8_t buf2[BUF_LEN];
  uint32_t i = 0;
  uint8_t *buf = buf1;
  bool schedule_tx = false;
  bool tx_error = false;

public:
  StreamResponse Put(int32_t b) override {
    if (tx_error) {
      tx_error = false;
      return {.count_written = 0, .flags = ERROR_STREAM_BROKEN};
    }
    // TODO thread safety
    if (END_OF_STREAM == b) {
      return {.count_written = 0, .flags = Transmit()};
    }

    if (is_buf_full()) {
      return {.count_written = 0, .flags = ERROR_BUFFER_FULL};
    } else {
      buf[i++] = static_cast<uint8_t>(b);
      if (is_buf_full()) {
        return {.count_written = 1, .flags = Transmit()};
      } else {
        return {.count_written = 1, .flags = STREAM_SUCCESS};
      }
    }
  }

  uint32_t BytesAvailableForWrite() override {
    // TODO thread safety
    if (uart_dma.isTxInProgress()) {
      return BUF_LEN - i;
    } else {
      return 2 * BUF_LEN - i;
    }
  }

  void onTxComplete() override {
    if (schedule_tx) {
      // started flag can be safely ignored as we are in TxComplete handler that
      // gets called when there is no more active tx happening
      [[maybe_unused]] bool started = uart_dma.startTX(buf, i, this);
      SwapBuffers();
      schedule_tx = false;
    }
  }

  // Only Very Bad (tm) things can cause this.
  // meaning that DMA configuration is not right.
  void onTxError() override { tx_error = true; }

private:
  // Starts DMA transmission if transmission is not ongoing
  ResponseFlags Transmit() {
    if (is_buf_full() && uart_dma.isTxInProgress()) {
      schedule_tx = true;
      return WARNING_BUFFER_FULL;
    }

    if (!is_buf_full() && uart_dma.isTxInProgress()) {
      schedule_tx = true;
      return STREAM_SUCCESS;
    }
    // we can safely ignore this started flag as we have checked that
    // transmission is not in progress before
    [[maybe_unused]] bool started = uart_dma.startTX(buf, i, this);
    SwapBuffers();
    return STREAM_SUCCESS;
  }

  bool is_buf_full() { return i >= BUF_LEN; }

  void SwapBuffers() {
    if (buf1 == buf) {
      buf = buf2;
    } else if (buf2 == buf) {
      buf = buf1;
    } else {
      // halt and catch on fire
    }
    i = 0;
  }
};
#endif
