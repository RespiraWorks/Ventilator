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

#include "commands.h"
#include "gtest/gtest.h"
#include <array>

namespace Debug::Command {

TEST(EepromHandler, ValidRead) {
  static constexpr size_t kMemorySize{50};
  static constexpr size_t kMaxRequestSize{20};

  TestEeprom test_eeprom(0x50, 64, kMemorySize);
  EepromHandler eeprom_handler(&test_eeprom);

  for (uint16_t byte = 0; byte < kMemorySize; ++byte) {
    uint8_t value{static_cast<uint8_t>(rand())};
    test_eeprom.WriteBytes(byte, static_cast<uint16_t>(1), &value, nullptr);
  }

  for (uint16_t read_length = 1; read_length <= kMaxRequestSize;
       ++read_length) {
    std::array<uint8_t, 6> read_command = {
        static_cast<uint8_t>(EepromHandler::Subcommand::kRead)};
    // set command length
    u16_to_u8(read_length, &read_command[3]);
    for (uint16_t offset = 0; offset < kMemorySize + 1 - read_length;
         ++offset) {
      // set command address
      u16_to_u8(offset, &read_command[1]);
      std::array<uint8_t, kMaxRequestSize> response;
      bool processed{false};
      Context read_context = {
          .request = read_command.data(),
          .request_length = static_cast<uint32_t>(std::size(read_command)),
          .response = response.data(),
          .max_response_length = std::size(response),
          .response_length = 0,
          .processed = &processed,
      };
      EXPECT_EQ(ErrorCode::kNone, eeprom_handler.Process(&read_context));
      EXPECT_EQ(read_context.response_length, read_length);
      EXPECT_TRUE(processed);

      std::array<uint8_t, kMaxRequestSize> mem_read{0};
      test_eeprom.ReadBytes(offset, read_length, &mem_read, nullptr);
      for (uint16_t byte = 0; byte < read_length; ++byte) {
        EXPECT_EQ(mem_read[byte], response[byte]);
      }
    }
  }
}

TEST(EepromHandler, ValidWrite) {
  static constexpr size_t kMemorySize{50};
  static constexpr size_t kMaxWriteSize{20};

  TestEeprom test_eeprom(0x50, 64, kMemorySize);
  EepromHandler eeprom_handler(&test_eeprom);

  for (uint16_t write_length = 1; write_length <= kMaxWriteSize;
       ++write_length) {
    std::array<uint8_t, kMaxWriteSize + 3> write_command = {
        static_cast<uint8_t>(EepromHandler::Subcommand::kWrite)};
    for (uint16_t offset = 0; offset < kMemorySize + 1 - write_length;
         ++offset) {
      // set command address
      u16_to_u8(offset, &write_command[1]);
      // set command data
      for (uint16_t byte = 0; byte < kMaxWriteSize; ++byte) {
        write_command[byte + 3] = static_cast<uint8_t>(rand());
      }

      std::array<uint8_t, 1> response;
      bool processed{false};
      Context write_context = {
          .request = write_command.data(),
          .request_length = static_cast<uint32_t>(write_length + 3),
          .response = response.data(),
          .max_response_length = std::size(response),
          .response_length = 0,
          .processed = &processed,
      };
      EXPECT_EQ(ErrorCode::kNone, eeprom_handler.Process(&write_context));
      EXPECT_EQ(write_context.response_length, 0);
      EXPECT_TRUE(processed);

      std::array<uint8_t, kMaxWriteSize> mem_read{0};
      test_eeprom.ReadBytes(offset, write_length, &mem_read, nullptr);
      for (uint16_t byte = 0; byte < write_length; ++byte) {
        EXPECT_EQ(mem_read[byte], write_command[byte + 3]);
      }
    }
  }
}

TEST(EepromHandler, Errors) {
  static constexpr size_t kMemorySize{50};
  TestEeprom test_eeprom(0x50, 64, kMemorySize);
  EepromHandler eeprom_handler(&test_eeprom);

  // build a write that is too long to be processed (size > kMaxWriteSize + 3)
  std::vector<uint8_t> long_write;
  long_write.resize(1028, 0);
  long_write[0] = 1;

  std::vector<std::tuple<std::vector<uint8_t>, ErrorCode>> requests = {
      {{}, ErrorCode::kMissingData},           // Missing subcommand
      {{0, 0, 0, 4, 0}, ErrorCode::kNoMemory}, // length > max response length
      {{0, 0, 0, 0}, ErrorCode::kMissingData}, // Read missing length
      {{1, 0, 0}, ErrorCode::kMissingData},    // Write missing data
      {{1, 0xFF, 0xFF, 0}, ErrorCode::kInternalError}, // Write outside eeprom
      {long_write, ErrorCode::kNoMemory},   // Write more than 1024 bytes
      {{2, 0, 0}, ErrorCode::kInvalidData}, // Invalid subcommand
  };

  for (auto &[request, error] : requests) {
    // response size 3 to provoke No Memory error once all other checks are OK
    std::array<uint8_t, 3> response;
    Context context = {.request = request.data(),
                       .request_length =
                           static_cast<uint32_t>(std::size(request)),
                       .response = response.data(),
                       .max_response_length = response.size(),
                       .response_length = 0};
    EXPECT_EQ(error, eeprom_handler.Process(&context));
    EXPECT_EQ(context.response_length, 0);
  }
}

} // namespace Debug::Command
