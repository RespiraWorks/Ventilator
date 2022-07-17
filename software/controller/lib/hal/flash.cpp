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

#include "flash.h"

#include "clocks_stm32.h"

// \TODO these consts should be maintained by caller
// Flash memory location & size info
static constexpr uint32_t FlashStartAddr{0x08000000};
static constexpr size_t FlashSize{32 * 1024};
static constexpr size_t FlashPageSize{2 * 1024};

// [RM] 3.7 Flash Registers (pg 100)
struct FlashStruct {
  // Access control register
  struct {
    uint32_t latency : 3;  // Number of wait states
    uint32_t reserved1 : 5;
    uint32_t prefecth_enable : 1;
    uint32_t instruction_cache_enable : 1;
    uint32_t data_cache_enable : 1;
    uint32_t instruction_cache_reset : 1;  // Instruction cache reset
    uint32_t data_cache_reset : 1;         // Data cache reset
    uint32_t run_powerdown : 1;            // Power-down during run or low power mode
    uint32_t sleep_powerdown : 1;          // Power-down during sleep mode
    uint32_t reserved2 : 17;
  } access;

  uint32_t powerdown_key;
  uint32_t key;
  uint32_t option_key;
  uint32_t status;

  // 0x14 - Control Register (FLASH_CR)
  struct {
    uint32_t program : 1;
    uint32_t page_erase : 1;
    uint32_t mass_erase : 1;
    uint32_t page : 8;
    uint32_t reserved1 : 5;
    uint32_t start_operation : 1;
    uint32_t opt_start : 1;
    uint32_t fast_programming : 1;
    uint32_t reserved2 : 5;
    uint32_t end_op_interrupt : 1;
    uint32_t error_interrupt : 1;
    uint32_t read_error_interrupt : 1;
    uint32_t force_byte_reloading : 1;
    uint32_t reserved3 : 2;
    uint32_t options_lock : 1;
    uint32_t flash_lock : 1;
  } control;

  uint32_t ecc;
  uint32_t reserved1;
  uint32_t option;
  uint32_t pcrop_start_address;
  uint32_t pcrop_end_address;
  uint32_t wrp_area_a;
  uint32_t wrp_area_b;
};

typedef volatile FlashStruct FlashReg;
inline FlashReg* const FlashBase = reinterpret_cast<FlashReg*>(0x40022000);

/* Write ct bytes to flash starting at the given address.
 * ct must be a multiple of 8 because flash is programmed 64-bits
 * at a time. The address must be a multiple of 8 for the same reason.
 * [RM] 3.3.7
 */
bool ValidFlashParameters(uint32_t addr, size_t ct) {
  return (addr >= FlashStartAddr) and (addr + ct <= FlashStartAddr + FlashSize) and !(ct & 7) and
         !(addr & 7);
}

/* After reset, write is not allowed in the Flash control register (FLASH_CR)
 * to protect the Flash memory against possible unwanted operations due,
 * for example, to electric disturbances. [RM] 3.3.5
 * Any wrong sequence will lock up the FLASH_CR register until the next system
 * reset. In the case of a wrong key sequence, a bus error is detected and a
 * Hard Fault interrupt is generated
 */
void UnlockFlash(FlashReg* reg) {
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
void LockFlash(FlashReg* reg) { reg->control.flash_lock = 1; }

namespace Flash {

void initialize() {
  // Reset caches and set latency for 80MHz operation
  // See chapter 3 of [RM] for details on the embedded flash module
  Clocks::enable_peripheral_clock(PeripheralID::Flash);
  FlashReg* flash = FlashBase;

  // Set four wait states (required to run at 80MHz)
  flash->access.latency = 4;

  // Reset the instruction and data caches
  flash->access.instruction_cache_reset = 1;
  flash->access.data_cache_reset = 1;
  flash->access.instruction_cache_reset = 0;
  flash->access.data_cache_reset = 0;

  // Enable the caches
  flash->access.instruction_cache_enable = 0;
  flash->access.data_cache_enable = 0;
}

/*
 * Erase a single 2k page of flash given the starting
 * address of that page.
 *
 * Returns true on success or false on failure
 */
bool erase_page(uint32_t addr) {
  if (!ValidFlashParameters(addr, FlashPageSize)) {
    return false;
  }

  // Clear all the status bits
  FlashReg* reg = FlashBase;
  reg->status = 0x0000C3FB;

  UnlockFlash(reg);

  // Find the page number
  uint8_t n = static_cast<uint8_t>((addr - FlashStartAddr) / FlashPageSize);

  reg->control.page = n;
  reg->control.page_erase = 1;
  reg->control.start_operation = 1;

  // Wait for the busy bit to clear
  while (reg->status & 0x00010000) {
  }

  reg->control.page = 0;
  reg->control.page_erase = 0;

  LockFlash(reg);
  return true;
}

bool write(uint32_t addr, void* data, size_t ct) {
  if (!ValidFlashParameters(addr, ct)) {
    return false;
  }
  FlashReg* reg = FlashBase;

  // Clear all the status bits
  reg->status = 0x0000C3FB;

  UnlockFlash(reg);

  // Set the PG bit to start programming
  reg->control.program = 1;

  auto* dest = reinterpret_cast<uint32_t*>(addr);
  auto* ptr = reinterpret_cast<uint32_t*>(data);
  for (int i = 0; i < ct / 8; i++) {
    // Write 64-bits
    *dest++ = *ptr++;
    *dest++ = *ptr++;

    // Wait for busy bit to clear
    while (reg->status & 0x00010000) {
    }

    // Check for errors
    if (reg->status & 0x0000C3FA) {
      break;
    }

    // Clear EOP
    reg->status = 0x00000001;
  }

  reg->control.program = 0;

  LockFlash(reg);
  return !(reg->status & 0x0000C3FA);
}

}  // namespace Flash
