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

#include "var_cmd.h"
#include "vars.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <array>
#include <stdint.h>

TEST(VarCmd, GetVar) {
  uint32_t value = 0xDEADBEEF;
  DebugVar var("name", &value, "help", "fmt");

  // Test that a GET command obtains the variable's value.
  uint8_t id[2];
  u16_to_u8(var.GetId(), id);
  std::array req = {
      uint8_t{1},   // GET
      id[0], id[1], // Var id
      uint8_t{0}    // Placeholder
  };
  std::array<uint8_t, 4> resp;
  CmdContext context = {.req = req.data(),
                        .req_len = std::size(req),
                        .resp = resp.data(),
                        .max_resp_len = std::size(resp),
                        .resp_len = 0};

  EXPECT_EQ(DbgErrCode::OK, VarCmd().HandleCmd(&context));
  EXPECT_EQ(4, context.resp_len);

  std::array<uint8_t, 4> expected_result;
  u32_to_u8(value, expected_result.data());
  EXPECT_EQ(resp, expected_result);
}

TEST(VarCmd, SetVar) {
  uint32_t value = 0xDEADBEEF;
  DebugVar var("name", &value, "help", "fmt");

  uint32_t new_value = 0xCAFEBABE;
  std::array<uint8_t, 4> new_bytes;
  u32_to_u8(new_value, new_bytes.data());

  // Test that a SET command changes the variable's value.
  uint8_t id[2];
  u16_to_u8(var.GetId(), id);
  std::array req = {
      uint8_t{2},                                            // SET
      id[0],        id[1],                                   // Var id
      new_bytes[0], new_bytes[1], new_bytes[2], new_bytes[3] // Value
  };

  std::array<uint8_t, 0> resp;
  CmdContext context = {.req = req.data(),
                        .req_len = std::size(req),
                        .resp = resp.data(),
                        .max_resp_len = std::size(resp),
                        .resp_len = 0};

  EXPECT_EQ(DbgErrCode::OK, VarCmd().HandleCmd(&context));
  EXPECT_EQ(0, context.resp_len);

  EXPECT_EQ(new_value, value);
}
