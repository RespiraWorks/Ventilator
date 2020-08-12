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

#ifndef UNITS_H
#define UNITS_H

#include <stdint.h>
#include <type_traits>

// Wrappers for measurements of different physical quantities [1] (length,
// time, etc.).
//
// The goals here are:
//
//  - Prevent unit mismatches.  If it can happen to JPL, it can happen to us.
//  - Make the physical quantity of variables explicit -- e.g. "this variable
//    contains a length".
//  - Achieve the above with minimal to no overhead.
//
// Physical quantities currently modeled:
//
//   - Length (e.g. meters)
//   - Pressure (e.g. kPa)
//   - Volumetric flow (e.g. m^3/s)
//   - Volume (e.g. m^3)
//   - Electric potential (volts)
//   - Elapsed time since startup (μs)
//   - Duration, aka time interval (μs)
//
// Feel free to add new ones!
//
// Example usage:
//
//   Pressure p = kPa(42);
//   Pressure q = cmH2O(10);
//   Pressure diff = kPa(42) - cmH2O(10);
//   printf("42 kPa - 10 cm_h20 is %f kPa or %f cm_h2o\n",
//          diff.kPa(), diff.cmH2O());
//
// Observe that we create an instance of Pressure by calling the kPa() bare
// function.  We provide the following bare functions
//
//   Pressure         kPa(float)
//   Pressure         cmH2O(float)
//   Length           meters(float)
//   Length           millimeters(float)
//   VolumetricFlow   cubic_m_per_sec(float)
//   VolumetricFlow   ml_per_min(float)
//   Volume           cubic_m(float)
//   Volume           ml(float)
//   Voltage          volts(float)
//   Duration         seconds(float)
//   Duration         milliseconds(int64_t)
//   Duration         milliseconds(float)
//   Duration         microseconds(int64_t)
//   Time             microsSinceStartup(int64_t)
//
// Values support addition and subtraction.  The laws for Duration and Time are
// different than the other units:
//
//   Duration + Duration = Duration
//   Duration - Duration = Duration
//   Time + Duration     = Time
//   Time - Duration     = Time
//   Time - Time         = Duration
//   Time + Time           NOT ALLOWED
//   Duration - Time       NOT ALLOWED
//
// [1] https://en.wikipedia.org/wiki/Physical_quantity

