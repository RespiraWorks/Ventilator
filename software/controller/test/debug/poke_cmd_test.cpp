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

TEST(PokeHandler, valid_poke) {
  static constexpr size_t kMemorySize{50};
  static constexpr size_t kMaxPokeSize{20};
  PokeHandler poke_handler;
  uint8_t memory[kMemorySize] = {0};

  for (uint16_t poke_length = 1; poke_length <= kMaxPokeSize; ++poke_length) {
    std::array<uint8_t, kMaxPokeSize + 4> poke_command = {0};

    // set data to be written
    for (size_t byte_number = 0; byte_number < poke_length; ++byte_number) {
      poke_command[byte_number + 4] = static_cast<uint8_t>(rand());
    }

    // write this data at all possible positions using poke command
    for (size_t byte_number = 0; byte_number < kMemorySize + 1 - poke_length;
         ++byte_number) {
      size_t address = reinterpret_cast<size_t>(&memory[byte_number]);
      // feed address MSW to the handler, as we only pass 32bits addresses
      // through the command protocol
      poke_handler.SetAddressMSW(address & 0xFFFFFFFF00000000);
      // set command address
      u32_to_u8(address & 0x00000000FFFFFFFF, poke_command.data());

      std::array<uint8_t, 1> response;
      bool processed{false};
      Context poke_context = {.request = poke_command.data(),
                              .request_length =
                                  static_cast<uint32_t>(poke_length + 4),
                              .response = response.data(),
                              .max_response_length = std::size(response),
                              .response_length = 0,
                              .processed = &processed};
      EXPECT_EQ(ErrorCode::kNone, poke_handler.Process(&poke_context));
      EXPECT_TRUE(processed);
      EXPECT_EQ(poke_context.response_length, 0);
      for (size_t offset = 0; offset < poke_length; ++offset) {
        EXPECT_EQ(poke_command[offset + 4], memory[byte_number + offset]);
      }
    }
  }
}

TEST(PokeHandler, errors) {
  std::array<uint8_t, 4> poke_command = {0, 0, 0, 0};
  std::array<uint8_t, 40> response;
  bool processed{false};
  Context poke_context = {.request = poke_command.data(),
                          .request_length = std::size(poke_command),
                          .response = response.data(),
                          .max_response_length = std::size(response),
                          .response_length = 0,
                          .processed = &processed};
  EXPECT_EQ(ErrorCode::kMissingData, PokeHandler().Process(&poke_context));
  EXPECT_FALSE(processed);
}
} // namespace Debug::Command
