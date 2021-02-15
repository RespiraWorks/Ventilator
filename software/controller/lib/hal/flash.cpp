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
#if defined(BARE_STM32)

#include "flash.h"
#include "hal.h"
#include "hal_stm32.h"

/* Write ct bytes to flash starting at the given address.
 * ct must be a multiple of 8 because flash is programmed 64-bits
 * at a time. The address must be a multiple of 8 for the same reason.
 * [RM] 3.3.7
 */
static bool ValidFlashParameters(uint32_t addr, size_t ct) {
  return (addr >= flash_start_addr) and
         (addr + ct <= flash_start_addr + flash_size) and !(ct & 7) and
         !(addr & 7);
}

/* After reset, write is not allowed in the Flash control register (FLASH_CR)
 * to protect the Flash memory against possible unwanted operations due,
 * for example, to electric disturbances. [RM] 3.3.5
 * Any wrong sequence will lock up the FLASH_CR register until the next system
 * reset. In the case of a wrong key sequence, a bus error is detected and a
 * Hard Fault interrupt is generated
 */
static inline void UnlockFlash(FlashReg *reg) {
  // Unlock flash by writing two special values to the key register
  reg->key = 0x45670123;
  reg->key = 0xCDEF89AB;
}

/*
 * The FLASH_CR register can be locked again by software by setting the
 * LOCK bit in the FLASH_CR register.
 * WARNING: The FLASH_CR register cannot be written when the BSY bit in the
 * Flash status register (FLASH_SR) is set. Any attempt to write to it with the
 * BSY bit set will cause the AHB bus to stall until the BSY bit is cleared.
 * [RM] 3.3.5
 */
static inline void LockFlash(FlashReg *reg) { reg->ctrl.lock = 1; }

/*
 * Erase a single 2k page of flash given the starting
 * address of that page.
 *
 * Returns true on success or false on failure
 */
bool HalApi::FlashErasePage(uint32_t addr) {
  if (!ValidFlashParameters(addr, flash_page_size))
    return false;

  // Clear all the status bits
  FlashReg *reg = FLASH_BASE;
  reg->status = 0x0000C3FB;

  UnlockFlash(reg);

  // Find the page number
  uint8_t n = static_cast<uint8_t>((addr - flash_start_addr) / flash_page_size);

  reg->ctrl.page = n;
  reg->ctrl.page_erase = 1;
  reg->ctrl.start = 1;

  // Wait for the busy bit to clear
  while (reg->status & 0x00010000) {
  }

  reg->ctrl.page = 0;
  reg->ctrl.page_erase = 0;

  LockFlash(reg);
  return true;
}

bool HalApi::FlashWrite(uint32_t addr, void *data, size_t ct) {

  if (!ValidFlashParameters(addr, ct))
    return false;
  FlashReg *reg = FLASH_BASE;

  // Clear all the status bits
  reg->status = 0x0000C3FB;

  UnlockFlash(reg);

  // Set the PG bit to start programming
  reg->ctrl.program = 1;

  auto *dest = (uint32_t *)addr;
  auto *ptr = (uint32_t *)data;
  for (int i = 0; i < ct / 8; i++) {
    // Write 64-bits
    *dest++ = *ptr++;
    *dest++ = *ptr++;

    // Wait for busy bit to clear
    while (reg->status & 0x00010000) {
    }

    // Check for errors
    if (reg->status & 0x0000C3FA)
      break;

    // Clear EOP
    reg->status = 0x00000001;
  }

  reg->ctrl.program = 0;

  LockFlash(reg);
  return !(reg->status & 0x0000C3FA);
}

#endif
