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
#include "hal.h"
#include "vars.h"
#include <string.h>

namespace NVParams {
// Size of the parameter block including the header
static constexpr uint32_t size{sizeof(Structure)};

// Calculate the CRC of the params at this address
static uint32_t CRC(Structure *param) {
  char *ptr = reinterpret_cast<char *>(param);
  return soft_crc32(ptr + sizeof(uint32_t), size - sizeof(uint32_t));
}

// Checks whether a param is valid (through its checksum)
static bool IsValid(Structure *param) { return param->crc == CRC(param); };

} // namespace NVParams

static DebugUInt32
    dbg_reinit("nvparams_reinit",
               "Set to 1 to request a reinit of NV params on next boot.", 0);
static DebugUInt32 dbg_serial("serial_number",
                              "Serial number of the ventilator, in EEPROM", 0);

static DebugUInt32 dbg_nvparams("nvparams_address", "Address of nv_params", 0);

// One time init of non-volatile parameter area.
// This must not be done when a watchdog is enabled, as it blocks
// execution while it reads through the I2C EEPROM.
void NVParams::Handler::Init(I2Ceeprom *eeprom) {
#ifndef TEST_MODE // this leads to conversion error when compiling on native
  dbg_nvparams.Set(reinterpret_cast<uint32_t>(&nv_param_));
#endif
  if (eeprom != nullptr) {
    eeprom_ = eeprom;
    linked_to_eeprom_ = true;
  }
  // Read flip side
  if (ReadFullParams(Address::kFlip, &nv_param_, eeprom_)) {
    nvparam_addr_ = Address::kFlip;
    // check its validity
    if (IsValid(&nv_param_)) {
      // Still read the flop in case they are both valid (which normally
      // shouldn't happen but could if power is lost at just the right time when
      // writing).
      Structure Flop;
      ReadFullParams(Address::kFlop, &Flop, eeprom_);
      // check its validity
      if (IsValid(&Flop)) {
        // Check the counter and keep Flop if it is the most recent one
        if (Flop.count > nv_param_.count ||
            (Flop.count == 0 && nv_param_.count == 0xFF)) {
          nv_param_ = Flop;
          nvparam_addr_ = Address::kFlop;
        }
      }
    } else { // flip is invalid ==> check the flop
      ReadFullParams(Address::kFlop, &nv_param_, eeprom_);
      if (IsValid(&nv_param_)) {
        nvparam_addr_ = Address::kFlop;
      } else {
        // none of the flip/flop is valid
        // TODO: this should only happen during the very first use of a
        // ventilator, maybe we should take action? (alarm?)
        nv_param_.reinit = 1;
      }
    }
  } else {
    // timeout while reading the flip side --> reinit nv_param and disable
    // sending write requests when updating values (eeprom or I2C failure)
    // TODO: this should not happen --> take action (alarm?)
    nv_param_.reinit = 1;
    linked_to_eeprom_ = false;
  }
  if (nv_param_.reinit == 1) {
    // Write the correct structure with init values to both sides in case
    // a reinit is needed (debug-user request or no valid params found)
    nv_param_ = Structure();
    nv_param_.crc = CRC(&nv_param_);
    if (linked_to_eeprom_) {
      WriteFullParams(Address::kFlip);
      WriteFullParams(Address::kFlop);
    }
  }
  // set write access dbg_vars = nv_params to prevent the first pass in
  // handler to reset those to their default values in nv_params
  dbg_reinit.Set(nv_param_.reinit);
  dbg_serial.Set(nv_param_.vent_serial_number);
  // increase power cycles counter in nv_params
  uint32_t counter = nv_param_.power_cycles + 1;
  Set(offsetof(Structure, power_cycles), &counter, 4);
}

