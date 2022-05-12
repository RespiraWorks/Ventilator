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

#include "interpolant.h"

#include <vector>

#include "gtest/gtest.h"

TEST(Interpolant, Constructors) {
  Interpolant<5> default_fill{"default_fill"};
  // checking the associated debugvar's attributes, I know it is the last registered one
  Debug::Variable::Base *default_array = Debug::Variable::Registry::singleton().find(
      static_cast<uint16_t>(Debug::Variable::Registry::singleton().count() - 1));
  EXPECT_STREQ(default_array->name(), "default_fill");
  EXPECT_STREQ(default_array->units(), "");
  EXPECT_STREQ(default_array->help(), "");
  EXPECT_STREQ(default_array->format(), "%.3f");
  EXPECT_TRUE(default_array->write_allowed());
  // checking that output = input
  for (uint i = 0; i < 11; ++i) {
    float input = static_cast<float>(i) * 0.1f;
    EXPECT_EQ(default_fill.get_value(input), input);
  }

  Interpolant<6> value_filled{"value_filled", 5.0f, 5.0f, "unit", "help for five", "%1.0f"};
  // checking the associated debugvar's attributes
  Debug::Variable::Base *five_array = Debug::Variable::Registry::singleton().find(
      static_cast<uint16_t>(Debug::Variable::Registry::singleton().count() - 1));
  EXPECT_STREQ(five_array->name(), "value_filled");
  EXPECT_STREQ(five_array->units(), "unit");
  EXPECT_STREQ(five_array->help(), "help for five");
  EXPECT_STREQ(five_array->format(), "%1.0f");
  // checking that whatever the input value, output is 5
  for (uint i = 0; i < 11; ++i) {
    EXPECT_EQ(value_filled.get_value(static_cast<float>(i) * 0.1f), 5.0f);
  }

  std::array<float, 7> cosinus{1.0f, 0.866f, 0.5f, 0.0f, -0.5f, -0.866f, -1.0f};
  // checking that the values at the interpolation points match the table's values
  Interpolant<cosinus.size()> cos_interp{"cosinus", cosinus};
  for (uint i = 0; i < cosinus.size(); ++i) {
    EXPECT_EQ(cos_interp.get_value(static_cast<float>(i) / static_cast<float>(cosinus.size() - 1)),
              cosinus[i]);
  }
}

TEST(Interpolant, Interpolation) {
  std::array<float, 3> values{-4.0f, -2.0f, -1.0f};
  Interpolant<3> negative{"negative", values};
  // checking out of bounds input
  EXPECT_EQ(negative.get_value(-0.1f), -4.0f);
  EXPECT_EQ(negative.get_value(1.1f), -1.0f);
  // checking output on the grid
  std::array<float, 3> grid{0.0f, 0.5f, 1.0f};
  for (size_t i = 0; i < values.size(); ++i) {
    EXPECT_EQ(negative.get_value(grid[i]), values[i]);
  }
  // checking interpolation
  std::vector<float> input_values{0.10f, 0.25f, 0.75f};
  std::vector<float> expected_out{-3.6f, -3.0f, -1.5f};
  for (size_t i = 0; i < input_values.size(); ++i) {
    EXPECT_EQ(negative.get_value(input_values[i]), expected_out[i]);
  }
}
