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

#include "binary_utils.h"

#include "gtest/gtest.h"

TEST(Binutils, Conversion_32bits) {
  uint32_t test_values[10] = {0,          0x12345678, 0xABCDEF01, 0x98765432,
                              0xFFFFFFFF, 0x55AA00FF, 0xAAFF5500, 0xAF0550FA};
  for (auto value : test_values) {
    uint8_t converted[4] = {0};
    u32_to_u8(value, converted);
    for (size_t i = 0; i < 4; ++i) {
      EXPECT_EQ(converted[i], (value & (0xFF << (8 * i))) >> (8 * i));
    }
    EXPECT_EQ(value, u8_to_u32(converted));
  }
}

TEST(Binutils, Conversion_16bits) {
  uint16_t test_values[10] = {0, 0x1234, 0xABCD, 0x7654, 0xFFFF, 0x55AA, 0xFF55, 0xAF05};
  for (auto value : test_values) {
    uint8_t converted[2] = {0};
    u16_to_u8(value, converted);
    for (size_t i = 0; i < 2; ++i) {
      EXPECT_EQ(converted[i], (value & (0xFF << (8 * i))) >> (8 * i));
    }
    EXPECT_EQ(value, u8_to_u16(converted));
  }
}
