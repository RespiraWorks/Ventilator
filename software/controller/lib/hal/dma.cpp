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

#include "dma.h"

namespace DMA {
struct DmaRegisters {
  union {
    struct {
      uint32_t gif1 : 1;   // global interrupt flag
      uint32_t tcif1 : 1;  // transfer complete (TC) flag
      uint32_t htif1 : 1;  // half transfer (HT) flag
      uint32_t teif1 : 1;  // transfer error (TE) flag
      uint32_t gif2 : 1;   // global interrupt flag
      uint32_t tcif2 : 1;  // transfer complete (TC) flag
      uint32_t htif2 : 1;  // half transfer (HT) flag
      uint32_t teif2 : 1;  // transfer error (TE) flag
      uint32_t gif3 : 1;   // global interrupt flag
      uint32_t tcif3 : 1;  // transfer complete (TC) flag
      uint32_t htif3 : 1;  // half transfer (HT) flag
      uint32_t teif3 : 1;  // transfer error (TE) flag
      uint32_t gif4 : 1;   // global interrupt flag
      uint32_t tcif4 : 1;  // transfer complete (TC) flag
      uint32_t htif4 : 1;  // half transfer (HT) flag
      uint32_t teif4 : 1;  // transfer error (TE) flag
      uint32_t gif5 : 1;   // global interrupt flag
      uint32_t tcif5 : 1;  // transfer complete (TC) flag
      uint32_t htif5 : 1;  // half transfer (HT) flag
      uint32_t teif5 : 1;  // transfer error (TE) flag
      uint32_t gif6 : 1;   // global interrupt flag
      uint32_t tcif6 : 1;  // transfer complete (TC) flag
      uint32_t htif6 : 1;  // half transfer (HT) flag
      uint32_t teif6 : 1;  // transfer error (TE) flag
      uint32_t gif7 : 1;   // global interrupt flag
      uint32_t tcif7 : 1;  // transfer complete (TC) flag
      uint32_t htif7 : 1;  // half transfer (HT) flag
      uint32_t teif7 : 1;  // transfer error (TE) flag
      uint32_t reserved : 4;
    };
    uint32_t full_reg;
  } interrupt_status;  // Interrupt Status Register (DMA_ISR) [RM] 11.6.1 (pg
  // 308)

