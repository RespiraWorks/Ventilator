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
//   - Voltage (volts)
//   - Elapsed time since startup (ms)
//   - Duration, aka time interval (ms)
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
//   Time             millisSinceStartup(int64_t)
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

// Represents a Scalar where:
// - addition and subtraction are well-defined
// - multiplication by an external factor of type ValTy is defined
// This is what mathematicians call a "module":
// https://en.wikipedia.org/wiki/Module_(mathematics)
// Note that multiplication by an external factor requires multiplication to be
// well defined on ValTy type.
// Be aware that dividing by 0.0f yields Inf and needs to be protected.
template <class Q, class ValTy> class ArithScalar : public Scalar<Q, ValTy> {
public:
  constexpr Q operator+(const ArithScalar &a) { return Q(this->val_ + a.val_); }
  constexpr Q operator-(const ArithScalar &a) { return Q(this->val_ - a.val_); }
  inline ArithScalar &operator+=(const ArithScalar &a) {
    return *this = *this + a;
  }
  inline ArithScalar &operator-=(const ArithScalar &a) {
    return *this = *this - a;
  }
  constexpr Q operator*(const ValTy &a) { return Q(this->val_ * a); }
  constexpr Q operator/(const ValTy &a) { return Q(this->val_ / a); }
  inline ArithScalar &operator*=(const ValTy &a) { return *this = *this * a; }
  inline ArithScalar &operator/=(const ValTy &a) { return *this = *this / a; }
  constexpr friend Q operator*(const ValTy &a, const Q &b) {
    return Q(b.val_ * a);
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
//
// Native unit (implementation detail): kPa
//
// Note that multiplying and dividing pressure by a float makes little sense,
// as they are not an absolute value.
// Division may be used to define an average.
class Pressure : public units_detail::ArithScalar<Pressure, float> {
public:
  [[nodiscard]] constexpr float kPa() const { return val_; }
  [[nodiscard]] constexpr float cmH2O() const { return val_ * CM_H2O_PER_KPA; }

private:
  // https://www.google.com/search?q=kpa+to+cmh2o
  static inline constexpr float CM_H2O_PER_KPA = 10.1972f;

  constexpr friend Pressure kPa(float kpa);
  constexpr friend Pressure cmH2O(float cm_h2o);

  using units_detail::ArithScalar<Pressure, float>::ArithScalar;
};

constexpr Pressure kPa(float kpa) { return Pressure(kpa); }
constexpr Pressure cmH2O(float cm_h2o) {
  return Pressure(cm_h2o / Pressure::CM_H2O_PER_KPA);
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
//
// Native unit (implementation detail): meters^3/sec
//
// Dividing a Volume (see below) by a Duration gives you a VolumetricFlow
// Multiplying a VolumetricFlow by a Duration (see below) gives you a Volume
class VolumetricFlow : public units_detail::ArithScalar<VolumetricFlow, float> {
public:
  [[nodiscard]] constexpr float cubic_m_per_sec() { return val_; }
  [[nodiscard]] constexpr float ml_per_min() {
    return val_ * 1000.0f * 1000.0f * 60.0f;
  }
  [[nodiscard]] constexpr float liters_per_sec() { return val_ * 1000.0f; }

private:
  constexpr friend VolumetricFlow cubic_m_per_sec(float m3ps);
  constexpr friend VolumetricFlow ml_per_min(float ml_per_min);
  constexpr friend VolumetricFlow liters_per_sec(float lps);

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
// Multiplying a VolumetricFlow by a Duration (see below) gives you a Volume
// Dividing a Volume by a Duration gives you a VolumetricFlow
class Volume : public units_detail::ArithScalar<Volume, float> {
public:
  [[nodiscard]] constexpr float cubic_m() { return val_; }
  [[nodiscard]] constexpr float ml() { return val_ * 1000.0f * 1000.0f; }

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
// - Duration represents a length of time, e.g. "10 seconds" or "-2
//   milliseconds".
//
// - Time represents a point in time, relative to when the device started up,
//   e.g. "1000 seconds after startup".
//
// Both classes have millisecond resolution.
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
//
// Native unit (implementation detail): int64_t miliseconds
//
// Multiplying a VolumetricFlow by a Duration gives you a Volume
// Dividing a Volume by a Duration gives you a VolumetricFlow
class Duration : public units_detail::Scalar<Duration, int64_t> {
public:
  [[nodiscard]] constexpr int64_t milliseconds() const { return val_; }
  [[nodiscard]] constexpr float seconds() const {
    return static_cast<float>(val_) / 1000;
  }
  [[nodiscard]] constexpr float minutes() const { return seconds() / 60; }

  constexpr friend Duration operator+(const Duration &a, const Duration &b);
  constexpr friend Duration operator-(const Duration &a, const Duration &b);
  constexpr friend Time operator+(const Time &a, const Duration &b);
  constexpr friend Time operator+(const Duration &a, const Time &b);
  constexpr friend Time operator-(const Time &a, const Duration &b);
  constexpr friend Duration operator-(const Time &a, const Time &b);

private:
  constexpr friend Duration milliseconds(int64_t millis);
  constexpr friend Duration seconds(float secs);

  using units_detail::Scalar<Duration, int64_t>::Scalar;
};

constexpr Duration milliseconds(int64_t millis) { return Duration(millis); }
constexpr Duration seconds(float secs) {
  return Duration(static_cast<int64_t>(1000 * secs));
}
constexpr Duration minutes(float mins) { return seconds(mins * 60); }
constexpr Duration operator*(int n, Duration d) {
  return milliseconds(static_cast<int64_t>(n) * d.milliseconds());
}
constexpr Duration operator*(Duration d, int n) {
  return milliseconds(static_cast<int64_t>(n) * d.milliseconds());
}

// Represents a point in time, relative to when the device started up.  See
// details above.
//
// Precision: 1ms
//
// Units:
//  - milliseconds
//
// Native unit (implementation detail): uint64_t miliseconds
class Time : public units_detail::Scalar<Time, uint64_t> {
public:
  [[nodiscard]] uint64_t millisSinceStartup() const { return val_; }

  constexpr friend Time operator+(const Time &a, const Duration &b);
  constexpr friend Time operator+(const Duration &a, const Time &b);
  constexpr friend Time operator-(const Time &a, const Duration &b);
  constexpr friend Duration operator-(const Time &a, const Time &b);
  inline Time &operator+=(const Duration &dt) { return *this = *this + dt; }
  inline Time &operator-=(const Duration &dt) { return *this = *this - dt; }

private:
  constexpr friend Time millisSinceStartup(uint64_t millis);

  using units_detail::Scalar<Time, uint64_t>::Scalar;
};

constexpr Time millisSinceStartup(uint64_t millis) { return Time(millis); }

constexpr inline Duration operator+(const Duration &a, const Duration &b) {
  return Duration(a.val_ + b.val_);
}
constexpr inline Duration operator-(const Duration &a, const Duration &b) {
  return Duration(a.val_ - b.val_);
}
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

// Be aware that dividing by zero leads to Inf and needs to be protected
constexpr inline VolumetricFlow operator/(Volume a, Duration b) {
  return ml_per_min(a.ml() / b.minutes());
}

#endif // UNITS_H
