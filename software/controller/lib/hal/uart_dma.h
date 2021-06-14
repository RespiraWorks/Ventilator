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

#ifndef UART_DMA_H
#define UART_DMA_H
#include "hal_stm32_regs.h"

enum class RxError {
  RxUnknownError,
  RxOverflow,
  RxFramingError,
  RxTimeout,
  RxDmaError
};

// An interface that gets called back by the driver on rx, tx complete
// and rx character match events.
// NOTE: all callbacks are called from interrupt context!
class UartDmaRxListener {
public:
  // Called on DMA RX complete
  virtual void OnRxComplete() = 0;
  // Called on specified character reception
  virtual void OnCharacterMatch() = 0;
  // Called on RX errors
  virtual void OnRxError(RxError) = 0;
};

class UartDmaTxListener {
public:
  // Called on DMA TX complete
  virtual void OnTxComplete() = 0;
  // Called on TX errors
  virtual void OnTxError() = 0;
};

class DMACtrl {
public:
  explicit DMACtrl(DmaReg *const dma) : dma_(dma) {}
  void Init() {
    // UART3 reception happens on DMA1 channel 3
    dma_->channel_select.c3s = 0b0010;
    // UART3 transmission happens on DMA1 channel 2
    dma_->channel_select.c2s = 0b0010;
  }

private:
  DmaReg *const dma_;
};

class UartDma {
public:
  UartDma(UartReg *const uart, DmaReg *const dma, uint8_t tx_chan,
          uint8_t rx_chan, UartDmaRxListener *rxl, UartDmaTxListener *txl,
          char match_char)
      : uart_(uart), dma_(dma), tx_channel_(tx_chan), rx_channel_(rx_chan),
        rx_listener_(rxl), tx_listener_(txl), match_char_(match_char) {}

  void Init(int baud);
  // Returns true if DMA TX is in progress
  bool TxInProgress() const;
  // Returns true if DMA RX is in progress
  bool RxInProgress() const;

  // Sets up UART3 to transfer [length] characters from [buf]
  // Returns false if DMA transmission is in progress, does not
  // interrupt previous transmission.
  // Returns true if no transmission is in progress
  bool StartTX(char *buf, uint32_t length);

  uint32_t GetRxBytesLeft();

  void StopTX();

  // Sets up reception of at least [length] chars from UART3 into [buf]
  // [timeout] is the number of baudrate bits for which RX line is
  // allowed to be idle before asserting timeout error.
  // Returns false if reception is in progress, new reception is not
  // setup. Returns true if no reception is in progress and new reception
  // was setup.

  bool StartRX(char *buf, uint32_t length, uint32_t timeout);
  void StopRX();
  void CharMatchEnable();

  void UartISR();
  void DmaRxISR();
  void DmaTxISR();

private:
  UartReg *const uart_;
  DmaReg *const dma_;
  uint8_t tx_channel_;
  uint8_t rx_channel_;
  UartDmaRxListener *rx_listener_;
  UartDmaTxListener *tx_listener_;
  char match_char_;
  bool tx_in_progress_{false};
  bool rx_in_progress_{false};
};
#endif
