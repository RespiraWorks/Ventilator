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

using namespace Debug::Variable;

TEST(DebugVar, DebugVarInt32) {
  int32_t value{5};
  int32_t other_val{0};
  Primitive32 var("var", Access::ReadOnly, &value, "unit", "help", "fmt");
  EXPECT_STREQ("var", var.name());
  var.prepend_name("pre_");
  EXPECT_STREQ("pre_var", var.name());
  EXPECT_STREQ("help", var.help());
  var.append_help(" so much help");
  EXPECT_STREQ("help so much help", var.help());
  EXPECT_EQ(Type::Int32, var.type());
  EXPECT_STREQ("fmt", var.format());
  EXPECT_STREQ("unit", var.units());
  EXPECT_EQ(Access::ReadOnly, var.access());
  EXPECT_EQ(&var, Registry::singleton().find(var.id()));

  other_val = 0;
  var.serialize_value(&other_val);
  EXPECT_EQ(int32_t{5}, other_val);
  other_val = 7;
  var.deserialize_value(&other_val);
  other_val = 0;
  var.serialize_value(&other_val);
  EXPECT_EQ(int32_t{7}, other_val);
  EXPECT_EQ(7, value);

  // All default arguments
  Primitive32 var_default("var", Access::ReadWrite, &value, "unit");
  EXPECT_STREQ("", var_default.help());
  EXPECT_STREQ("%d", var_default.format());
}

TEST(DebugVar, DebugVarUint32Defaults) {
  uint32_t value{5};
  uint32_t other_val{0};
  // All default arguments
  Primitive32 var("var", Access::ReadWrite, &value, "unit");
  other_val = 0;
  var.serialize_value(&other_val);
  EXPECT_EQ(uint32_t{5}, other_val);
  EXPECT_STREQ("", var.help());
  EXPECT_STREQ("%u", var.format());
  EXPECT_EQ(Type::UInt32, var.type());
}

TEST(DebugVar, DebugVarFloatDefaults) {
  float value{5.0f};
  // All default arguments
  Primitive32 var("var", Access::ReadWrite, &value, "unit");

  EXPECT_STREQ("", var.help());
  EXPECT_STREQ("%.3f", var.format());
  EXPECT_EQ(Type::Float, var.type());

  // Rountrip through uint32 should not change value.
  uint32_t uint_val{0};
  var.serialize_value(&uint_val);
  var.deserialize_value(&uint_val);
  uint32_t other_val{0};
  var.serialize_value(&other_val);
  EXPECT_EQ(uint_val, other_val);
  EXPECT_EQ(5.0f, value);
}

TEST(DebugVar, FloatArray) {
  FloatArray<3> fa3("fa3", Access::ReadWrite, "units");
  EXPECT_EQ(fa3.byte_size(), 4 * 3);
  fa3.data[0] = 24.0f;
  fa3.data[1] = 42.0f;
  fa3.data[2] = 69.0f;
  std::array<float, 3> compare_to{24.0f, 42.0f, 69.0f};

  EXPECT_EQ(fa3.data, compare_to);

  uint32_t buffer[3 * 4];
  fa3.serialize_value(buffer);
  fa3.data = {0};
  fa3.deserialize_value(buffer);
  EXPECT_EQ(fa3.data, compare_to);
  EXPECT_NE(fa3.data.data(), compare_to.data());
  EXPECT_NE(reinterpret_cast<void*>(fa3.data.data()), reinterpret_cast<void*>(buffer));

  FloatArray<5> fa5("fa5", Access::ReadWrite, 4.0f, "units");
  EXPECT_EQ(fa5.data[4], 4.0f);

  FloatArray<2> fa2("fa2", Access::ReadWrite, {1.0f, 2.0f}, "units");
  EXPECT_EQ(fa2.data[0], 1.0f);
  EXPECT_EQ(fa2.data[1], 2.0f);
}

TEST(DebugVar, Registration) {
  uint32_t num_vars = Registry::singleton().count();
  int32_t int_value = 5;
  Primitive32 var1("var1", Access::ReadWrite, &int_value, "unit");

  EXPECT_EQ(Registry::singleton().count(), num_vars + 1);

  float float_value = 7;
  Primitive32 var2("var2", Access::ReadWrite, &float_value, "unit");

  EXPECT_EQ(Registry::singleton().count(), num_vars + 2);

  EXPECT_EQ(&var1, Registry::singleton().find(var1.id()));
  EXPECT_EQ(&var2, Registry::singleton().find(var2.id()));
  EXPECT_EQ(nullptr, Registry::singleton().find(12345));
}
