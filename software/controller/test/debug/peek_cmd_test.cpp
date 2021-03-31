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

TEST(PeekHandler, valid_peek) {
  static constexpr size_t kMemorySize{50};
  static constexpr size_t kResponseSize{20};
  PeekHandler peek_handler;
  uint8_t memory[kMemorySize];
  for (size_t byte_number = 0; byte_number < kMemorySize; ++byte_number) {
    memory[byte_number] = static_cast<uint8_t>(rand());
  }
  for (uint16_t peek_length = 1; peek_length <= kResponseSize; ++peek_length) {
    std::array<uint8_t, 6> peek_command = {0, 0, 0, 0, 0, 0};
    // set command length
    u16_to_u8(peek_length, peek_command.data() + sizeof(uint32_t));

    for (size_t byte_number = 0; byte_number < kMemorySize + 1 - peek_length;
         ++byte_number) {
      size_t address = reinterpret_cast<size_t>(&memory[byte_number]);
      // feed address MSW to the handler, as we only pass 32bits addresses
      // through the command protocol
      peek_handler.SetAddressMSW(address & 0xFFFFFFFF00000000);
      // set command address
      u32_to_u8(address & 0x00000000FFFFFFFF, peek_command.data());

      // format response to kResponseSize - 1 in order to hit the max response
      // size limit when peek length is kResponseSize
      std::array<uint8_t, kResponseSize - 1> response;

      bool processed{false};
      Context peek_context = {.request = peek_command.data(),
                              .request_length = std::size(peek_command),
                              .response = response.data(),
                              .max_response_length = std::size(response),
                              .response_length = 0,
                              .processed = &processed};
      EXPECT_EQ(ErrorCode::kNone, peek_handler.Process(&peek_context));
      EXPECT_TRUE(processed);
      EXPECT_EQ(
          peek_context.response_length,
          std::min(peek_length, static_cast<uint16_t>(std::size(response))));
      for (size_t offset = 0; offset < peek_context.response_length; ++offset) {
        EXPECT_EQ(response[offset], memory[byte_number + offset]);
      }
    }
  }
}

TEST(PeekHandler, errors) {
  std::array<uint8_t, 5> peek_command = {0, 0, 0, 0, 0};
  std::array<uint8_t, 40> response;

  bool processed{false};
  Context peek_context = {.request = peek_command.data(),
                          .request_length = std::size(peek_command),
                          .response = response.data(),
                          .max_response_length = std::size(response),
                          .response_length = 0,
                          .processed = &processed};
  EXPECT_EQ(ErrorCode::kMissingData, PeekHandler().Process(&peek_context));
  EXPECT_FALSE(processed);
}

} // namespace Debug::Command
