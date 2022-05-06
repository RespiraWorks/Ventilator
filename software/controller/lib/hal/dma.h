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

#include <cstddef>  // used for size_t definition
#include <cstdint>

#include "interrupts.h"

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
enum class ChannelPriority { Low = 0, Medium = 1, High = 2, Highest = 3 };

class ChannelControl {
 public:
  ChannelControl(Base base, Channel channel);
  void Initialize(uint8_t selection, volatile uint32_t *peripheral, ChannelDir dir,
                  bool tx_interrupt = false,
                  ChannelPriority channel_priority = ChannelPriority::Medium,
                  InterruptPriority interrupt_priority = InterruptPriority::Standard,
                  bool circular = false, TransferSize size = TransferSize::Byte);
  bool InterruptStatus(Interrupt interrupt);
  void ClearInterrupt(Interrupt interrupt);
  void SetupTransfer(volatile void *data, uint32_t length);
  void Enable();
  void Disable();
  size_t Remaining();

 private:
  void SelectChannel(uint8_t selection);
  void SetupChannel(ChannelPriority prio, TransferSize size, ChannelDir dir, bool tx_interrupt,
                    bool circular);

  Base base_;
  Channel channel_;
};

}  // namespace DMA
