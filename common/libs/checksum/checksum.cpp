/* Copyright 2020, Edwin Chiu

  This file is part of FixMoreLungs.

  FixMoreLungs is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FixMoreLungs is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FixMoreLungs.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdint.h>

#include "checksum.h"

uint16_t checksum_fletcher16(const char *data, uint8_t count,
                             uint16_t state /*=0*/) {
  uint8_t s1 = state & 0xff;
  uint8_t s2 = (state >> 8) & 0xff;
  for (uint8_t index = 0; index < count; ++index) {
    s1 = (uint16_t{s1} + static_cast<uint16_t>(data[index])) % 255;
    s2 = (uint16_t{s2} + uint16_t{s1}) % 255;
  }
  return (uint16_t{s2} << 8) | s1;
}
