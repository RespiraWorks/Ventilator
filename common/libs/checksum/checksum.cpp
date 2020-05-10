/* Copyright 2020, Edwin Chiu

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

#include "checksum.h"
#include <stdint.h>

uint16_t checksum_fletcher16(const char *data, uint8_t count,
                             uint16_t state /*=0*/) {
  uint8_t s1 = state & 0xff;
  uint8_t s2 = (state >> 8) & 0xff;
  for (uint8_t index = 0; index < count; ++index) {
    s1 = (uint16_t{s1} + static_cast<uint16_t>(data[index])) % 255;
    s2 = (uint16_t{s2} + uint16_t{s1}) % 255;
  }
  return static_cast<uint16_t>(uint16_t{s2} << 8) | s1;
}
