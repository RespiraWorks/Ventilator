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

#ifndef NVPARAM_H_
#define NVPARAM_H_

#include <stddef.h>
#include <stdint.h>

// This structure defines the layout of the non-volatile
// parameter info stored in flash.
// The structure should be exactly 512 bytes long.
struct NVparams {
  // Header info used to keep track of parameter info
  uint32_t crc;  // 32-bit CRC of remaining structure
  uint8_t count; // Incremented on each write.
  uint8_t mark;  // Used to corrupt old blocks
  uint16_t info; // For future use, just zero for now

  // Non-volatile parameters should be added here
  uint32_t rsvd[126];
};

static_assert(sizeof(NVparams) == 512);

// prototypes
void NVparamsInit(void);
const NVparams *FindStore(void);
bool NVparamsUpdtOff(uint32_t offset, const void *value, uint8_t len);

// Convenience macro to update a member of the non-volatile
// parameter structure given it's name
#define NVparamsUpdt(member, value, len)                                       \
  NVparamsUpdtOff(offsetof(NVparams, member), value, len)

// Update a 32-bit non-volatile parameter
#define NVparamsUpdt32(member, value)                                          \
  NVparamsUpdtOff(offsetof(NVparams, member), value, sizeof(uint32_t))

#endif