  union {
    struct {
      uint32_t gif1 : 1;   // global interrupt flag
      uint32_t tcif1 : 1;  // transfer complete (TC) flag
      uint32_t htif1 : 1;  // half transfer (HT) flag
      uint32_t teif1 : 1;  // transfer error (TE) flag
      uint32_t gif2 : 1;   // global interrupt flag
      uint32_t tcif2 : 1;  // transfer complete (TC) flag
      uint32_t htif2 : 1;  // half transfer (HT) flag
      uint32_t teif2 : 1;  // transfer error (TE) flag
      uint32_t gif3 : 1;   // global interrupt flag
      uint32_t tcif3 : 1;  // transfer complete (TC) flag
      uint32_t htif3 : 1;  // half transfer (HT) flag
      uint32_t teif3 : 1;  // transfer error (TE) flag
      uint32_t gif4 : 1;   // global interrupt flag
      uint32_t tcif4 : 1;  // transfer complete (TC) flag
      uint32_t htif4 : 1;  // half transfer (HT) flag
      uint32_t teif4 : 1;  // transfer error (TE) flag
      uint32_t gif5 : 1;   // global interrupt flag
      uint32_t tcif5 : 1;  // transfer complete (TC) flag
      uint32_t htif5 : 1;  // half transfer (HT) flag
      uint32_t teif5 : 1;  // transfer error (TE) flag
      uint32_t gif6 : 1;   // global interrupt flag
      uint32_t tcif6 : 1;  // transfer complete (TC) flag
      uint32_t htif6 : 1;  // half transfer (HT) flag
      uint32_t teif6 : 1;  // transfer error (TE) flag
      uint32_t gif7 : 1;   // global interrupt flag
      uint32_t tcif7 : 1;  // transfer complete (TC) flag
      uint32_t htif7 : 1;  // half transfer (HT) flag
      uint32_t teif7 : 1;  // transfer error (TE) flag
      uint32_t reserved : 4;
    };
    uint32_t full_reg;
  } interrupt_clear;  // Interrupt Flag Clear Register [RM] 11.6.2 (pg 311)
  struct ChannelRegs {
    struct {
      uint32_t enable : 1;
      uint32_t tx_complete_interrupt : 1;
      uint32_t half_tx_interrupt : 1;
      uint32_t tx_error_interrupt : 1;
      uint32_t direction : 1;
      uint32_t circular : 1;
      uint32_t peripheral_increment : 1;
      uint32_t memory_increment : 1;
      uint32_t peripheral_size : 2;
      uint32_t memory_size : 2;
      uint32_t priority : 2;
      uint32_t mem2mem : 1;
      uint32_t reserved : 17;
    } config;
    uint32_t count;
    volatile void *peripheral_address;
    volatile void *memory_address;
    uint32_t reserved;
  } channel[7];
  uint32_t reserved[5];
  union {
    struct {
      uint32_t c1s : 4;
      uint32_t c2s : 4;
      uint32_t c3s : 4;
      uint32_t c4s : 4;
      uint32_t c5s : 4;
      uint32_t c6s : 4;
      uint32_t c7s : 4;
      uint32_t reserved : 4;
    };
    uint32_t full_reg;
  } channel_select;  // Channel Selection Register [RM] 11.6.7 (pg 317)
};
typedef volatile DmaRegisters DmaReg;

// DMA base registers, per [RM] table 2 (p68)
DmaReg *get_register(const Base id) {
  switch (id) {
    case Base::DMA1:
      return reinterpret_cast<DmaReg *>(0x40020000);
    case Base::DMA2:
      return reinterpret_cast<DmaReg *>(0x40020400);
  }
  // All cases covered above (and GCC checks this).
  __builtin_unreachable();
}

volatile DmaReg::ChannelRegs *get_channel_reg(const Base id, const Channel channel) {
  auto *base_reg = get_register(id);
  return &base_reg->channel[static_cast<uint8_t>(channel)];
}

ChannelControl::ChannelControl(Base base, Channel channel) : base_(base), channel_(channel){};

void ChannelControl::Initialize(uint8_t selection, volatile uint32_t *peripheral, ChannelDir dir,
                                bool tx_interrupt, ChannelPriority prio, bool circular,
                                TransferSize size) {
  Disable();
  SelectChannel(selection);
  SetupChannel(prio, size, dir, tx_interrupt, circular);

  auto *channel_reg = get_channel_reg(base_, channel_);
  channel_reg->peripheral_address = peripheral;
}

bool ChannelControl::InterruptStatus(Interrupt interrupt) {
  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(channel_);  // 4 events per channel ([RM] 11.6.2)
  auto *dma_reg = get_register(base_);
  return (x & dma_reg->interrupt_status.full_reg) > 0;
};

void ChannelControl::ClearInterrupt(Interrupt interrupt) {
  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(channel_);  // 4 events per channel ([RM] 11.6.2)
  auto *dma_reg = get_register(base_);
  dma_reg->interrupt_clear.full_reg = x;
};

void ChannelControl::SetupTransfer(volatile void *data, uint32_t length) {
  auto *channel_reg = get_channel_reg(base_, channel_);
  channel_reg->memory_address = data;
  channel_reg->count = length;
}

void ChannelControl::Enable() {
  auto *channel_reg = get_channel_reg(base_, channel_);
  channel_reg->config.enable = 1;
}

void ChannelControl::Disable() {
  auto *channel_reg = get_channel_reg(base_, channel_);
  channel_reg->config.enable = 0;
}

size_t ChannelControl::Remaining() {
  auto *channel_reg = get_channel_reg(base_, channel_);
  return channel_reg->count;
}

void ChannelControl::SelectChannel(uint8_t selection) {
  selection &= 0x0F;

  int x = 4 * static_cast<int>(channel_);  // 4 bits per channel ([RM] 11.3.2)

  auto *dma_reg = get_register(base_);
  uint32_t val = dma_reg->channel_select.full_reg;

  val &= ~(0xF << x);
  val |= selection << x;

  dma_reg->channel_select.full_reg = val;
}

void ChannelControl::SetupChannel(ChannelPriority prio, TransferSize size, ChannelDir dir,
                                  bool tx_interrupt, bool circular) {
  auto *channel_reg = get_channel_reg(base_, channel_);
  channel_reg->config.priority = static_cast<uint32_t>(prio) & 0b11;
  channel_reg->config.tx_complete_interrupt = tx_interrupt;
  channel_reg->config.memory_size = static_cast<uint32_t>(size) & 0b11;
  channel_reg->config.peripheral_size = static_cast<uint32_t>(size) & 0b11;
  channel_reg->config.direction = static_cast<uint32_t>(dir) & 0b1;
  channel_reg->config.circular = circular;

  // those settings are the same for all DMA channels we use.
  channel_reg->config.half_tx_interrupt = 0;
  channel_reg->config.tx_error_interrupt = 1;
  channel_reg->config.mem2mem = 0;
  channel_reg->config.memory_increment = 1;
  channel_reg->config.peripheral_increment = 0;
}

}  // namespace DMA
