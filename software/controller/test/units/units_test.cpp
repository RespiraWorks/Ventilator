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
#include <type_traits>

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

template <typename T, typename NumTy>
void checkArithmeticOperators(T (*unit)(NumTy), NumTy (T::*get)() const) {
  // x / y is defined only for Ts backed by floats (in practice, everything
  // other than Duration).
  constexpr bool is_fp_based =
      std::is_base_of_v<units_detail::ArithScalar<T, float>, T>;

  // Make t const to check that these operators work with const operands.
  const T t = unit(2.1f);
  EXPECT_FLOAT_EQ(((t + unit(1.2f)).*get)(), 3.3f);
  EXPECT_FLOAT_EQ(((t - unit(1.2f)).*get)(), 0.9f);
  EXPECT_FLOAT_EQ(((t * 2).*get)(), 4.2f);
  if constexpr (is_fp_based) {
    EXPECT_FLOAT_EQ(((t * 2.1f).*get)(), 4.41f);
    EXPECT_FLOAT_EQ(((t / .1f).*get)(), 21.f);
  }
  EXPECT_FLOAT_EQ(1, t / t);
  EXPECT_FLOAT_EQ(2, t * 2 / t);
  EXPECT_FLOAT_EQ(0.25, t / (4 * t));

  T u = unit(2.1f);
  EXPECT_FLOAT_EQ((u.*get)(), 2.1f);
  u += unit(1.2f);
  EXPECT_FLOAT_EQ((u.*get)(), 3.3f);
  u -= unit(5.3f);
  EXPECT_FLOAT_EQ((u.*get)(), -2.0f);
  u *= 2.f;
  EXPECT_FLOAT_EQ((u.*get)(), -4.0f);
  if constexpr (is_fp_based) {
    u /= -0.5f;
    EXPECT_FLOAT_EQ((u.*get)(), 8.0f);
    u *= -0.5f; // undo our change.
    EXPECT_FLOAT_EQ((u.*get)(), -4.0f);
  }

  // Check return type and return value of +=, -=, *=, and /=.
  T v(u += unit(1));
  EXPECT_FLOAT_EQ((u.*get)(), -3.0f);
  EXPECT_FLOAT_EQ((v.*get)(), -3.0f);
  v = (u -= unit(1));
  EXPECT_FLOAT_EQ((u.*get)(), -4.0f);
  EXPECT_FLOAT_EQ((v.*get)(), -4.0f);
  v = (u *= 2);
  EXPECT_FLOAT_EQ((u.*get)(), -8.0f);
  EXPECT_FLOAT_EQ((v.*get)(), -8.0f);
  // x /= y is defined only for Ts backed by floats.
  if constexpr (is_fp_based) {
    v = (u /= 2);
    EXPECT_FLOAT_EQ((u.*get)(), -4.0f);
    EXPECT_FLOAT_EQ((v.*get)(), -4.0f);
    v = (u *= 2); // undo our change.
  }
}

TEST(Units, Pressure) {
  EXPECT_FLOAT_EQ(kPa(1).kPa(), 1);
  EXPECT_FLOAT_EQ(kPa(1).cmH2O(), 10.1972f);
  EXPECT_FLOAT_EQ(cmH2O(1).cmH2O(), 1);
  EXPECT_FLOAT_EQ(atm(1).atm(), 1);
  EXPECT_FLOAT_EQ(atm(1).kPa(), 101.325f);
  EXPECT_FLOAT_EQ((kPa(1) - kPa(2)).kPa(), -1);
  EXPECT_FLOAT_EQ((cmH2O(1) + cmH2O(10)).cmH2O(), 11);
  EXPECT_FLOAT_EQ((kPa(1) - cmH2O(10.1972f)).kPa(), 0);
  EXPECT_FLOAT_EQ((atm(1) - kPa(101.325f)).kPa(), 0);

  checkRelationalOperators(kPa);
  checkRelationalOperators(cmH2O);
  checkRelationalOperators(atm);
  checkArithmeticOperators(kPa, &Pressure::kPa);

  EXPECT_FLOAT_EQ((cmH2O(2.5f) * 101.972f).kPa(), 25);
  EXPECT_FLOAT_EQ((1.01972f * cmH2O(-5.2f)).kPa(), -0.52f);
  EXPECT_FLOAT_EQ((kPa(12.345f) / 10.1972f).cmH2O(), 12.345f);
}

