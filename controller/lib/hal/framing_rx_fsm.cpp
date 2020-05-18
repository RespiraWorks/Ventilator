#include "framing_rx_fsm.h"
#include "network_protocol.pb.h"
#include "uart_dma.h"

extern UART_DMA dmaUART;

void FramingRxFSM::begin() {
  state = STATE_LOST;
  dmaUART.charMatchEnable();
  dmaUART.startRX(rx_buf, RX_BYTES_MAX, RX_TIMEOUT, this);
}

void FramingRxFSM::onRxComplete() {
  // We should never reach the full read of rx buffer.
  // If we get here, this means, there are no marker
  // chars in the stream, so we are lost
  errorCounter++;
  state = STATE_LOST;
  restartRX();
}

void FramingRxFSM::onCharacterMatch() {
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

void FramingRxFSM::onRxError(RxError_t e) {
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

uint32_t FramingRxFSM::receivedBytesCount() {
  return (RX_BYTES_MAX - dmaUART.getRxBytesLeft());
}

void FramingRxFSM::restartRX() {
  dmaUART.stopRX();
  dmaUART.startRX(rx_buf, RX_BYTES_MAX, RX_TIMEOUT, this);
}

void FramingRxFSM::processReceivedData() {
  out_buf_length = receivedBytesCount() - 1;
  memcpy(out_buf, rx_buf, out_buf_length);
  isNewOutBufReady = true;
}

uint8_t *FramingRxFSM::getReceivedBuf() {
  isNewOutBufReady = false;
  return out_buf;
}

uint32_t FramingRxFSM::getReceivedLength() { return out_buf_length; }

bool FramingRxFSM::isDataAvailable() { return isNewOutBufReady; }
