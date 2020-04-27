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

#include "units.h"
#include "gtest/gtest.h"

template <typename T, typename NumTy>
void checkRelationalOperators(T (*unit)(NumTy)) {
  EXPECT_TRUE(unit(1) < unit(2));
  EXPECT_FALSE(unit(1) < unit(1));
  EXPECT_FALSE(unit(2) < unit(1));

  EXPECT_TRUE(unit(1) <= unit(2));
  EXPECT_TRUE(unit(1) <= unit(1));
  EXPECT_FALSE(unit(2) <= unit(1));

  EXPECT_FALSE(unit(1) == unit(2));
  EXPECT_TRUE(unit(1) == unit(1));

  EXPECT_TRUE(unit(1) != unit(2));
  EXPECT_FALSE(unit(1) != unit(1));

  EXPECT_FALSE(unit(1) >= unit(2));
  EXPECT_TRUE(unit(1) >= unit(1));
  EXPECT_TRUE(unit(2) >= unit(1));

  EXPECT_FALSE(unit(1) > unit(2));
  EXPECT_FALSE(unit(1) > unit(1));
  EXPECT_TRUE(unit(2) > unit(1));
}

TEST(Units, Pressure) {
  EXPECT_FLOAT_EQ(kPa(1).kPa(), 1);
  EXPECT_FLOAT_EQ(kPa(1).cmH2O(), 10.1972);
  EXPECT_FLOAT_EQ(cmH2O(1).cmH2O(), 1);
  EXPECT_FLOAT_EQ((kPa(1) - kPa(2)).kPa(), -1);
  EXPECT_FLOAT_EQ((cmH2O(1) + cmH2O(10)).cmH2O(), 11);
  EXPECT_FLOAT_EQ((kPa(1) - cmH2O(10.1972)).kPa(), 0);

  checkRelationalOperators(kPa);
  checkRelationalOperators(cmH2O);
}

TEST(Units, Length) {
  EXPECT_FLOAT_EQ(meters(1).meters(), 1);
  EXPECT_FLOAT_EQ(meters(1).millimeters(), 1000);
  EXPECT_FLOAT_EQ(millimeters(1).millimeters(), 1);
  EXPECT_FLOAT_EQ(millimeters(1).meters(), 0.001);
  EXPECT_FLOAT_EQ((meters(1) - meters(2)).meters(), -1);
  EXPECT_FLOAT_EQ((millimeters(1) + millimeters(10)).millimeters(), 11);
  EXPECT_FLOAT_EQ((meters(1) - millimeters(1000)).meters(), 0);

  checkRelationalOperators(meters);
  checkRelationalOperators(millimeters);
}

TEST(Units, VolumetricFlow) {
  EXPECT_FLOAT_EQ(cubic_m_per_sec(1).cubic_m_per_sec(), 1);
  EXPECT_FLOAT_EQ(cubic_m_per_sec(1).ml_per_min(), 60.0 * 1000 * 1000);
  EXPECT_FLOAT_EQ(ml_per_min(1).ml_per_min(), 1);
  EXPECT_FLOAT_EQ(ml_per_min(1).cubic_m_per_sec(), 1 / (60.0 * 1000 * 1000));
  EXPECT_FLOAT_EQ((cubic_m_per_sec(1) - cubic_m_per_sec(2)).cubic_m_per_sec(),
                  -1);
  EXPECT_FLOAT_EQ((ml_per_min(1) + ml_per_min(10)).ml_per_min(), 11);
  EXPECT_FLOAT_EQ(
      (cubic_m_per_sec(1) - ml_per_min(60.0 * 1000 * 1000)).cubic_m_per_sec(),
      0);

  checkRelationalOperators(cubic_m_per_sec);
  checkRelationalOperators(ml_per_min);
}

TEST(Units, Duration) {
  EXPECT_FLOAT_EQ(seconds(1).seconds(), 1);
  EXPECT_FLOAT_EQ(seconds(1).milliseconds(), 1000);
  EXPECT_FLOAT_EQ(milliseconds(1).milliseconds(), 1);
  EXPECT_FLOAT_EQ(milliseconds(1).seconds(), 0.001);
  EXPECT_FLOAT_EQ((seconds(1) - seconds(2)).seconds(), -1);
  EXPECT_FLOAT_EQ((milliseconds(1) + milliseconds(10)).milliseconds(), 11);
  EXPECT_FLOAT_EQ((seconds(1) - milliseconds(1000)).seconds(), 0);

  checkRelationalOperators(seconds);
  checkRelationalOperators(milliseconds);
}

TEST(Units, Time) {
  // "millisSinceStartup" is long and makes this test hard to read; shorten it.
  auto ms = &millisSinceStartup;

  EXPECT_FLOAT_EQ(ms(42).millisSinceStartup(), 42);
  EXPECT_FLOAT_EQ((ms(10) + milliseconds(1)).millisSinceStartup(), 11);
  EXPECT_FLOAT_EQ((milliseconds(42) + ms(100)).millisSinceStartup(), 142);
  EXPECT_FLOAT_EQ((ms(2000) - milliseconds(1000)).millisSinceStartup(), 1000);
  EXPECT_FLOAT_EQ((ms(5432) - seconds(3)).millisSinceStartup(), 2432);
  EXPECT_FLOAT_EQ((ms(1000) - millisSinceStartup(500)).seconds(), 0.5);
  EXPECT_FLOAT_EQ((ms(500) - ms(1000)).seconds(), -0.5);

  checkRelationalOperators(millisSinceStartup);
}
