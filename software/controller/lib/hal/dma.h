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

#include <cstdint>

/// \TODO: move this to cpp
struct DmaStruct {
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
typedef volatile DmaStruct DmaReg;

namespace DMA {

enum class Base {
  DMA1,
  DMA2,
};

// [RM] 11.4.4 DMA channels (pg 302)
enum class Channel : uint8_t {
  Chan1 = 0,
  Chan2 = 1,
  Chan3 = 2,
  Chan4 = 3,
  Chan5 = 4,
  Chan6 = 5,
  Chan7 = 6,
};

enum class ChannelDir { PeripheralToMemory = 0, MemoryToPeripheral = 1 };
enum class TransferSize { Byte = 0, HalfWord = 1, Word = 2 };
enum class Interrupt { Global = 1, TransferComplete = 2, HalfTransfer = 4, TransferError = 8 };

DmaReg *const get_register(Base id);

void SelectChannel(Base dma_id, Channel chan, int selection);

// Detect a DMA Interrupt
bool IntStatus(Base dma_id, Channel chan, Interrupt interrupt);

// Clear a DMA Interrupt
void ClearInt(Base dma_id, Channel chan, Interrupt interrupt);

}  // namespace DMA