TEST(Units, Length) {
  EXPECT_FLOAT_EQ(meters(1).meters(), 1);
  EXPECT_FLOAT_EQ(meters(1).millimeters(), 1000);
  EXPECT_FLOAT_EQ(millimeters(1).millimeters(), 1);
  EXPECT_FLOAT_EQ(millimeters(1).meters(), 0.001f);
  EXPECT_FLOAT_EQ((meters(1) - meters(2)).meters(), -1);
  EXPECT_FLOAT_EQ((millimeters(1) + millimeters(10)).millimeters(), 11);
  EXPECT_FLOAT_EQ((meters(1) - millimeters(1000)).meters(), 0);

  checkRelationalOperators(meters);
  checkRelationalOperators(millimeters);
  checkArithmeticOperators(meters, &Length::meters);

  EXPECT_FLOAT_EQ((millimeters(1234) * 1.5f).meters(), 1.851f);
  EXPECT_FLOAT_EQ((2.345f * millimeters(654.3f)).meters(), 1.5343335f);
  EXPECT_FLOAT_EQ((meters(2.46f) / 2000.0f).millimeters(), 1.23f);
}

TEST(Units, VolumetricFlow) {
  EXPECT_FLOAT_EQ(cubic_m_per_sec(1).cubic_m_per_sec(), 1);
  EXPECT_FLOAT_EQ(cubic_m_per_sec(1).ml_per_min(), 60.0f * 1000 * 1000);
  EXPECT_FLOAT_EQ(cubic_m_per_sec(1).liters_per_sec(), 1000.0f);
  EXPECT_FLOAT_EQ(cubic_m_per_sec(1).ml_per_sec(), 1.0e6f);
  EXPECT_FLOAT_EQ(ml_per_min(1).ml_per_min(), 1);
  EXPECT_FLOAT_EQ(ml_per_min(1).cubic_m_per_sec(), 1 / (60.0f * 1000 * 1000));
  EXPECT_FLOAT_EQ(ml_per_min(1).liters_per_sec(), 1 / (60.0f * 1000));
  EXPECT_FLOAT_EQ(ml_per_min(1).ml_per_sec(), 1 / 60.0f);
  EXPECT_FLOAT_EQ(ml_per_sec(1).ml_per_sec(), 1);
  EXPECT_FLOAT_EQ(ml_per_sec(1).ml_per_min(), 60.0f);
  EXPECT_FLOAT_EQ(ml_per_sec(1).cubic_m_per_sec(), 1 / 1.0e6f);
  EXPECT_FLOAT_EQ(ml_per_sec(1).liters_per_sec(), 1 / 1000.0f);
  EXPECT_FLOAT_EQ(liters_per_sec(1).liters_per_sec(), 1);
  EXPECT_FLOAT_EQ(liters_per_sec(1).ml_per_min(), 1 * (60.0f * 1000));
  EXPECT_FLOAT_EQ(liters_per_sec(1).ml_per_sec(), 1000.0f);
  EXPECT_FLOAT_EQ(liters_per_sec(1).cubic_m_per_sec(), 1 / (1000.0f));
  EXPECT_FLOAT_EQ((cubic_m_per_sec(1) - cubic_m_per_sec(2)).cubic_m_per_sec(),
                  -1);
  EXPECT_FLOAT_EQ((ml_per_min(1) + ml_per_min(10)).ml_per_min(), 11);
  EXPECT_FLOAT_EQ(
      (cubic_m_per_sec(1) - ml_per_min(60.0f * 1000 * 1000)).cubic_m_per_sec(),
      0);
  EXPECT_FLOAT_EQ(
      (cubic_m_per_sec(1) - liters_per_sec(1000.0f)).cubic_m_per_sec(), 0);

  checkRelationalOperators(cubic_m_per_sec);
  checkRelationalOperators(ml_per_min);
  checkRelationalOperators(liters_per_sec);
  checkArithmeticOperators(cubic_m_per_sec, &VolumetricFlow::cubic_m_per_sec);

  EXPECT_FLOAT_EQ((liters_per_sec(4.0f) * 500.0f).cubic_m_per_sec(), 2.0f);
  EXPECT_FLOAT_EQ((1234.0f * liters_per_sec(2.5f)).cubic_m_per_sec(), 3.085f);
  EXPECT_FLOAT_EQ((cubic_m_per_sec(6.283f) / 2000.0f).liters_per_sec(),
                  3.1415f);

  EXPECT_FLOAT_EQ((cubic_m(3.198f) / seconds(2.6f)).cubic_m_per_sec(), 1.23f);
}

