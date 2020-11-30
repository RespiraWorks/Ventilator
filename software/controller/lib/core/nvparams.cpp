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
// These parameters are stored in a dedicated 256 kbits flash memory.
//
// On system startup we read through the flash to initialize the parameters.
//
// A checksum and flip/flop system ensure the integrity of the parameters: write
// operations write the data to one side of the flip/flop, update the checksum,
// (which validates that side), and then update the data on the other side (but
// not its checksum, which renders it invalid) in preparation of the next write.

#include "nvparams.h"
#include "checksum.h"
#include "eeprom.h"
#include "hal.h"
#include "vars.h"
#include <string.h>

// Size of the parameter block including the header
static constexpr uint32_t nvparam_size = sizeof(NVparams);

#if defined(BARE_STM32)
NVParams nv_params;

// local functions
static bool ReadFullParams(NVParamsAddress address, void *param);
static void WriteFullParams(NVParamsAddress address, void *param);
static uint32_t ParamsCRC(void *param);
static bool IsValid(NVparams *param) { return param->crc == ParamsCRC(param); };

static DebugUInt32
    dbg_reinit("NV Params reinit",
               "Set to 1 to request a reinit of NV params on next boot", 0);

// One time init of non-volatile parameter area.
// This must not be done when a watchdog is enabled, as it blocks
// execution while it reads through the I2C EEPROM.
void NVParams::Init() {
  // Read flip side
  if (ReadFullParams(NVParamsAddress::kFlip, &nv_param_)) {
    // check its validity
    if (IsValid(&nv_param_)) {
      // Still read the flop in case they are both valid (which normally
      // shouldn't happen but could if power is lost at just the right time when
      // writing).
      NVparams Flop;
      ReadFullParams(NVParamsAddress::kFlop, &Flop);
      // check its validity
      if (IsValid(&Flop)) {
        // Check the counter and keep Flop if it is the most recent one
        if (Flop.count > nv_param_.count ||
            (Flop.count == 0 && nv_param_.count == 0xFF)) {
          nv_param_ = Flop;
          nvparam_addr_ = NVParamsAddress::kFlop;
        }
      }
    } else { // flip is invalid ==> check the flop
      ReadFullParams(NVParamsAddress::kFlop, &nv_param_);
      if (IsValid(&nv_param_)) {
        nvparam_addr_ = NVParamsAddress::kFlop;
      } else {
        // none of the flip/flop is valid
        // TODO: this should only happen during the very first use of a
        // ventilator, maybe we should take action? (alarm?)
        nv_param_.reinit = 1;
      }
    }
  } else {
    // timeout while reading the flip side --> reinit nv_param
    nv_param_ = NVparams();
    linked_ = false;
  }
  if (nv_param_.reinit == 1) {
    // Write the correct structure with init values to both sides in case
    // a reinit is needed (debug-user request or no valid params found)
    nv_param_ = NVparams();
    nv_param_.crc = ParamsCRC(&nv_param_);
    WriteFullParams(NVParamsAddress::kFlip, &nv_param_);
    WriteFullParams(NVParamsAddress::kFlop, &nv_param_);
  }
}

bool NVParams::Set(uint16_t offset, const void *value, uint8_t len) {
  // Make sure the passed pointer is pointing to somewhere
  // in the structure and isn't in the reserved first 3 bytes
  if ((offset < 3) || ((offset + len) > nvparam_size))
    return false;

  // Update the contents in nv_params
  memcpy(reinterpret_cast<uint8_t *>(&nv_param_) + offset, value, len);

  nv_param_.count++;
  nv_param_.crc = ParamsCRC(&nv_param_);

  if (linked_) {
    // Update the contents in eeprom (with flip/flop logic)
    uint16_t base_address = static_cast<uint16_t>(NVParamsAddress::kFlip);
    if (nvparam_addr_ == NVParamsAddress::kFlip) {
      base_address = static_cast<uint16_t>(NVParamsAddress::kFlop);
    }
    // write the changed data and both crc+counter to the new side
    eeprom.WriteBytes(static_cast<uint16_t>(base_address + offset), len, value,
                      nullptr);
    eeprom.WriteBytes(base_address, 2, &nv_param_, nullptr);

    // only write the changed data to the old side in preparation of the next
    // operation. This makes it invalid as a desired side effect.
    eeprom.WriteBytes(
        static_cast<uint16_t>(static_cast<uint16_t>(nvparam_addr_) + offset),
        len, value, nullptr);
    // point nvparam_address to the newly-written side
    nvparam_addr_ = static_cast<NVParamsAddress>(base_address);
  }
  return true;
}

bool NVParams::Get(uint16_t offset, void *value, uint8_t len) {
  // Make sure the passed pointer is pointing to somewhere
  // in the structure and isn't in the reserved first 3 bytes
  if ((offset < 3) || ((offset + len) > nvparam_size))
    return false;
  memcpy(value, (&nv_param_) + offset, len);
  return true;
}

// call this function in order to write debugvars to nv_params
void NVParams::DebugHandler() {
  uint8_t var = static_cast<uint8_t>(dbg_reinit.Get());
  if (var != nv_param_.reinit) {
    Set(offsetof(NVparams, reinit), &var, 1);
  }
}

// Calculate the CRC of the params at this address
static uint32_t ParamsCRC(void *param) {
  char *ptr = reinterpret_cast<char *>(param);
  return soft_crc32(ptr + sizeof(uint32_t), nvparam_size - sizeof(uint32_t));
}

// This function must not be called when a watchdog is looking as it blocks
// the execution while it reads the EEPROM.
static bool ReadFullParams(NVParamsAddress address, void *param) {
  bool read_finished{false};
  eeprom.ReadBytes(static_cast<uint16_t>(address), nvparam_size, param,
                   &read_finished);
  Time start_time = Hal.now();
  // Wait until the read is performed, or at most 500 ms: reading 4kB should
  // take under 100 ms if the 400 kHz I²C bus is used at 100% capacity.
  // If this takes longer, it most likely means our EEPROM is irresponsive (or
  // absent...) and we will most likely clog the request queue after a while.
  while (!read_finished) {
    if (Hal.now() < start_time + milliseconds(500)) {
      // maybe we should have an alarm in case our EEPROM is irresponsive?
      return false;
    }
  }
  return true;
}

static void WriteFullParams(NVParamsAddress address, void *param) {
  eeprom.WriteBytes(static_cast<uint16_t>(address), nvparam_size, param,
                    nullptr);
  // for a write, we don't really need to wait until the request is processed:
  // the I2C queue should ensure nothing is lost.
  return;
}

// Fake NVparams implementation for testing purposes
#else

NVParams fakeParams;

void NVParams::Init() {}

bool NVParams::Set(uint16_t offset, const void *value, uint8_t len) {
  if ((offset < 3) || ((offset + len) > nvparam_size))
    return false;

  // Update the contents in RAM
  memcpy(reinterpret_cast<uint8_t *>(&nv_param_) + offset, value, len);

  return true;
}

bool NVParams::Get(uint16_t offset, void *value, uint8_t len) {
  if ((offset < 3) || ((offset + len) > nvparam_size))
    return false;

  // Update the contents in RAM
  memcpy(value, reinterpret_cast<uint8_t *>(&nv_param_) + offset, len);

  return true;
}

#endif