namespace units_detail {

// Represents a value of some physical quantity Q, e.g. length, pressure, time.
//
// The value is stored as type ValTy (e.g. float, int64_t), and standard
// comparison operators are available.
//
// The motivation for this base class is, if we don't, we have to type out the
// comparison operators for each type.  I'm afraid of typo'ing them, as those
// bugs are bad and hard to catch.  The C++20 spaceship operator would obviate
// the need for all this, but it requires gcc 10, which is newer than we can
// get.
//
// This is a CRTP [1] class; Q should inherit from Scalar<Q>, e.g.
//
//   class Length : public Scalar<Length, float> { ... };
//
// [1] https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
template <class Q, class ValTy> class Scalar {
public:
  Scalar() : val_(0) {}

  constexpr bool operator<(const Scalar &s) const { return val_ < s.val_; }
  constexpr bool operator<=(const Scalar &s) const { return val_ <= s.val_; }
  constexpr bool operator==(const Scalar &s) const { return val_ == s.val_; }
  constexpr bool operator!=(const Scalar &s) const { return val_ != s.val_; }
  constexpr bool operator>=(const Scalar &s) const { return val_ >= s.val_; }
  constexpr bool operator>(const Scalar &s) const { return val_ > s.val_; }

protected:
  constexpr explicit Scalar(ValTy val) : val_(val) {}
  ValTy val_;
};

// Represents a Scalar where certain arithmetic operations are defined.
//
// Let x and y be ArithScalars of the same type, and let c be a constant of
// type ValTy.  Then the following operations are defined.
//
// - x + y and x += y
// - x - y and x -= y
// - x / y (ratio of two measurements, yields a float)
// - c * x, x * c, and x *= c
// - if ValTy is floating point, x / c and x /= c.  If ValTy is not
//   floating-point, these operators are not defined.
//
// Be aware that dividing by 0 yields Inf and needs to be protected.
template <class Q, class ValTy> class ArithScalar : public Scalar<Q, ValTy> {
public:
  constexpr Q operator+(const ArithScalar &a) const {
    return Q(this->val_ + a.val_);
  }
  Q &operator+=(const ArithScalar &a) {
    return static_cast<Q &>(*this = *this + a);
  }

  constexpr Q operator-(const ArithScalar &a) const {
    return Q(this->val_ - a.val_);
  }
  Q &operator-=(const ArithScalar &a) {
    return static_cast<Q &>(*this = *this - a);
  }

  constexpr Q operator*(const ValTy &a) const { return Q(this->val_ * a); }
  constexpr friend Q operator*(const ValTy &a, const Q &b) {
    return Q(b.val_ * a);
  }
  Q &operator*=(const ValTy &a) { return static_cast<Q &>(*this = *this * a); }

  // Division by a unitless scalar, defined only if ValTy is floating-point.
  template <typename VT = ValTy,
            std::enable_if_t<std::is_floating_point_v<VT>, int> = 0>
  constexpr Q operator/(const ValTy &a) const {
    return Q(this->val_ / a);
  }
  template <typename VT = ValTy,
            std::enable_if_t<std::is_floating_point_v<VT>, int> = 0>
  Q &operator/=(const ValTy &a) {
    return static_cast<Q &>(*this = *this / a);
  }

  // Ratio of two measurements.  Always defined, and always returns a float,
  // irrespective of ValTy.
  constexpr float operator/(const ArithScalar &a) const {
    return static_cast<float>(this->val_) / static_cast<float>(a.val_);
  }

protected:
  // Pull in base class's constructor.
  using Scalar<Q, ValTy>::Scalar;
};

} // namespace units_detail

// Represents pressure, e.g. air pressure.
//
// Precision: float.
//
// Units:
//  - kPa (kilopascals)
//  - cm h2o (centimeters of water)
//  - atm (atmospheres)
//
// Native unit (implementation detail): kPa
class Pressure : public units_detail::ArithScalar<Pressure, float> {
public:
  [[nodiscard]] constexpr float kPa() const { return val_; }
  [[nodiscard]] constexpr float cmH2O() const { return val_ * kKPaToCmH2O; }
  [[nodiscard]] constexpr float atm() const { return val_ / kAtmToKPa; }

private:
  // https://www.google.com/search?q=kpa+to+cmh2o
  static constexpr float kKPaToCmH2O{10.1972f};
  // https://www.google.com/search?q=atm+to+kPa
  static constexpr float kAtmToKPa{101.325f};

  constexpr friend Pressure kPa(float kpa);
  constexpr friend Pressure cmH2O(float cm_h2o);
  constexpr friend Pressure atm(float atm);

  using units_detail::ArithScalar<Pressure, float>::ArithScalar;
};

constexpr Pressure kPa(float kpa) { return Pressure(kpa); }
constexpr Pressure cmH2O(float cm_h2o) {
  return Pressure(cm_h2o / Pressure::kKPaToCmH2O);
}
constexpr Pressure atm(float atm) {
  return Pressure(atm * Pressure::kAtmToKPa);
}

// Represents a length.
//
// Precision: float.
//
// Units:
//   - meters
//   - millimeters
//
// Native unit (implementation detail): meters
class Length : public units_detail::ArithScalar<Length, float> {
public:
  [[nodiscard]] constexpr float meters() const { return val_; }
  [[nodiscard]] constexpr float millimeters() const { return val_ * 1000; }

private:
  constexpr friend Length meters(float meters);
  constexpr friend Length millimeters(float mm);

  using units_detail::ArithScalar<Length, float>::ArithScalar;
};

constexpr Length meters(float meters) { return Length(meters); }
constexpr Length millimeters(float mm) { return Length(mm / 1000); }

