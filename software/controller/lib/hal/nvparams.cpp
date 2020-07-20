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

// This module handles non-volatile parameter storage
//
// These parameters are stored in a structure of fixed size
// in two pages of flash memory.
//
// On system startup we look through these two pages of flash to find the
// valid parameters.  There should only be one valid copy under normal
// conditions.  If multiple copies are found (which could happen if we lose
// power during an update for example), then the most recently written copy will
// be kept.
//
// When a parameter value is changed, the function NVparamsUpdt is called to
// update the values in flash.  This function first writes a new copy of
// the parameter structure to an erased area of flash.  Once that copy
// is written and validated it overwrites part of the old copy to invalidate
// it.  In this way there should always be at least one valid copy of the
// parameters in flash memory and normally only one.

#include "nvparams.h"
#include "checksum.h"
#include "flash.h"
#include "hal.h"
#include <string.h>

// Size of the parameter block including the header
static constexpr uint32_t nvparam_size = 512;

#ifdef BARE_STM32

// This is the value of the 'mark' field of a good structure
// in flash.  It's pretty arbitrary, anything but 0 or 0xff
static constexpr uint8_t good_mark = 0x55;

// Address of valid parameter block
static uint32_t nvparam_addr;

// local functions
static bool CheckStruct(uint32_t addr);
static uint32_t BlockCRC(void *blk);
static void Invalidate(uint32_t addr);
static bool SaveBlock(NVparams *blk, uint32_t addr);

// One time init of non-volatile parameter area.
void NVparamsInit(void) {

  // Total structures in two pages of flash
  int N = flash_params_size / nvparam_size;

  // Run through structures looking for a valid one.
  uint32_t addr = flash_params_start;
  for (int i = 0; i < N; i++, addr += nvparam_size) {
    // Check the structure at this address
    if (!CheckStruct(addr))
      continue;

    // If this is the first valid block we've found,
    // just keep track of the address
    if (!nvparam_addr) {
      nvparam_addr = addr;
      continue;
    }

    // I've found multiple valid blocks.  This normally shouldn't
    // happen but could if power is lost at just the right time
    // when updating flash.
    //
    // I'll check the counter to see which was is newer and keep
    // that address.  I'll also invalidate the older one.
    NVparams *a = reinterpret_cast<NVparams *>(nvparam_addr);
    NVparams *b = reinterpret_cast<NVparams *>(addr);

    int8_t diff = static_cast<int8_t>(b->count - a->count);
    if (diff > 0) {
      Invalidate(nvparam_addr);
      nvparam_addr = addr;
    } else
      Invalidate(addr);
  }

  // If I found a valid parameter area, I'm done
  if (nvparam_addr)
    return;

  // If no valid storage was found then I'll just create one with
  // zero values.
  NVparams blank;
  memset(&blank, 0, sizeof(blank));
  blank.mark = good_mark;
  blank.crc = BlockCRC(&blank);

  Hal.FlashErasePage(flash_params_start);
  Hal.FlashWrite(flash_params_start, &blank, sizeof(blank));
  nvparam_addr = flash_params_start;
}

const NVparams *GetNvParams(void) {
  return reinterpret_cast<const NVparams *>(nvparam_addr);
}

bool NVparamsUpdtOff(uint32_t offset, const void *value, uint8_t len) {

  // Make sure the passed pointer is pointing to somewhere
  // in the current page and isn't in the reserved first 8 bytes
  if ((offset < 8) || ((offset + len) > nvparam_size))
    return false;

  // Copy the current parameter block to RAM
  NVparams temp;
  memcpy(&temp, reinterpret_cast<void *>(nvparam_addr), nvparam_size);

  // Update the contents in RAM
  memcpy(reinterpret_cast<uint8_t *>(&temp) + offset, value, len);

  // Make sure the reserved area is kept at zero
  memset(&temp.rsvd, 0, sizeof(temp.rsvd));

  temp.count++;
  temp.mark = good_mark;
  temp.crc = BlockCRC(&temp);

  uint32_t addr = nvparam_addr + nvparam_size;

  bool ok = SaveBlock(&temp, addr);

  // That shouldn't really fail, but if it does for some reason
  // try storing in the first location of the other flash page.
  // That will cause an erase first, so is more likely to succeed
  if (!ok) {
    addr = flash_params_start;
    if (nvparam_addr < flash_params_start + flash_page_size)
      addr += flash_page_size;
    ok = SaveBlock(&temp, addr);
  }

  if (!ok)
    return false;

  // After successfully storing and validating the new parameters,
  // Invalidate the old ones
  Invalidate(nvparam_addr);
  nvparam_addr = addr;

  return true;
}

static bool SaveBlock(NVparams *blk, uint32_t addr) {
  // If the address is the start of a page, first erase the page
  if ((addr & (flash_page_size - 1)) == 0) {
    bool ok = Hal.FlashErasePage(addr);
    if (!ok)
      return false;
  }

  bool ok = Hal.FlashWrite(addr, blk, nvparam_size);
  if (!ok)
    return false;

  if (memcmp(reinterpret_cast<void *>(addr), blk, nvparam_size))
    return false;

  return true;
}

// Check the block at this address to see if it's valid.
// Returns true if it is, false if not
static bool CheckStruct(uint32_t addr) {
  NVparams *store = reinterpret_cast<NVparams *>(addr);

  // We set the mark to a fixed value if it's good.
  // That allows me to quickly identify bad blocks.
  // If the block is erased, the mark will be 0xFF
  // When I invalidate a block I set it to 0x00.
  // The good mark could be any other value picked
  // pretty much arbitrarily.
  if (store->mark != good_mark)
    return false;

  uint32_t crc = BlockCRC(store);
  return store->crc == crc;
}

// Calculate the CRC of the block at this address
static uint32_t BlockCRC(void *blk) {
  char *ptr = reinterpret_cast<char *>(blk);
  return soft_crc32(ptr + sizeof(uint32_t), nvparam_size - sizeof(uint32_t));
}

// Invalidate a block by zeroing out the first 64-bits
// which includes the CRC and mark.
// When flash memory is erased all bits are set to 1.
// You can program bits to 0, but can't change a 0 to a 1 without erasing
// the whole page.  I invalite the block by setting the beginning of the
// header (including the CRC and mark) to all zeros.
static void Invalidate(uint32_t addr) {
  uint32_t zero[2] = {0, 0};
  Hal.FlashWrite(addr, zero, sizeof(zero));
}

// Fake NVparams implementation for testing purposes
#else

static NVparams fakeParams;

void NVparamsInit(void) {}

const NVparams *GetNvParams(void) { return &fakeParams; }

bool NVparamsUpdtOff(uint32_t offset, const void *value, uint8_t len) {
  if ((offset < 8) || ((offset + len) > nvparam_size))
    return false;

  // Update the contents in RAM
  memcpy(reinterpret_cast<uint8_t *>(&fakeParams) + offset, value, len);

  // Make sure the reserved area is kept at zero
  memset(&fakeParams.rsvd, 0, sizeof(fakeParams.rsvd));
  return true;
}

#endif
