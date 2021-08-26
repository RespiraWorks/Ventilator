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

#include <stdint.h>

#include <array>

#include "commands.h"
#include "gtest/gtest.h"
#include "vars.h"

namespace Debug::Command {

TEST(VarHandler, GetVarInfo) {
  uint32_t value = 0xDEADBEEF;
  const char *name = "name";
  const char *help = "help string";
  const char *format = "format";
  const char *unit = "unit";
  Debug::Variable::Primitive32 var(name, Debug::Variable::Access::ReadOnly, &value, unit, help,
                                   format);

  // expected result is hand-built from format given in var_cmd.cpp
  std::vector<uint8_t> expected = {static_cast<uint8_t>(Debug::Variable::Type::UInt32),
                                   static_cast<uint8_t>(Debug::Variable::Access::ReadOnly),
                                   0,
                                   0,
                                   static_cast<uint8_t>(strlen(name)),
                                   static_cast<uint8_t>(strlen(format)),
                                   static_cast<uint8_t>(strlen(help)),
                                   static_cast<uint8_t>(strlen(unit))};
  for (size_t i = 0; i < strlen(name); ++i) expected.push_back(name[i]);
  for (size_t i = 0; i < strlen(format); ++i) expected.push_back(format[i]);
  for (size_t i = 0; i < strlen(help); ++i) expected.push_back(help[i]);
  for (size_t i = 0; i < strlen(unit); ++i) expected.push_back(unit[i]);

  uint8_t id[2];
  u16_to_u8(var.id(), id);
  std::array req = {
      static_cast<uint8_t>(VarHandler::Subcommand::GetInfo), id[0],
      id[1],  // Var id
  };
  std::array<uint8_t, 50> response;
  bool processed{false};
  Context context = {.request = req.data(),
                     .request_length = std::size(req),
                     .response = response.data(),
                     .max_response_length = std::size(response),
                     .response_length = 0,
                     .processed = &processed};

  EXPECT_EQ(ErrorCode::None, VarHandler().Process(&context));
  EXPECT_TRUE(processed);
  for (size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(context.response[i], expected[i]);
  }
}

TEST(VarHandler, GetVar) {
  uint32_t value = 0xDEADBEEF;
  Debug::Variable::Primitive32 var("name", Debug::Variable::Access::ReadWrite, &value, "units",
                                   "help");

  // Test that a GET command obtains the variable's value.
  uint8_t id[2];
  u16_to_u8(var.id(), id);
  std::array req = {
      static_cast<uint8_t>(VarHandler::Subcommand::Get), id[0],
      id[1],  // Var id
  };
  std::array<uint8_t, 4> response;
  bool processed{false};
  Context context = {.request = req.data(),
                     .request_length = std::size(req),
                     .response = response.data(),
                     .max_response_length = std::size(response),
                     .response_length = 0,
                     .processed = &processed};

  EXPECT_EQ(ErrorCode::None, VarHandler().Process(&context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(4, context.response_length);

  std::array<uint8_t, 4> expected_result;
  u32_to_u8(value, expected_result.data());
  EXPECT_EQ(response, expected_result);
}

TEST(VarHandler, SetVar) {
  uint32_t value = 0xDEADBEEF;
  Debug::Variable::Primitive32 var("name", Debug::Variable::Access::ReadWrite, &value, "units",
                                   "help");

  uint32_t new_value = 0xCAFEBABE;
  std::array<uint8_t, 4> new_bytes;
  u32_to_u8(new_value, new_bytes.data());

  // Test that a SET command changes the variable's value.
  uint8_t id[2];
  u16_to_u8(var.id(), id);
  std::array req = {
      static_cast<uint8_t>(VarHandler::Subcommand::Set),
      id[0],
      id[1],  // Var id
      new_bytes[0],
      new_bytes[1],
      new_bytes[2],
      new_bytes[3]  // Value
  };

  std::array<uint8_t, 0> response;
  bool processed{false};
  Context context = {.request = req.data(),
                     .request_length = std::size(req),
                     .response = response.data(),
                     .max_response_length = std::size(response),
                     .response_length = 0,
                     .processed = &processed};

  EXPECT_EQ(ErrorCode::None, VarHandler().Process(&context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(0, context.response_length);

  EXPECT_EQ(new_value, value);
}

TEST(VarHandler, GetVarCount) {
  uint32_t value = 0xDEADBEEF;
  Debug::Variable::Primitive32 dummy("name", Debug::Variable::Access::ReadWrite, &value, "units");

  // Test that GetVarCount command obtains the number of defined variables
  std::array req = {static_cast<uint8_t>(VarHandler::Subcommand::GetCount)};
  std::array<uint8_t, 4> response;
  bool processed{false};
  Context context = {.request = req.data(),
                     .request_length = std::size(req),
                     .response = response.data(),
                     .max_response_length = std::size(response),
                     .response_length = 0,
                     .processed = &processed};

  EXPECT_EQ(ErrorCode::None, VarHandler().Process(&context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(4, context.response_length);

  std::array<uint8_t, 4> expected_result;
  u32_to_u8(Debug::Variable::Registry::singleton().count(), expected_result.data());
  EXPECT_EQ(response, expected_result);
}

TEST(VarHandler, Errors) {
  uint32_t value = 0xDEADBEEF;
  Debug::Variable::UInt32 var("name", Debug::Variable::Access::ReadWrite, value, "units", "help");
  uint8_t id[2];
  u16_to_u8(var.id(), id);
  Debug::Variable::UInt32 var_readonly("name", Debug::Variable::Access::ReadOnly, value, "units",
                                       "help");
  uint8_t id_readonly[2];
  u16_to_u8(var_readonly.id(), id_readonly);

  std::vector<std::tuple<std::vector<uint8_t>, ErrorCode>> requests = {
      {{}, ErrorCode::MissingData},   // Missing subcommand
      {{4}, ErrorCode::InvalidData},  // Invalid subcommand
      {{0, 0xFF, 0xFF}, ErrorCode::UnknownVariable},
      {{1, 0xFF, 0xFF}, ErrorCode::UnknownVariable},
      {{2, 0xFF, 0xFF}, ErrorCode::UnknownVariable},
      {{0, 1}, ErrorCode::MissingData},
      {{1, 1}, ErrorCode::MissingData},
      {{2, 1}, ErrorCode::MissingData},
      {{0, id[0], id[1]}, ErrorCode::NoMemory},
      {{1, id[0], id[1]}, ErrorCode::NoMemory},
      {{3}, ErrorCode::NoMemory},
      {{2, id[0], id[1], 0xCA, 0xFE, 0x00}, ErrorCode::MissingData},
      {{2, id_readonly[0], id_readonly[1], 0xCA, 0xFE, 0x00, 0x00}, ErrorCode::InternalError},
  };

  for (auto &[request, error] : requests) {
    // response size 3 to provoke No Memory error once all other checks are OK
    std::array<uint8_t, 3> response;
    bool processed{false};
    Context context = {.request = request.data(),
                       .request_length = static_cast<uint32_t>(request.size()),
                       .response = response.data(),
                       .max_response_length = response.size(),
                       .response_length = 0,
                       .processed = &processed};
    EXPECT_EQ(error, VarHandler().Process(&context));
    EXPECT_FALSE(processed);
    EXPECT_EQ(context.response_length, 0);
  }
}

}  // namespace Debug::Command
