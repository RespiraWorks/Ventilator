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
#include <limits>
#include <stdint.h>

TEST(DebugVar, DebugVarInt32) {
  int32_t value = 5;
  DebugVar var("var", &value, "help", "fmt");
  EXPECT_EQ("var", var.GetName());
  EXPECT_EQ("help", var.GetHelp());
  EXPECT_EQ(VarType::INT32, var.GetType());
  EXPECT_EQ("fmt", var.GetFormat());
  EXPECT_EQ(&var, DebugVar::FindVar(var.GetId()));

  EXPECT_EQ(5, var.GetValue());
  var.SetValue(7);
  EXPECT_EQ(7, var.GetValue());
  EXPECT_EQ(7, value);

  // Test a value outside the range of int32_t.
  uint32_t max = std::numeric_limits<uint32_t>::max();
  var.SetValue(max);
  EXPECT_EQ(max, var.GetValue());
  EXPECT_EQ(static_cast<int32_t>(max), value);

  // All default arguments
  DebugVar var_default("var", &value);
  EXPECT_EQ("", var_default.GetHelp());
  EXPECT_EQ("%d", var_default.GetFormat());
}

TEST(DebugVar, DebugVarUint32Defaults) {
  uint32_t value = 5;
  // All default arguments
  DebugVar var("var", &value);
  EXPECT_EQ(5, var.GetValue());
  EXPECT_EQ("", var.GetHelp());
  EXPECT_EQ("%u", var.GetFormat());
  EXPECT_EQ(VarType::UINT32, var.GetType());
}

TEST(DebugVar, DebugVarFloatDefaults) {
  float value = 5.0f;
  // All default arguments
  DebugVar var("var", &value);

  EXPECT_EQ("", var.GetHelp());
  EXPECT_EQ("%.3f", var.GetFormat());
  EXPECT_EQ(VarType::FLOAT, var.GetType());

  // Rountrip through uint32 should not change value.
  uint32_t uint_value = var.GetValue();
  var.SetValue(uint_value);
  EXPECT_EQ(uint_value, var.GetValue());
  EXPECT_EQ(5.0f, value);
}

TEST(DebugVar, Registration) {
  int32_t int_value = 5;
  DebugVar var1("var1", &int_value);

  float float_value = 7;
  DebugVar var2("var2", &float_value);

  EXPECT_EQ(&var1, DebugVar::FindVar(var1.GetId()));
  EXPECT_EQ(&var2, DebugVar::FindVar(var2.GetId()));
  EXPECT_EQ(nullptr, DebugVar::FindVar(12345));
}
