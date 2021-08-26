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

TEST(DebugVar, DebugVarInt32) {
  int32_t value = 5;
  Debug::Variable::Primitive32 var("var", Debug::Variable::Access::ReadOnly,
                                   &value, "unit", "help", "fmt");
  EXPECT_STREQ("var", var.GetName());
  var.prepend_name("pre_");
  EXPECT_STREQ("pre_var", var.GetName());
  EXPECT_STREQ("help", var.GetHelp());
  var.append_help(" so much help");
  EXPECT_STREQ("help so much help", var.GetHelp());
  EXPECT_EQ(Debug::Variable::Type::Int32, var.GetType());
  EXPECT_STREQ("fmt", var.GetFormat());
  EXPECT_STREQ("unit", var.GetUnits());
  EXPECT_EQ(Debug::Variable::Access::ReadOnly, var.GetAccess());
  EXPECT_EQ(&var, Debug::Variable::Registry::singleton().FindVar(var.GetId()));

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
  Debug::Variable::Primitive32 var_default(
      "var", Debug::Variable::Access::ReadWrite, &value, "unit");
  EXPECT_STREQ("", var_default.GetHelp());
  EXPECT_STREQ("%d", var_default.GetFormat());
}

TEST(DebugVar, DebugVarUint32Defaults) {
  uint32_t value = 5;
  // All default arguments
  Debug::Variable::Primitive32 var("var", Debug::Variable::Access::ReadWrite,
                                   &value, "unit");
  EXPECT_EQ(uint32_t{5}, var.GetValue());
  EXPECT_STREQ("", var.GetHelp());
  EXPECT_STREQ("%u", var.GetFormat());
  EXPECT_EQ(Debug::Variable::Type::UInt32, var.GetType());
}

TEST(DebugVar, DebugVarFloatDefaults) {
  float value = 5.0f;
  // All default arguments
  Debug::Variable::Primitive32 var("var", Debug::Variable::Access::ReadWrite,
                                   &value, "unit");

  EXPECT_STREQ("", var.GetHelp());
  EXPECT_STREQ("%.3f", var.GetFormat());
  EXPECT_EQ(Debug::Variable::Type::Float, var.GetType());

  // Rountrip through uint32 should not change value.
  uint32_t uint_value = var.GetValue();
  var.SetValue(uint_value);
  EXPECT_EQ(uint_value, var.GetValue());
  EXPECT_EQ(5.0f, value);
}

TEST(DebugVar, Registration) {
  uint32_t num_vars = Debug::Variable::Registry::singleton().GetVarCount();
  int32_t int_value = 5;
  Debug::Variable::Primitive32 var1("var1", Debug::Variable::Access::ReadWrite,
                                    &int_value, "unit");

  EXPECT_EQ(Debug::Variable::Registry::singleton().GetVarCount(), num_vars + 1);

  float float_value = 7;
  Debug::Variable::Primitive32 var2("var2", Debug::Variable::Access::ReadWrite,
                                    &float_value, "unit");

  EXPECT_EQ(Debug::Variable::Registry::singleton().GetVarCount(), num_vars + 2);

  EXPECT_EQ(&var1,
            Debug::Variable::Registry::singleton().FindVar(var1.GetId()));
  EXPECT_EQ(&var2,
            Debug::Variable::Registry::singleton().FindVar(var2.GetId()));
  EXPECT_EQ(nullptr, Debug::Variable::Registry::singleton().FindVar(12345));
}
