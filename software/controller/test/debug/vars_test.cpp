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

#include "vars.h"
#include "gtest/gtest.h"
#include <limits>
#include <stdint.h>

TEST(DebugVar, DebugVarInt32) {
  int32_t value = 5;
  DebugVar var("var", &value, "help", "fmt", "unit");
  EXPECT_EQ("var", var.GetName());
  EXPECT_EQ("help", var.GetHelp());
  EXPECT_EQ(VarType::Int32, var.GetType());
  EXPECT_EQ("fmt", var.GetFormat());
  EXPECT_EQ("unit", var.GetUnit());
  EXPECT_EQ(VarAccess::ReadWrite, var.GetAccess());
  EXPECT_EQ(&var, DebugVar::FindVar(var.GetId()));

  EXPECT_EQ(uint32_t{5}, var.GetValue());
  var.SetValue(7);
  EXPECT_EQ(uint32_t{7}, var.GetValue());
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
  EXPECT_EQ("", var_default.GetUnit());
}

TEST(DebugVar, ReadOnlyDebugVar) {
  int32_t value = 5;
  // Attempting to SetValue (or Set) read only variables yields false and leave
  // value unchanged
  DebugVar var("var", &value, "help", "fmt", "unit", VarAccess::ReadOnly);
  EXPECT_FALSE(var.SetValue(8));
  EXPECT_EQ(value, var.GetValue());

  DebugInt32 int32("var", "help", value, "fmt", "unit", VarAccess::ReadOnly);
  EXPECT_FALSE(int32.Set(-8));
  EXPECT_EQ(value, int32.GetValue());

  DebugUInt32 uint32("var", "help", static_cast<uint32_t>(value), "fmt", "unit",
                     VarAccess::ReadOnly);
  EXPECT_FALSE(uint32.Set(8));
  EXPECT_EQ(value, uint32.GetValue());

  DebugFloat floatvar("var", "help", 5.0f, "fmt", "unit", VarAccess::ReadOnly);
  EXPECT_FALSE(floatvar.Set(-8.0f));
  EXPECT_EQ(5.0f, floatvar.Get());
}

TEST(DebugVar, DebugVarUint32Defaults) {
  uint32_t value = 5;
  // All default arguments
  DebugVar var("var", &value);
  EXPECT_EQ(uint32_t{5}, var.GetValue());
  EXPECT_EQ("", var.GetHelp());
  EXPECT_EQ("%u", var.GetFormat());
  EXPECT_EQ(VarType::UInt32, var.GetType());
  EXPECT_EQ("", var.GetUnit());
}

TEST(DebugVar, DebugVarFloatDefaults) {
  float value = 5.0f;
  // All default arguments
  DebugVar var("var", &value);

  EXPECT_EQ("", var.GetHelp());
  EXPECT_EQ("%.3f", var.GetFormat());
  EXPECT_EQ(VarType::Float, var.GetType());
  EXPECT_EQ("", var.GetUnit());

  // Rountrip through uint32 should not change value.
  uint32_t uint_value = var.GetValue();
  var.SetValue(uint_value);
  EXPECT_EQ(uint_value, var.GetValue());
  EXPECT_EQ(5.0f, value);
}

TEST(DebugVar, Registration) {
  uint32_t num_vars = DebugVarBase::GetVarCount();
  int32_t int_value = 5;
  DebugVar var1("var1", &int_value);

  EXPECT_EQ(DebugVarBase::GetVarCount(), num_vars + 1);

  float float_value = 7;
  DebugVar var2("var2", &float_value);

  EXPECT_EQ(DebugVarBase::GetVarCount(), num_vars + 2);

  EXPECT_EQ(&var1, DebugVar::FindVar(var1.GetId()));
  EXPECT_EQ(&var2, DebugVar::FindVar(var2.GetId()));
  EXPECT_EQ(nullptr, DebugVar::FindVar(12345));
}
