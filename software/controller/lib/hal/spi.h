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

#pragma once

// [RM] 40.6 SPI Registers (pg 1330)
struct SpiStruct {
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
typedef volatile SpiStruct SpiReg;
inline SpiReg *const Spi1Base = reinterpret_cast<SpiReg *>(0x40013000);
inline SpiReg *const Spi2Base = reinterpret_cast<SpiReg *>(0x40003800);
inline SpiReg *const Spi3Base = reinterpret_cast<SpiReg *>(0x40003C00);
