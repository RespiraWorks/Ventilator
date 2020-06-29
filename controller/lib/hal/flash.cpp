/* Copyright 2020, RespiraWorks

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

/*
 * Erase a single 2k page of flash given the starting
 * address of that page.
 *
 * Returns true on success or false on failure
 */
bool HalApi::FlashErasePage(uint32_t addr) {

  // Check for a reasonable address
  if (addr < flash_start_addr)
    return false;

  if (addr >= flash_start_addr + flash_size)
    return false;

  // Address needs to be the start of a page
  if (addr & (flash_page_size - 1))
    return false;

  // Clear all the status bits
  FlashReg *reg = FLASH_BASE;
  reg->status = 0x0000C3FB;

  // Unlock flash by writing two special values to the key register
  reg->key = 0x45670123;
  reg->key = 0xCDEF89AB;

  // Find the page number
  uint8_t n = static_cast<uint8_t>((addr - flash_start_addr) / flash_page_size);

  reg->ctrl.page = n;
  reg->ctrl.page_erase = 1;
  reg->ctrl.start = 1;

  // Wait for the busy bit to clear
  while (reg->status & 0x00010000) {
  }

  // Lock the flash again
  reg->ctrl.page = 0;
  reg->ctrl.page_erase = 0;
  reg->ctrl.lock = 1;
  return true;
}

inline uint32_t u8_to_u32(uint8_t *ptr) {
  uint32_t a = *ptr++;
  uint32_t b = *ptr++;
  uint32_t c = *ptr++;
  uint32_t d = *ptr++;
  return a | (b << 8) | (c << 16) | (d << 24);
}

// Write ct bytes to flash starting at the given address.
// ct must be a multiple of 8 because flash is programmed 64-bits
// at a time.  If ct isn't a multiple of 8 this function will fail.
// The address must also be a multiple of 8 for the same reason.
bool HalApi::FlashWrite(uint32_t addr, void *data, int ct) {
  // Make sure the address is the start of a flash page
  if ((addr < flash_start_addr) ||
      ((addr + ct) > flash_start_addr + flash_size))
    return false;

  if ((ct < 0) || (ct & 7) || (addr & 7))
    return false;

  FlashReg *reg = FLASH_BASE;

  // Clear all the status bits
  reg->status = 0x0000C3FB;

  // Unlock flash
  reg->key = 0x45670123;
  reg->key = 0xCDEF89AB;

  // Set the PG bit to start programming
  reg->ctrl.program = 1;

  uint8_t *ptr = (uint8_t *)data;

  uint32_t *dest = (uint32_t *)addr;
  for (int i = 0; i < ct / 8; i++) {
    // Write 64-bits
    *dest++ = u8_to_u32(ptr);
    ptr += 4;

    *dest++ = u8_to_u32(ptr);
    ptr += 4;

    // Wait for busy bit to clear
    while (reg->status & 0x00010000) {
    }

    // Check fo rerrors
    if (reg->status & 0x0000C3FA)
      break;

    // Clear EOP
    reg->status = 0x00000001;
  }

  // Lock flash
  reg->ctrl.program = 0;
  reg->ctrl.lock = 1;

  if (reg->status & 0x0000C3FA)
    return false;
  return true;
}

#endif
