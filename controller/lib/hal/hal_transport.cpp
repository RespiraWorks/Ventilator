#include "hal_transport.h"

#include "uart_dma.h"

void HalTransport::begin(UART_DMA_RxListener *rxl) {
  uart_dma.charMatchEnable();
  uart_dma.startRX(rx_buf, RX_BYTES_MAX, RX_TIMEOUT, rxl);
}

void HalTransport::restartRX(UART_DMA_RxListener *rxl) {
  uart_dma.stopRX();
  uart_dma.startRX(rx_buf, RX_BYTES_MAX, RX_TIMEOUT, rxl);
}

uint32_t HalTransport::receivedBytesCount() {
  return (RX_BYTES_MAX - uart_dma.getRxBytesLeft());
}
