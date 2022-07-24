/* Copyright 2020-2022, RespiraWorks

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

// The STM32 chip comes with 3 SPI channels, which can be used to exchange
// data with peripherals such as our stepper motors.
//
// Current implementation does not provide any bufferization, which is done
// by the user.
// Also, this only works with DMA, if DMA::Base provided in constructor cannot
// be mapped to the desired SPI::Base, the transfers will not work.

#include <optional>

#include "dma.h"
#include "gpio_stm32.h"
#include "serial_listeners.h"

namespace SPI {

enum class Base { SPI1, SPI2, SPI3 };

// possible bitrates on SPI bus (see [RM] p1131).
enum class Bitrate : uint8_t {
  CpuFreqByTwo = 0,
  CpuFreqByFour = 1,
  CpuFreqByEight = 2,
  CpuFreqBySixteen = 3,
  CpuFreqByThirtyTwo = 4,
  CpuFreqBySixtyFour = 5,
  CpuFreqByOneTwentyEight = 6,
  CpuFreqByTwoFiftySix = 7,
};

class Channel {
 public:
  Channel(Base spi, DMA::Base dma);

  void Initialize(GPIO::Port clock_port, uint8_t clock_pin, GPIO::Port miso_port, uint8_t miso_pin,
                  GPIO::Port mosi_port, uint8_t mosi_pin, GPIO::Port chip_select_port,
                  uint8_t chip_select_pin, GPIO::Port reset_port, uint8_t reset_pin,
                  uint8_t word_size, Bitrate bitrate, bool rx_interrupts_enabled,
                  bool tx_interrupts_enabled, RxListener *rx_listener = nullptr,
                  TxListener *tx_listener = nullptr);

  virtual void SetChipSelect() { chip_select_->set(); }
  virtual void ClearChipSelect() { chip_select_->clear(); }

  virtual void SetupReception(uint8_t *receive_buffer, size_t length);
  virtual void SendCommand(uint8_t *send_buffer, size_t length, bool clear_chip_select);

  // Interrupt handlers for DMA transfer complete
  void TxDMAInterruptHandler();
  virtual void RxDMAInterruptHandler();

 protected:
  // The way we setup SPI uses DMA to control the data flow
  std::optional<DMA::ChannelControl> rx_dma_{std::nullopt};
  std::optional<DMA::ChannelControl> tx_dma_{std::nullopt};

  // listeners for DMA transfer complete
  RxListener *rx_listener_{nullptr};
  TxListener *tx_listener_{nullptr};

 private:
  // SPI base of this channel
  Base spi_;

  // request number to setup the DMA channels
  uint8_t dma_request_{0};

  // We need to keep reference of the chip_select pin (NSS) so the application
  // can manage communications properly
  std::optional<GPIO::DigitalOutputPin> chip_select_{std::nullopt};
};

}  // namespace SPI
