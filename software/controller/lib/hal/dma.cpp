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

DmaReg* const get_register(const Base id) {
  switch (id) {
    case Base::DMA1:
      return reinterpret_cast<DmaReg*>(0x40020000);
    case Base::DMA2:
      return reinterpret_cast<DmaReg*>(0x40020400);
  }
  // All cases covered above (and GCC checks this).
  __builtin_unreachable();
}

void SelectChannel(Base dma_id, Channel chan, int selection) {
  DmaReg* dma = get_register(dma_id);

  selection &= 0x0F;

  int x = 4 * static_cast<int>(chan);  // 4 bits per channel ([RM] 11.3.2)

  uint32_t val = dma->channel_select.full_reg;
  val &= ~(0xF << x);
  val |= selection << x;
  dma->channel_select.full_reg = val;
}

bool IntStatus(Base dma_id, Channel chan, Interrupt interrupt) {
  DmaReg* dma = get_register(dma_id);

  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(chan);  // 4 events per channel ([RM] 11.6.2)
  return (x & dma->interrupt_status.full_reg) > 0;
}

void ClearInt(Base dma_id, Channel chan, Interrupt interrupt) {
  DmaReg* dma = get_register(dma_id);

  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(chan);  // 4 events per channel ([RM] 11.6.2)
  dma->interrupt_clear.full_reg = x;
}

}  // namespace DMA
