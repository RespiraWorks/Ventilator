#ifndef __FRAMING_RX_FSM
#define __FRAMING_RX_FSM

#include "network_protocol.pb.h"
#include "uart_dma.h"

class FramingRxFSM : public UART_DMA_RxListener {
  enum State_t { STATE_LOST, STATE_WAIT_START, STATE_RX_FRAME };

  UART_DMA &uart_dma;

  static constexpr uint32_t RX_TIMEOUT = 115200 * 10;

  State_t state = STATE_LOST;
  uint32_t errorCounter = 0;
  // Size of the buffer is set asuming a corner case where EVERY GuiStatus
  // byte and CRC32 will be escaped + two marker chars; this is too big, but
  // safe.
  static constexpr uint32_t RX_BUF_LEN = (GuiStatus_size + 4) * 2 + 2;
  static constexpr uint32_t RX_BYTES_MAX = RX_BUF_LEN;
  uint8_t rx_buf[RX_BUF_LEN];
  uint8_t out_buf[RX_BUF_LEN];
  uint32_t out_buf_length = 0;
  bool isNewOutBufReady = false;

public:
  FramingRxFSM(UART_DMA &uart_dma) : uart_dma(uart_dma){};
  void begin();
  void onRxComplete() override;
  void onCharacterMatch() override;
  void onRxError(RxError_t e) override;
  uint8_t *getReceivedBuf();
  uint32_t getReceivedLength();
  bool isDataAvailable();

private:
  uint32_t receivedBytesCount();
  void restartRX();
  void processReceivedData();
};
#endif
