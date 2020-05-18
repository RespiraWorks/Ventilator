#include "network_protocol.pb.h"
#include "uart_dma.h"

extern UART_DMA dmaUART;

class FramingRxFSM : public UART_DMA_RxListener {
  enum State_t { STATE_LOST, STATE_WAIT_START, STATE_RX_FRAME };
  static constexpr uint32_t RX_TIMEOUT = 115200 * 10;

  State_t state = STATE_LOST;
  uint32_t errorCounter = 0;
  // Size of the buffer is set asuming a corner case where EVERY GuiStatus
  // byte and CRC32 will be escaped + two marker chars; this is too big, but
  // safe.
  static constexpr uint32_t RX_BUF_LEN = (GuiStatus_size + 4) * 2 + 2;
  static constexpr uint32_t RX_BYTES_MAX = RX_BUF_LEN;
  uint8_t rx_buf[RX_BUF_LEN];

public:
  void begin() {
    state = STATE_LOST;
    dmaUART.charMatchEnable();
    dmaUART.startRX(rx_buf, RX_BYTES_MAX, RX_TIMEOUT);
  }

  void onRxComplete() {
    // We should never reach the full read of rx buffer.
    // If we get here, this means, there are no marker
    // chars in the stream, so we are lost
    errorCounter++;
    state = STATE_LOST;
    restartRX();
  }

  void onCharacterMatch() {
    switch (state) {
    case STATE_LOST:
      // if we have received something before this marker,
      // we assume, this is the frame end marker, so wait
      // for start
      if (receivedBytesCount() > 1) {
        state = STATE_WAIT_START;
        // if we were lucky to get lost in the interframe silence,
        // assume this is the start of the frame
      } else if (receivedBytesCount() == 1) {
        state = STATE_RX_FRAME;
      } else {
        // Should never end up here
        // DMA is not working?
      }
      break;
    case STATE_WAIT_START:
      if (receivedBytesCount() == 1) {
        state = STATE_RX_FRAME;
      } else {
        // some junk received during start wait,
        // but should have been just silence
        errorCounter++;
        state = STATE_LOST;
      }
      break;
    case STATE_RX_FRAME:
      if (receivedBytesCount() > 1) {
        processReceivedData();
        state = STATE_WAIT_START;
      } else {
        // repeated marker char received
      }
      break;
    }
    restartRX();
  }

  void onRxError(RxError_t e) {
    switch (state) {
    case STATE_LOST:
    case STATE_WAIT_START:
      // no change
      break;
    case STATE_RX_FRAME:
      state = STATE_LOST;
      break;
    }
    errorCounter++;
  };

private:
  uint32_t receivedBytesCount() {
    return (RX_BYTES_MAX - dmaUART.getRxBytesLeft());
  }

  void restartRX() {
    dmaUART.stopRX();
    dmaUART.startRX(rx_buf, RX_BYTES_MAX, RX_TIMEOUT);
  }

  void processReceivedData() { rx_buf[receivedBytesCount() - 1] = 0; }
};
