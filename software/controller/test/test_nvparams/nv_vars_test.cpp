/* Copyright 2021-2022, RespiraWorks
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

#include "nv_vars.h"

#include "gtest/gtest.h"
#include "mock_eeprom.h"

using namespace Debug::Variable;
using namespace NVParams;

static constexpr Structure ref_params;
// Address of a non-zero value within Structure.
static constexpr uint16_t non_zero_offset{offsetof(Structure, blower_pinch_cal) + 4};
// Address of a positive float array within Structure
static constexpr uint16_t array_offset{offsetof(Structure, exhale_pinch_cal)};
static TestEeprom test_eeprom(0x50, 64, 8192);
static Handler nv_params;

TEST(NonVolatile, Unlinked) {
  // initialise eeprom and nv params
  test_eeprom.WriteBytes(0, sizeof(Structure), &ref_params, nullptr);
  nv_params.Init(&test_eeprom);

  // default constructor creates unlinked var
  NonVolatile nv_var;
  EXPECT_FALSE(nv_var.linked());

  int eeprom_value;
  nv_params.Get(non_zero_offset, &eeprom_value, sizeof(eeprom_value));
  auto var_value{eeprom_value - 1};

  // read/write when not linked has no effect
  nv_var.read(&var_value, sizeof(var_value));
  EXPECT_EQ(var_value, eeprom_value - 1);

  nv_var.write(&var_value, sizeof(var_value));
  nv_params.Get(non_zero_offset, &eeprom_value, sizeof(eeprom_value));
  EXPECT_NE(var_value, eeprom_value);

  // linking
  nv_var.link(&nv_params, non_zero_offset);

  EXPECT_TRUE(nv_var.linked());
  nv_var.read(&var_value, sizeof(var_value));
  EXPECT_EQ(var_value, eeprom_value);
}

TEST(NonVolatile, Linked) {
  // initialise eeprom and nv params
  test_eeprom.WriteBytes(0, sizeof(Structure), &ref_params, nullptr);
  nv_params.Init(&test_eeprom);

  // using constructor with link
  NonVolatile nv_var(&nv_params, non_zero_offset);
  EXPECT_TRUE(nv_var.linked());

  int eeprom_value;
  nv_params.Get(non_zero_offset, &eeprom_value, sizeof(eeprom_value));
  auto var_value{eeprom_value - 1};

  nv_var.read(&var_value, sizeof(var_value));
  EXPECT_EQ(var_value, eeprom_value);

  var_value += 5;

  nv_var.write(&var_value, sizeof(var_value));
  nv_params.Get(non_zero_offset, &eeprom_value, sizeof(eeprom_value));
  EXPECT_EQ(var_value, eeprom_value);

  // unlinking with link(nullptr, 0)
  nv_var.link(nullptr, 0);
  EXPECT_FALSE(nv_var.linked());
}

TEST(NVFloatArray, Unlinked) {
  // initialise eeprom and nv params
  test_eeprom.WriteBytes(0, sizeof(Structure), &ref_params, nullptr);
  nv_params.Init(&test_eeprom);

  // using default constructor
  NVFloatArray<4> nv_array("nv_array", Access::ReadWrite);
  EXPECT_FALSE(nv_array.linked());

  float eeprom_array[nv_array.size()];
  nv_params.Get(array_offset, &eeprom_array, static_cast<uint16_t>(nv_array.byte_size()));

  nv_array.fill(-1.0f);
  // Using negatives since I know that the cal array doesn't use negative values and count on this
  // to check that the float array was modified but eeprom_array is unchanged
  nv_params.Get(array_offset, &eeprom_array, static_cast<uint16_t>(nv_array.byte_size()));
  for (size_t i = 0; i < nv_array.size(); ++i) {
    EXPECT_NE(nv_array.get_data(i), eeprom_array[i]);
    EXPECT_EQ(nv_array.get_data(i), -1.0f);
  }

  nv_array.set_data(2, -2.0f);
  nv_params.Get(array_offset, &eeprom_array, static_cast<uint16_t>(nv_array.byte_size()));
  for (size_t i = 0; i < nv_array.size(); ++i) {
    EXPECT_NE(nv_array.get_data(i), eeprom_array[i]);
    EXPECT_EQ(nv_array.get_data(i), -1.0f - 1.0f * static_cast<float>(i == 2));
  }

  // linking again
  nv_array.link(&nv_params, array_offset);
  // check that a call to link did overwrite the existing array
  for (size_t i = 0; i < nv_array.size(); ++i) {
    EXPECT_EQ(nv_array.get_data(i), eeprom_array[i]);
  }
}

TEST(NVFloatArray, Linked) {
  // initialise eeprom and nv params
  test_eeprom.WriteBytes(0, sizeof(Structure), &ref_params, nullptr);
  nv_params.Init(&test_eeprom);

  // using constructor with link
  NVFloatArray<4> nv_array("nv_array", Access::ReadWrite, &nv_params, array_offset);
  EXPECT_TRUE(nv_array.linked());

  // check initialization from EEPROM
  float eeprom_array[nv_array.size()];
  nv_params.Get(array_offset, &eeprom_array, static_cast<uint16_t>(nv_array.byte_size()));
  for (size_t i = 0; i < nv_array.size(); ++i) {
    EXPECT_EQ(nv_array.get_data(i), eeprom_array[i]);
  }

  nv_array.set_data(0, eeprom_array[0] + 1);
  // check that set_data alters get_data (here I have not yet refreshed eeprom_array from EEPROM)
  EXPECT_EQ(nv_array.get_data(0), eeprom_array[0] + 1);
  // check that EEPROM was also altered properly
  nv_params.Get(array_offset, &eeprom_array, static_cast<uint16_t>(nv_array.byte_size()));
  for (size_t i = 0; i < nv_array.size(); ++i) {
    EXPECT_EQ(nv_array.get_data(i), eeprom_array[i]);
  }

  // fill the array
  nv_array.fill(2.0f);
  nv_params.Get(array_offset, &eeprom_array, static_cast<uint16_t>(nv_array.byte_size()));
  for (size_t i = 0; i < nv_array.size(); ++i) {
    EXPECT_EQ(nv_array.get_data(i), 2.0f);
    EXPECT_EQ(nv_array.get_data(i), eeprom_array[i]);
  }

  // check that deserialize_value alters both nv_array and EEPROM contents
  float write_buff[nv_array.size()] = {-0.5f, 0.0f, 0.5f, 1.0f};
  nv_array.deserialize_value(write_buff);
  nv_params.Get(array_offset, &eeprom_array, static_cast<uint16_t>(nv_array.byte_size()));
  for (size_t i = 0; i < nv_array.size(); ++i) {
    EXPECT_EQ(nv_array.get_data(i), write_buff[i]);
    EXPECT_EQ(nv_array.get_data(i), eeprom_array[i]);
  }

  // using link method with nullptr actually unlinks the array
  nv_array.link(nullptr, 0);
  EXPECT_FALSE(nv_array.linked());
}