// Represents flow over time, the rate of air passing through a tube.
//
// Precision: float.
//
// Units:
//
//   - meters^3/sec
//   - mL^3/min
//   - liters/sec
//   - ml/sec
//
// Native unit (implementation detail): meters^3/sec
//
// Dividing a Volume by a Duration (see below) gives you a VolumetricFlow.
// Multiplying a VolumetricFlow by a Duration (see below) gives you a Volume.
class VolumetricFlow : public units_detail::ArithScalar<VolumetricFlow, float> {
public:
  [[nodiscard]] constexpr float cubic_m_per_sec() const { return val_; }
  [[nodiscard]] constexpr float ml_per_min() const {
    return val_ * 1000.0f * 1000.0f * 60.0f;
  }
  [[nodiscard]] constexpr float liters_per_sec() const {
    return val_ * 1000.0f;
  }
  [[nodiscard]] constexpr float ml_per_sec() const { return val_ * 1.0e6f; }

private:
  constexpr friend VolumetricFlow cubic_m_per_sec(float m3ps);
  constexpr friend VolumetricFlow ml_per_min(float ml_per_min);
  constexpr friend VolumetricFlow liters_per_sec(float lps);
  constexpr friend VolumetricFlow ml_per_sec(float mmps);

  using units_detail::ArithScalar<VolumetricFlow, float>::ArithScalar;
};

constexpr VolumetricFlow cubic_m_per_sec(float m3ps) {
  return VolumetricFlow(m3ps);
}
constexpr VolumetricFlow ml_per_min(float ml_per_min) {
  return VolumetricFlow(ml_per_min / (1000.0f * 1000.0f * 60.0f));
}
constexpr VolumetricFlow liters_per_sec(float lps) {
  return VolumetricFlow(lps / (1000.0f));
}
constexpr VolumetricFlow ml_per_sec(float mmps) {
  return VolumetricFlow(mmps / (1.0e6f));
}

// Represents volume.
//
// Precision: float.
//
// Units:
//
//   - meters^3
//   - mL
//
// Native unit (implementation detail): meters^3
//
// Multiplying a VolumetricFlow by a Duration (see below) gives you a Volume.
// Dividing a Volume by a Duration gives you a VolumetricFlow.
class Volume : public units_detail::ArithScalar<Volume, float> {
public:
  [[nodiscard]] constexpr float cubic_m() const { return val_; }
  [[nodiscard]] constexpr float ml() const { return val_ * 1000.0f * 1000.0f; }

private:
  constexpr friend Volume cubic_m(float m3);
  constexpr friend Volume ml(float ml);

  using units_detail::ArithScalar<Volume, float>::ArithScalar;
};

constexpr Volume cubic_m(float m3) { return Volume(m3); }
constexpr Volume ml(float ml) { return Volume(ml / (1000.0f * 1000.0f)); }

// Represents voltage.
//
// Precision: float.
//
// Units: Volts
class Voltage : public units_detail::ArithScalar<Voltage, float> {
public:
  [[nodiscard]] constexpr float volts() const { return val_; }

private:
  constexpr friend Voltage volts(float v);

  using units_detail::ArithScalar<Voltage, float>::ArithScalar;
};

constexpr Voltage volts(float v) { return Voltage(v); }

// Time and Duration classes.
//
// - Duration represents a length of time, e.g. "10 seconds" or
//   "-2 microseconds".
//
// - Time represents a point in time, relative to when the device started up,
//   e.g. "1000 seconds after startup".
//
// Both classes have microsecond resolution.  2^63 μs is about 300,000 years,
// so no need to worry about rollover.
//
// You can add and subtract Times and Durations in the natural way.
//
//   Duration +/- Duration = Duration
//   Time +/- Duration = Time
//   Time - Time = Duration
//
class Time;