TEST(Units, Volume) {
  EXPECT_FLOAT_EQ(cubic_m(1).cubic_m(), 1);
  EXPECT_FLOAT_EQ(cubic_m(1).ml(), 1000 * 1000);
  EXPECT_FLOAT_EQ(ml(1).ml(), 1.0);
  EXPECT_FLOAT_EQ(ml(1).cubic_m(), 1 / (1000.0f * 1000));
  EXPECT_FLOAT_EQ((cubic_m(1) - cubic_m(2)).cubic_m(), -1);
  EXPECT_FLOAT_EQ((ml(1) + ml(10)).ml(), 11);
  EXPECT_FLOAT_EQ((cubic_m(1) - ml(1000 * 1000)).cubic_m(), 0);

  checkRelationalOperators(cubic_m);
  checkRelationalOperators(ml);
  checkArithmeticOperators(ml, &Volume::ml);

  EXPECT_FLOAT_EQ((cubic_m(0.123f) / 1e4f).ml(), 12.3f);
  EXPECT_FLOAT_EQ((ml(1234.0f) * 567.0f).cubic_m(), 0.699678f);

  EXPECT_FLOAT_EQ((liters_per_sec(1.47f) * seconds(2.58f)).ml(), 3792.6f);
  EXPECT_FLOAT_EQ((seconds(4.56f) * liters_per_sec(3.1415f)).ml(), 14325.24f);
}

TEST(Units, Duration) {
  EXPECT_FLOAT_EQ(seconds(1).seconds(), 1);
  EXPECT_FLOAT_EQ(minutes(1).seconds(), 60);
  EXPECT_FLOAT_EQ(minutes(10).seconds(), 600);
  EXPECT_FLOAT_EQ(seconds(300).minutes(), 5);
  EXPECT_FLOAT_EQ(seconds(1).milliseconds(), 1000);
  EXPECT_FLOAT_EQ(milliseconds(1).milliseconds(), 1);
  EXPECT_FLOAT_EQ(milliseconds(1).seconds(), 0.001f);
  EXPECT_FLOAT_EQ((seconds(2) - seconds(1)).seconds(), 1);
  EXPECT_FLOAT_EQ((seconds(1) - seconds(2)).seconds(), -1);
  EXPECT_FLOAT_EQ((milliseconds(1) + milliseconds(10)).milliseconds(), 11);
  EXPECT_FLOAT_EQ((seconds(1) - milliseconds(1000)).seconds(), 0);

  checkRelationalOperators(seconds);
  checkArithmeticOperators(seconds, &Duration::seconds);

  // Test all the overloads of the milliseconds factory function.  static_cast
  // is one of C++'s weird ways of getting the address of an overloaded
  // function, see
  // https://en.cppreference.com/w/cpp/language/overloaded_address
  checkRelationalOperators(static_cast<Duration (*)(int64_t)>(milliseconds));
  checkRelationalOperators(static_cast<Duration (*)(float)>(milliseconds));
  checkRelationalOperators(static_cast<Duration (*)(double)>(milliseconds));
}

TEST(Units, Time) {
  // "microsSinceStartup" is long and makes this test hard to read; shorten it.
  auto us = &microsSinceStartup;

  EXPECT_EQ(us(42).microsSinceStartup(), 42u);
  EXPECT_EQ((us(10) + microseconds(1)).microsSinceStartup(), 11u);
  EXPECT_EQ((microseconds(42) + us(100)).microsSinceStartup(), 142u);
  EXPECT_EQ((us(2000) - microseconds(1000)).microsSinceStartup(), 1000u);
  EXPECT_EQ((us(5432) - milliseconds(3)).microsSinceStartup(), 2432u);
  EXPECT_EQ((us(2 * 1000 * 1000) - seconds(1)).microsSinceStartup(), 1'000'000);
  EXPECT_FLOAT_EQ((us(1000) - microsSinceStartup(500)).milliseconds(), 0.5f);
  EXPECT_EQ((us(1000) + microseconds(-500)).microsSinceStartup(), 500);
  EXPECT_EQ((microseconds(-100) + us(1000)).microsSinceStartup(), 900);
  // Negative times are not supported:
  // EXPECT_EQ((us(500) - us(1000)).seconds(), ???);
  Time time = us(2000);
  time += microseconds(1000);
  EXPECT_EQ(time.microsSinceStartup(), static_cast<uint64_t>(3000));
  time -= microseconds(2000);
  EXPECT_EQ(time.microsSinceStartup(), static_cast<uint64_t>(1000));

  checkRelationalOperators(microsSinceStartup);
}
