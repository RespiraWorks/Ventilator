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

#ifndef FAN_PRESSURES_H
#define FAN_PRESSURES_H

#include "units.h"
#include <algorithm>
#include <array>
#include <utility>

// Steady-state fan pressure recorded at various fan power levels.
//
// This data comes from sample-data/2020-06-15-fan-pressure/make_cpp_table.py,
// but with some cleanups.
//
//  - Power levels below 35% are removed.  The fan is barely spinning at those
//    power levels; it doesn't generate a meaningful amount of pressure.
//
//  - Power levels in range [86%, 100%) are removed.
//
//    Above 85% our measured pressure stops increasing, because we max out our
//    pressure sensor.  So if you want more pressure than you can get at 85%,
//    this table directs you to use 100% power.  (In theory we could try
//    interpolating or something, but it's not clear that's worthwhile.)
//
inline constexpr std::array<std::pair<float, Pressure>, 40> FAN_PRESSURES{{
    {0.35f, cmH2O(1.92f)},  {0.40f, cmH2O(3.89f)},  {0.45f, cmH2O(6.13f)},
    {0.50f, cmH2O(8.68f)},  {0.51f, cmH2O(9.19f)},  {0.52f, cmH2O(9.72f)},
    {0.53f, cmH2O(10.46f)}, {0.54f, cmH2O(11.03f)}, {0.55f, cmH2O(11.57f)},
    {0.56f, cmH2O(12.07f)}, {0.57f, cmH2O(12.77f)}, {0.58f, cmH2O(13.54f)},
    {0.59f, cmH2O(14.05f)}, {0.60f, cmH2O(14.62f)}, {0.61f, cmH2O(15.16f)},
    {0.62f, cmH2O(15.88f)}, {0.63f, cmH2O(16.53f)}, {0.64f, cmH2O(17.07f)},
    {0.65f, cmH2O(17.76f)}, {0.66f, cmH2O(18.43f)}, {0.67f, cmH2O(18.88f)},
    {0.68f, cmH2O(19.56f)}, {0.69f, cmH2O(20.38f)}, {0.70f, cmH2O(21.03f)},
    {0.71f, cmH2O(21.69f)}, {0.72f, cmH2O(22.43f)}, {0.73f, cmH2O(23.24f)},
    {0.74f, cmH2O(24.05f)}, {0.75f, cmH2O(24.96f)}, {0.76f, cmH2O(26.05f)},
    {0.77f, cmH2O(26.96f)}, {0.78f, cmH2O(27.84f)}, {0.79f, cmH2O(29.15f)},
    {0.80f, cmH2O(30.37f)}, {0.81f, cmH2O(32.05f)}, {0.82f, cmH2O(33.33f)},
    {0.83f, cmH2O(35.22f)}, {0.84f, cmH2O(36.71f)}, {0.85f, cmH2O(38.42f)},
    {1.00f, cmH2O(38.75f)},
}};

// Check at compile time that the FAN_PRESSURES array is monotonic in both the
// fan speed (first element of the pair) and fan pressure (second element).
//
// This lets us binary search with confidence!
static_assert([] {
  for (int i = 1; i < FAN_PRESSURES.size(); i++) {
    auto p0 = FAN_PRESSURES[i - 1];
    auto p1 = FAN_PRESSURES[i];
    if (p0.first > p1.first || p0.second > p1.second) {
      return false;
    }
  }
  return true;
}());

// Finds the lowest fan power which achieves at least this pressure.
inline float FanPowerFor(Pressure p) {
  auto it = std::lower_bound(
      FAN_PRESSURES.begin(), FAN_PRESSURES.end(), p,
      [&](const auto &elem, const Pressure &p) { return elem.second < p; });

  if (it == FAN_PRESSURES.end()) {
    it = FAN_PRESSURES.end() - 1;
  }
  return it->first;
}

#endif // FAN_PRESSURES_H
