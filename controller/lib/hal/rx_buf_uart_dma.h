#ifndef __HAL_TRANSPORT
#include "network_protocol.pb.h"
#include "uart_dma.h"

// RX buffer wrapper that controls restart of reception and provides count of
// received bytes.
// We need this class to abstract hardware access from FrameDetector FSM so it
// can be used in GUI code
template <int RX_BYTES_MAX> class RxBufferUartDma {
public:
  RxBufferUartDma(UART_DMA &uart_dma) : uart_dma_(uart_dma){};

  // Sets up underlying receive infrastructure and starts the first reception
  [[nodiscard]] bool Begin(RxListener *rxl) {
    uart_dma_.charMatchEnable();
    return uart_dma_.startRX(rx_buf_, RX_BYTES_MAX, RX_TIMEOUT_, rxl);
  }

  // Restarts the ongoing reception, this means the rx_buf will be written from
  // the beginning
  void RestartRX(RxListener *rxl) {
    uart_dma_.stopRX();
    [[maybe_unused]] bool started =
        uart_dma_.startRX(rx_buf_, RX_BYTES_MAX, RX_TIMEOUT_, rxl);
  }

  // Returns how many bytes were written into rx_buf
  uint32_t ReceivedLength() {
    return (RX_BYTES_MAX - uart_dma_.getRxBytesLeft());
  }

  // Returns the rx_buf
  uint8_t *get() { return rx_buf_; }

#ifdef TEST_MODE
  // Puts a byte to rx_buf
  void test_PutByte(uint8_t b);
#endif

private:
  // UART_DMA provides receiving to this buffer
  UART_DMA &uart_dma_;
  // Buffer into which the data is received
  uint8_t rx_buf_[RX_BYTES_MAX];
  // If no activity is happening on RX line for the given amount of time,
  // listener will get an onError callback.
  // TODO determine the right amount of time via risk analysis
  static constexpr Duration RX_TIMEOUT_ = seconds(10);
};

#ifdef TEST_MODE
extern uint32_t rx_i;
// Puts a byte to rx_buf
template <int RX_BYTES_MAX>
void RxBufferUartDma<RX_BYTES_MAX>::test_PutByte(uint8_t b) {
  rx_buf_[rx_i++] = b;
}
#endif

#endif
