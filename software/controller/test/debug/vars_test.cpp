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
  int32_t value{5};
  int32_t other_val{0};
  Debug::Variable::Primitive32 var("var", Debug::Variable::Access::ReadOnly, &value, "unit", "help",
                                   "fmt");
  EXPECT_STREQ("var", var.name());
  var.prepend_name("pre_");
  EXPECT_STREQ("pre_var", var.name());
  EXPECT_STREQ("help", var.help());
  var.append_help(" so much help");
  EXPECT_STREQ("help so much help", var.help());
  EXPECT_EQ(Debug::Variable::Type::Int32, var.type());
  EXPECT_STREQ("fmt", var.format());
  EXPECT_STREQ("unit", var.units());
  EXPECT_EQ(Debug::Variable::Access::ReadOnly, var.access());
  EXPECT_EQ(&var, Debug::Variable::Registry::singleton().find(var.id()));

  other_val = 0;
  var.get_value(&other_val);
  EXPECT_EQ(int32_t{5}, other_val);
  other_val = 7;
  var.set_value(&other_val);
  other_val = 0;
  var.get_value(&other_val);
  EXPECT_EQ(int32_t{7}, other_val);
  EXPECT_EQ(7, value);

  // All default arguments
  Debug::Variable::Primitive32 var_default("var", Debug::Variable::Access::ReadWrite, &value,
                                           "unit");
  EXPECT_STREQ("", var_default.help());
  EXPECT_STREQ("%d", var_default.format());
}

TEST(DebugVar, DebugVarUint32Defaults) {
  uint32_t value{5};
  uint32_t other_val{0};
  // All default arguments
  Debug::Variable::Primitive32 var("var", Debug::Variable::Access::ReadWrite, &value, "unit");
  other_val = 0;
  var.get_value(&other_val);
  EXPECT_EQ(uint32_t{5}, other_val);
  EXPECT_STREQ("", var.help());
  EXPECT_STREQ("%u", var.format());
  EXPECT_EQ(Debug::Variable::Type::UInt32, var.type());
}

TEST(DebugVar, DebugVarFloatDefaults) {
  float value{5.0f};
  // All default arguments
  Debug::Variable::Primitive32 var("var", Debug::Variable::Access::ReadWrite, &value, "unit");

  EXPECT_STREQ("", var.help());
  EXPECT_STREQ("%.3f", var.format());
  EXPECT_EQ(Debug::Variable::Type::Float, var.type());

  // Rountrip through uint32 should not change value.
  uint32_t uint_val{0};
  var.get_value(&uint_val);
  var.set_value(&uint_val);
  uint32_t other_val{0};
  var.get_value(&other_val);
  EXPECT_EQ(uint_val, other_val);
  EXPECT_EQ(5.0f, value);
}

TEST(DebugVar, FloatArray) {
  Debug::Variable::FloatArray<3> fa3("fa3", Debug::Variable::Access::ReadWrite, "units");
  EXPECT_EQ(fa3.size(), 4 * 3);
  fa3.data[0] = 24.0f;
  fa3.data[1] = 42.0f;
  fa3.data[2] = 69.0f;
  std::array<float, 3> compare_to{24.0f, 42.0f, 69.0f};

  EXPECT_EQ(fa3.data, compare_to);

  uint32_t buffer[3 * 4];
  fa3.get_value(buffer);
  fa3.data = {0};
  fa3.set_value(buffer);
  EXPECT_EQ(fa3.data, compare_to);
  EXPECT_NE(fa3.data.data(), compare_to.data());
  EXPECT_NE(reinterpret_cast<void*>(fa3.data.data()), reinterpret_cast<void*>(buffer));

  Debug::Variable::FloatArray<5> fa5("fa5", Debug::Variable::Access::ReadWrite, 4.0f, "units");
  EXPECT_EQ(fa5.data[4], 4.0f);

  Debug::Variable::FloatArray<2> fa2("fa2", Debug::Variable::Access::ReadWrite, {1.0f, 2.0f},
                                     "units");
  EXPECT_EQ(fa2.data[0], 1.0f);
  EXPECT_EQ(fa2.data[1], 2.0f);
}

TEST(DebugVar, Registration) {
  uint32_t num_vars = Debug::Variable::Registry::singleton().count();
  int32_t int_value = 5;
  Debug::Variable::Primitive32 var1("var1", Debug::Variable::Access::ReadWrite, &int_value, "unit");

  EXPECT_EQ(Debug::Variable::Registry::singleton().count(), num_vars + 1);

  float float_value = 7;
  Debug::Variable::Primitive32 var2("var2", Debug::Variable::Access::ReadWrite, &float_value,
                                    "unit");

  EXPECT_EQ(Debug::Variable::Registry::singleton().count(), num_vars + 2);

  EXPECT_EQ(&var1, Debug::Variable::Registry::singleton().find(var1.id()));
  EXPECT_EQ(&var2, Debug::Variable::Registry::singleton().find(var2.id()));
  EXPECT_EQ(nullptr, Debug::Variable::Registry::singleton().find(12345));
}
