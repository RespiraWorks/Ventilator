/* Copyright 2022, RespiraWorks

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

#include "spi.h"

#include "clocks_stm32.h"

namespace SPI {

// [RM] 40.6 SPI Registers (pg 1330)
struct Register {
  struct {
    uint32_t clock_phase : 1;
    uint32_t clock_polarity : 1;
    uint32_t master : 1;
    uint32_t bitrate : 3;
    uint32_t enable : 1;
    uint32_t lsb_first : 1;
    uint32_t internal_slave_select : 1;
    uint32_t sw_slave_management : 1;
    uint32_t rx_only : 1;
    uint32_t crc_length : 1;
    uint32_t crc_next : 1;
    uint32_t crc_enable : 1;
    uint32_t bidir_output : 1;
    uint32_t bidir_mode : 1;
    uint32_t reserved : 16;
  } control_reg1;  // Control register 1 (SPIx_CR1) [RM] 40.6.1 (pg 1330)
  struct {
    uint32_t rx_dma : 1;
    uint32_t tx_dma : 1;
    uint32_t ss_output : 1;
    uint32_t nss_pulse : 1;
    uint32_t frame_format : 1;
    uint32_t error_interrupt : 1;
    uint32_t rx_not_empty_interrupt : 1;
    uint32_t tx_empty_interrupt : 1;
    uint32_t data_size : 4;
    uint32_t fifo_rx_threshold : 1;
    uint32_t last_dma_rx : 1;
    uint32_t last_dma_tx : 1;
    uint32_t reserved : 17;
  } control2;  // Control register 2 (SPIx_CR2) [RM] 40.6.2
  uint32_t status;
  uint32_t data;
  uint32_t crc_polynomial;
  uint32_t rx_crc;
  uint32_t tx_crc;
};
typedef volatile Register SpiReg;

// SPI base registers, per [RM] table 2 (p68)
SpiReg *get_register(const Base id) {
  switch (id) {
    case Base::SPI1:
      return reinterpret_cast<SpiReg *>(0x40013000);
    case Base::SPI2:
      return reinterpret_cast<SpiReg *>(0x40003800);
    case Base::SPI3:
      return reinterpret_cast<SpiReg *>(0x40003C00);
  }
  // All cases covered above (and GCC checks this).
  __builtin_unreachable();
}

Channel::Channel(Base spi, DMA::Base dma) : spi_(spi) {
  // DMA mapping for SPI (see [RM] p299)
  static struct {
    DMA::Base dma_base;
    Base spi_base;
    DMA::Channel tx_channel_id;
    DMA::Channel rx_channel_id;
    uint8_t request_number;
  } DmaMap[] = {
      {DMA::Base::DMA1, Base::SPI1, DMA::Channel::Chan3, DMA::Channel::Chan2, 1},
      {DMA::Base::DMA1, Base::SPI2, DMA::Channel::Chan5, DMA::Channel::Chan4, 1},
      {DMA::Base::DMA2, Base::SPI1, DMA::Channel::Chan4, DMA::Channel::Chan3, 4},
      {DMA::Base::DMA2, Base::SPI2, DMA::Channel::Chan2, DMA::Channel::Chan1, 3},
  };

  for (auto &map : DmaMap) {
    if (dma == map.dma_base && spi_ == map.spi_base) {
      tx_dma_.emplace(dma, map.tx_channel_id);
      rx_dma_.emplace(dma, map.rx_channel_id);
      dma_request_ = map.request_number;
      break;
    }
  }

};

void Channel::Initialize(GPIO::Port clock_port, uint8_t clock_pin, GPIO::Port miso_port,
                         uint8_t miso_pin, GPIO::Port mosi_port, uint8_t mosi_pin,
                         GPIO::Port chip_select_port, uint8_t chip_select_pin,
                         GPIO::Port reset_port, uint8_t reset_pin,
                         uint8_t word_size, uint8_t bitrate_scaler,
                         bool rx_interrupts_enabled, bool tx_interrupts_enabled,
                         RxListener *rx_listener, TxListener *tx_listener){
  // Enable the clock for the selected peripheral
  switch(spi_){
    case Base::SPI1:
      enable_peripheral_clock(PeripheralID::SPI1);
      break;
    case Base::SPI2:
      enable_peripheral_clock(PeripheralID::SPI2);
      break;
    case Base::SPI3:
      enable_peripheral_clock(PeripheralID::SPI3);
      break;
    default:
      // All cases covered above (and GCC checks this).
      __builtin_unreachable();
  }

  // Configure the CS and reset pins as outputs, pulled high.
  // I don't really use the reset pin, I just want it to be high so I don't reset the part
  // inadvertently.
  GPIO::DigitalOutputPin(reset_port, reset_pin, true);
  chip_select_.emplace(chip_select_port, chip_select_pin, true);

  // Assign the three SPI pins to the SPI peripheral, [DS] Table 17 (pg 76) shows that SPI always
  // uses AF5. We use the highest speed setting.
  GPIO::AlternatePin(clock_port, clock_pin, GPIO::AlternativeFunction::AF5, GPIO::PullType::None,
                     GPIO::OutSpeed::Smoking);
  GPIO::AlternatePin(miso_port, miso_pin, GPIO::AlternativeFunction::AF5);
  GPIO::AlternatePin(mosi_port, mosi_pin, GPIO::AlternativeFunction::AF5, GPIO::PullType::None,
                     GPIO::OutSpeed::Smoking);

  // Configure my SPI port to talk to the stepper
  SpiReg *spi = get_register(spi_);

  // Enable RXNE interrupts
  spi->control2.rx_dma = 1;             // Enable DMA on receive
  spi->control2.tx_dma = 1;             // Enable DMA on transmit
  
  spi->control2.fifo_rx_threshold = 1;  // Receive interrupt on every byte

  spi->control2.data_size = static_cast<uint8_t>(word_size - 1) & 0b1111;
  //register value to achieve that is 3) [RM p1331]
  spi->control_reg1.bitrate = bitrate_scaler & 0b111;

  // Configure for master mode, CPOL and CPHA both 0.
  spi->control_reg1.clock_phase = 1;           // Data is sampled on the rising edge of the clock
  spi->control_reg1.clock_polarity = 1;        // Clock line is high when not active
  spi->control_reg1.master = 1;                // We're acting as the master on the SPI bus
  spi->control_reg1.internal_slave_select = 1; // Enable software slave select
  spi->control_reg1.sw_slave_management = 1;   // Software slave select management

  spi->control_reg1.enable = 1;                // Enable the SPI module

  rx_dma_->Initialize(dma_request_, &spi->data, DMA::ChannelDir::PeripheralToMemory,
                      rx_interrupts_enabled, DMA::ChannelPriority::Low,
                      InterruptPriority::Standard);
  tx_dma_->Initialize(dma_request_, &spi->data, DMA::ChannelDir::MemoryToPeripheral,
                      tx_interrupts_enabled, DMA::ChannelPriority::Low,
                      InterruptPriority::Standard);

  rx_listener_=rx_listener;
  tx_listener_=tx_listener;
}

void Channel::SetupReception(uint8_t *receive_buffer, size_t length) {
  rx_dma_->Disable();
  rx_dma_->SetupTransfer(receive_buffer, length);
  rx_dma_->Enable();
}

void Channel::SendCommand(uint8_t *send_buffer, size_t length, bool clear_chip_select) {
  tx_dma_->Disable();

  tx_dma_->SetupTransfer(send_buffer, length);

  if(clear_chip_select){
    ClearChipSelect();
  }

  tx_dma_->Enable();
}

// DMA channel Interrupt Handlers, which clear interrupts and run adequate callbacks
void Channel::TxDMAInterruptHandler() {
  if (tx_dma_->InterruptStatus(DMA::Interrupt::TransferError)) {
    tx_dma_->ClearInterrupt(DMA::Interrupt::TransferError);
    if (tx_listener_) {
      tx_listener_->on_tx_error();
    }
  }
  if (tx_dma_->InterruptStatus(DMA::Interrupt::TransferComplete)) {
    tx_dma_->ClearInterrupt(DMA::Interrupt::TransferComplete);
    if (tx_listener_) {
      tx_listener_->on_tx_complete();
    }
  }
}

void Channel::RxDMAInterruptHandler() {
  if (rx_dma_->InterruptStatus(DMA::Interrupt::TransferError)) {
    rx_dma_->ClearInterrupt(DMA::Interrupt::TransferError);
    if (rx_listener_) {
      rx_listener_->on_rx_error(RxError::DMA);
    }
  };

  if (rx_dma_->InterruptStatus(DMA::Interrupt::TransferComplete)) {
    rx_dma_->ClearInterrupt(DMA::Interrupt::TransferComplete);
    if (rx_listener_) {
      rx_listener_->on_rx_complete();
    }
  }
}

} // namespace SPI
