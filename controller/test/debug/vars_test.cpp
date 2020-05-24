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

#include "vars.h"
#include "gtest/gtest.h"
#include <stdint.h>

TEST(DebugVar, DebugVarInt32) {
  int32_t value = 5;
  DebugVar var(
      "var", "help", "fmt", [&] { return value; },
      [&](int32_t v) { value = v; });
  EXPECT_EQ(5, var.Get());
  EXPECT_EQ(5, var.as_uint32());

  var.Set(7);
  EXPECT_EQ(7, var.Get());
  EXPECT_EQ(7, var.as_uint32());

  var.set_uint32(42);
  EXPECT_EQ(42, var.Get());
  EXPECT_EQ(42, var.as_uint32());
}

TEST(DebugVar, DebugVarFloatRoundtrip) {
  float value = 42.37f;
  DebugVar var(
      "var", "help", "fmt", [&] { return value; }, [&](float v) { value = v; });

  EXPECT_EQ(42.37f, var.Get());

  var.Set(var.Get());
  EXPECT_EQ(42.37f, var.Get());

  var.set_uint32(var.as_uint32());
  EXPECT_EQ(42.37f, var.Get());

  var.set_uint32(0xDEADBEEF);
  EXPECT_EQ(0xDEADBEEF, var.as_uint32());

  float new_value = var.Get();
  var.Set(new_value);
  EXPECT_EQ(new_value, var.Get());
  EXPECT_EQ(0xDEADBEEF, var.as_uint32());
}

TEST(DebugVar, Registration) {
  int32_t int_value = 5;
  DebugVar var1(
      "var1", "help1", "fmt1", [&] { return int_value; },
      [&](int32_t v) { int_value = v; });

  float float_value = 7;
  DebugVar var2(
      "var2", "help2", "fmt2", [&] { return float_value; },
      [&](float v) { float_value = v; });

  auto *untyped_var1 = DebugVarRegistry::GetVar(var1.id());
  EXPECT_EQ("var1", untyped_var1->name());
  EXPECT_EQ("help1", untyped_var1->help());
  EXPECT_EQ("fmt1", untyped_var1->fmt());
  EXPECT_EQ(VarType::INT32, untyped_var1->type());
  EXPECT_EQ(var1.id(), untyped_var1->id());

  auto *untyped_var2 = DebugVarRegistry::GetVar(var2.id());
  EXPECT_EQ("var2", untyped_var2->name());
  EXPECT_EQ("help2", untyped_var2->help());
  EXPECT_EQ("fmt2", untyped_var2->fmt());
  EXPECT_EQ(VarType::FLOAT, untyped_var2->type());
  EXPECT_EQ(var2.id(), untyped_var2->id());
}