// Represents a length of time.
//
// Precision: 1ms
//
// Units:
//  - seconds
//  - milliseconds
//  - microseconds
//
// Native unit (implementation detail): int64_t microseconds
//
// Multiplying a VolumetricFlow by a Duration gives you a Volume.
// Dividing a Volume by a Duration gives you a VolumetricFlow.
//
// Note that because the factory `Duration milliseconds(int64_t)` takes an
// int64, but `float Duration::milliseconds()` must return a float, you may
// encounter an int64_t x such that
//
//   milliseconds(x).milliseconds() != x.
//
// This is unfortunate, but the alternative (not offering a
// milliseconds(int64_t) factory) is worse, because converting an int64
// milliseconds to float may lose useful precision.
class Duration : public units_detail::ArithScalar<Duration, int64_t> {
public:
  [[nodiscard]] constexpr int64_t microseconds() const { return val_; }
  [[nodiscard]] constexpr float milliseconds() const {
    return static_cast<float>(val_) / 1000;
  }
  [[nodiscard]] constexpr float seconds() const {
    return milliseconds() / 1000;
  }
  [[nodiscard]] constexpr float minutes() const { return seconds() / 60; }

  constexpr friend Time operator+(const Time &a, const Duration &b);
  constexpr friend Time operator+(const Duration &a, const Time &b);
  constexpr friend Time operator-(const Time &a, const Duration &b);
  constexpr friend Duration operator-(const Time &a, const Time &b);

private:
  constexpr friend Duration microseconds(int64_t micros);

  using units_detail::ArithScalar<Duration, int64_t>::ArithScalar;
};

constexpr Duration microseconds(int64_t micros) { return Duration(micros); }
constexpr Duration milliseconds(int64_t millis) {
  return microseconds(millis * 1000);
}

// Add a dummy template to make these overloads have lower priority than the
// int64_t version.  We need a `double` version otherwise passing a double
// (even just a double constant, like "0.1") will prefer the int64_t version!
template <int /*dummy*/ = 0> constexpr Duration milliseconds(float millis) {
  return microseconds(static_cast<int64_t>(millis * 1000));
}
template <int /*dummy*/ = 0> constexpr Duration milliseconds(double millis) {
  return microseconds(static_cast<int64_t>(millis * 1000));
}

constexpr Duration seconds(float secs) { return milliseconds(secs * 1000); }
constexpr Duration minutes(float mins) { return seconds(mins * 60); }

// Represents a point in time, relative to when the device started up.  See
// details above.
//
// Precision: 1μs
//
// Units:
//  - milliseconds
//  - microseconds
//
// Native unit (implementation detail): uint64_t microseconds
class Time : public units_detail::Scalar<Time, uint64_t> {
public:
  [[nodiscard]] uint64_t microsSinceStartup() const { return val_; }

  constexpr friend Time operator+(const Time &a, const Duration &b);
  constexpr friend Time operator+(const Duration &a, const Time &b);
  constexpr friend Time operator-(const Time &a, const Duration &b);
  constexpr friend Duration operator-(const Time &a, const Time &b);
  Time &operator+=(const Duration &dt) { return *this = *this + dt; }
  Time &operator-=(const Duration &dt) { return *this = *this - dt; }

private:
  constexpr friend Time microsSinceStartup(uint64_t micros);

  using units_detail::Scalar<Time, uint64_t>::Scalar;
};

constexpr Time microsSinceStartup(uint64_t micros) { return Time(micros); }

constexpr inline Time operator+(const Time &t, const Duration &dt) {
  return Time(t.val_ + static_cast<uint64_t>(dt.val_));
}
constexpr inline Time operator+(const Duration &dt, const Time &t) {
  return Time(t.val_ + static_cast<uint64_t>(dt.val_));
}
constexpr inline Time operator-(const Time &t, const Duration &dt) {
  return Time(t.val_ - static_cast<uint64_t>(dt.val_));
}
constexpr inline Duration operator-(const Time &a, const Time &b) {
  return Duration(static_cast<int64_t>(a.val_ - b.val_));
}

constexpr inline Volume operator*(VolumetricFlow a, Duration b) {
  return ml(a.ml_per_min() * b.minutes());
}

constexpr inline Volume operator*(Duration b, VolumetricFlow a) {
  return ml(a.ml_per_min() * b.minutes());
}

// Be aware that dividing by zero leads to Inf and needs to be protected.
constexpr inline VolumetricFlow operator/(Volume a, Duration b) {
  return ml_per_min(a.ml() / b.minutes());
}

#endif // UNITS_H
