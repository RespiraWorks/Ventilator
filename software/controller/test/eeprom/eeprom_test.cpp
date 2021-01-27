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

#include "eeprom.h"
#include "gtest/gtest.h"

static constexpr uint32_t kMemSize{32768};
// initialize memory with data = address % 256
uint8_t memory[kMemSize];

static I2Ceeprom eeprom = I2Ceeprom(0x50, 64, 32768, &i2c1);

// This test check that the read address is built correctly
TEST(I2C_EEPROM, Read_Write) {

  for (int i = 0; i < kMemSize; ++i) {
    memory[i] = static_cast<uint8_t>(i % 256);
  }

  bool processed{false};
  // Write full memory
  eeprom.WriteBytes(0, kMemSize, &memory, &processed);
  ASSERT_TRUE(processed);

  // Check the memory content
  uint8_t eeprom_memory[kMemSize];
  processed = false;
  eeprom.ReadBytes(0, kMemSize, &eeprom_memory, &processed);
  ASSERT_TRUE(processed);

  for (int i = 0; i < kMemSize; ++i) {
    ASSERT_EQ(eeprom_memory[i], static_cast<uint8_t>(i % 256));
  }

  // Write accross pages, not starting on a page start.
  // This checks the current_offset and length are correctly computed
  for (int i = 20; i < 220; ++i) {
    memory[i] = static_cast<uint8_t>(i + 25 % 256);
  }
  eeprom.WriteBytes(20, 200, &memory[20], nullptr);

  eeprom.ReadBytes(0, kMemSize, &eeprom_memory, nullptr);

  for (int i = 0; i < kMemSize; ++i) {
    ASSERT_EQ(eeprom_memory[i], memory[i]);
  }

  // Random read
  for (int i = 20; i < 220; ++i) {
    memory[i] = static_cast<uint8_t>(i + 50 % 256);
  }
  eeprom.WriteBytes(0, kMemSize, &memory, nullptr);

  eeprom.ReadBytes(20, 200, &eeprom_memory[20], nullptr);

  for (int i = 0; i < kMemSize; ++i) {
    ASSERT_EQ(eeprom_memory[i], memory[i]);
  }
}

TEST(I2C_EEPROM, InvalidAccess) {
  ASSERT_TRUE(eeprom.ReadBytes(0, kMemSize, &memory, nullptr));
  ASSERT_TRUE(eeprom.WriteBytes(0, kMemSize, &memory, nullptr));
  ASSERT_FALSE(eeprom.ReadBytes(0, kMemSize + 1, &memory, nullptr));
  ASSERT_FALSE(eeprom.WriteBytes(0, kMemSize + 1, &memory, nullptr));
  ASSERT_TRUE(eeprom.ReadBytes(kMemSize - 1, 1, &memory, nullptr));
  ASSERT_TRUE(eeprom.WriteBytes(kMemSize - 1, 1, &memory, nullptr));
  ASSERT_FALSE(eeprom.ReadBytes(kMemSize, 1, &memory, nullptr));
  ASSERT_FALSE(eeprom.WriteBytes(kMemSize, 1, &memory, nullptr));
}
