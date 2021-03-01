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

#ifndef BINUTILS_H
#define BINUTILS_H

#include <stdint.h>
// Some simple data conversion functions.
// These assume little endian byte format
inline uint16_t u8_to_u16(const uint8_t *dat) {
  uint16_t L = dat[0];
  uint16_t H = dat[1];
  return static_cast<uint16_t>(L | (H << 8));
}

inline uint32_t u8_to_u32(const uint8_t *dat) {
  uint32_t A = dat[0];
  uint32_t B = dat[1];
  uint32_t C = dat[2];
  uint32_t D = dat[3];
  return static_cast<uint32_t>(A | (B << 8) | (C << 16) | (D << 24));
}

inline void u16_to_u8(uint16_t val, uint8_t *buff) {
  buff[0] = static_cast<uint8_t>(val);
  buff[1] = static_cast<uint8_t>(val >> 8);
}

inline void u32_to_u8(uint32_t val, uint8_t *buff) {
  buff[0] = static_cast<uint8_t>(val);
  buff[1] = static_cast<uint8_t>(val >> 8);
  buff[2] = static_cast<uint8_t>(val >> 16);
  buff[3] = static_cast<uint8_t>(val >> 24);
}

#endif // BINUTILS_H