bool NVParams::Handler::Set(uint16_t offset, void *value, uint8_t len) {
  // Make sure the passed pointer is pointing to somewhere
  // in the structure and isn't in the reserved first 6 bytes
  if ((offset < 6) || ((offset + len) > size))
    return false;

  // Update the contents in nv_params
  memcpy(reinterpret_cast<uint8_t *>(&nv_param_) + offset, value, len);

  nv_param_.count++;
  nv_param_.crc = CRC(&nv_param_);

  if (linked_to_eeprom_) {
    // Update the contents in eeprom (with flip/flop logic)
    uint16_t new_address{static_cast<uint16_t>(Address::kFlip)};
    if (nvparam_addr_ == Address::kFlip) {
      new_address = static_cast<uint16_t>(Address::kFlop);
    }
    // write the changed data and both crc+counter to the new side
    eeprom_->WriteBytes(static_cast<uint16_t>(new_address + offset), len, value,
                        nullptr);
    eeprom_->WriteBytes(new_address, 5, &nv_param_, nullptr);

    // write the changed data to the old side in preparation of the next
    // operation. This makes it invalid as a desired side effect.
    eeprom_->WriteBytes(
        static_cast<uint16_t>(static_cast<uint16_t>(nvparam_addr_) + offset),
        len, value, nullptr);
    // point nvparam_address to the newly-written side
    if (nvparam_addr_ == Address::kFlip) {
      nvparam_addr_ = Address::kFlop;
    } else {
      nvparam_addr_ = Address::kFlip;
    }
  }
  return true;
}

bool NVParams::Handler::Get(uint16_t offset, void *value, uint8_t len) {
#ifndef TEST_MODE // in test mode I need to be able to access any byte
  // Make sure the passed pointer is pointing to somewhere
  // in the structure and isn't in the reserved first 6 bytes
  if ((offset < 6) || ((offset + len) > size))
    return false;
#endif
  memcpy(value, reinterpret_cast<uint8_t *>(&nv_param_) + offset, len);
  return true;
}

// call this function in order to write variables to nv_params
void NVParams::Handler::Update(const Time now, VentParams *params) {
  // We only update cumulated service every so often
  if (now >= last_update_ + kUpdateInterval) {
    uint32_t cumulated = nv_param_.cumulated_service +
                         static_cast<uint32_t>((now - last_update_).seconds());
    Set(offsetof(Structure, cumulated_service), &cumulated, 4);
    last_update_ = now;
  }

  // assert that the size of VentParams is still good - if it isn't, we need to
  // update the condition below to capture all members
  static_assert(sizeof(VentParams) == 32);

  if (params->mode != nv_param_.last_settings.mode ||
      params->peep_cm_h2o != nv_param_.last_settings.peep_cm_h2o ||
      params->pip_cm_h2o != nv_param_.last_settings.pip_cm_h2o ||
      params->breaths_per_min != nv_param_.last_settings.breaths_per_min ||
      params->fio2 != nv_param_.last_settings.fio2 ||
      params->inspiratory_expiratory_ratio !=
          nv_param_.last_settings.inspiratory_expiratory_ratio ||
      params->inspiratory_trigger_cm_h2o !=
          nv_param_.last_settings.inspiratory_trigger_cm_h2o ||
      params->expiratory_trigger_ml_per_min !=
          nv_param_.last_settings.expiratory_trigger_ml_per_min) {
    Set(offsetof(Structure, last_settings), params, sizeof(VentParams));
  }

  // Update from debug variables
  uint8_t reinit = static_cast<uint8_t>(dbg_reinit.Get());
  if (reinit != nv_param_.reinit) {
    Set(offsetof(Structure, reinit), &reinit, 1);
  }
  uint32_t serial = dbg_serial.Get();
  if (serial != nv_param_.vent_serial_number) {
    Set(offsetof(Structure, vent_serial_number), &serial, 4);
  }
}

// This method must not be called when a watchdog is looking as it blocks
// the execution while it reads the EEPROM.
bool NVParams::Handler::ReadFullParams(Address address, Structure *param,
                                       I2Ceeprom *eeprom) {
  bool read_finished{false};
  eeprom->ReadBytes(static_cast<uint16_t>(address), size, param,
                    &read_finished);
  Time start_time = Hal.now();
  // Wait until the read is performed, or at most 500 ms: reading 4kB should
  // take under 100 ms if the 400 kHz I²C bus is used at 100% capacity.
  // If this takes longer, it most likely means our EEPROM is irresponsive (or
  // absent...).
  while (!read_finished) {
    if (Hal.now() > start_time + milliseconds(500)) {
      // maybe we should have an alarm in case our EEPROM is irresponsive?
      return false;
    }
  }
  return true;
}

void NVParams::Handler::WriteFullParams(Address address) {
  eeprom_->WriteBytes(static_cast<uint16_t>(address), size, &nv_param_,
                      nullptr);
  // for a write, we don't really need to wait until the request is processed:
  // the I2C queue should ensure nothing is lost.
}
