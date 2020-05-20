#ifndef __FRAMING_RX_FSM
#define __FRAMING_RX_FSM

#include "network_protocol.pb.h"
#include "uart_dma.h"
template <class Transport> class FramingRxFSM : public UART_DMA_RxListener {
  enum State_t { STATE_LOST, STATE_WAIT_START, STATE_RX_FRAME };

private:
  Transport transport;
  State_t state = STATE_LOST;
  uint32_t errorCounter = 0;
  bool isNewOutBufReady = false;
  static constexpr uint32_t RX_BUF_LEN = (GuiStatus_size + 4) * 2 + 2;
  uint8_t out_buf[RX_BUF_LEN];
  uint32_t out_buf_length = 0;

public:
  FramingRxFSM(Transport t) : transport(t){};
  void begin();
  void onRxComplete() override;
  void onCharacterMatch() override;
  void onRxError(RxError_t e) override;
  uint8_t *getReceivedBuf();
  uint32_t getReceivedLength();
  bool isDataAvailable();
#ifdef TEST_MODE
  void test_PutRxBuffer(uint8_t *buf, uint32_t len);
#endif
protected:
  // virtual void restartRX() = 0;
  // virtual uint32_t receivedBytesCount() = 0;

private:
  void processReceivedData();
};

template <class Transport> void FramingRxFSM<Transport>::begin() {
  state = STATE_LOST;
  transport.begin(this);
}

template <class Transport> void FramingRxFSM<Transport>::onRxComplete() {
  // We should never reach the full read of rx buffer.
  // If we get here, this means, there are no marker
  // chars in the stream, so we are lost
  errorCounter++;
  state = STATE_LOST;
  transport.restartRX(this);
}

template <class Transport> void FramingRxFSM<Transport>::onCharacterMatch() {
  switch (state) {
  case STATE_LOST:
    // if we have received something before this marker,
    // we assume, this is the frame end marker, so wait
    // for start
    if (transport.receivedBytesCount() > 1) {
      state = STATE_WAIT_START;
      // if we were lucky to get lost in the interframe silence,
      // assume this is the start of the frame
    } else if (transport.receivedBytesCount() == 1) {
      state = STATE_RX_FRAME;
    } else {
      // Should never end up here
      // DMA is not working?
    }
    break;
  case STATE_WAIT_START:
    if (transport.receivedBytesCount() == 1) {
      state = STATE_RX_FRAME;
    } else {
      // some junk received during start wait,
      // but should have been just silence
      errorCounter++;
      state = STATE_LOST;
    }
    break;
  case STATE_RX_FRAME:
    if (transport.receivedBytesCount() > 1) {
      processReceivedData();
      state = STATE_WAIT_START;
    } else {
      // repeated marker char received
    }
    break;
  }
  transport.restartRX(this);
}

template <class Transport>
void FramingRxFSM<Transport>::onRxError(RxError_t e) {
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

template <class Transport> void FramingRxFSM<Transport>::processReceivedData() {
  // we strip markers from the stream, but that does not influence the frame
  // decoder code
  out_buf_length = transport.receivedBytesCount() - 1;
  memcpy(out_buf, transport.get_rx_buf(), out_buf_length);
  isNewOutBufReady = true;
}

template <class Transport> uint8_t *FramingRxFSM<Transport>::getReceivedBuf() {
  isNewOutBufReady = false;
  return out_buf;
}

template <class Transport>
uint32_t FramingRxFSM<Transport>::getReceivedLength() {
  return out_buf_length;
}

template <class Transport> bool FramingRxFSM<Transport>::isDataAvailable() {
  return isNewOutBufReady;
}

#endif
