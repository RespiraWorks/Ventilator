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

#pragma once

// Shorthands for the very verbose types and functions from std::chrono.

#include <chrono>

using DurationMs = std::chrono::milliseconds;
using SteadyClock = std::chrono::steady_clock;
using SteadyInstant = std::chrono::time_point<SteadyClock>;

// Given two time points a, b, computes a - b as a DurationMs.
// The name is clunky but unambiguous as to which is subtracted
// from which.
template <typename TimePointT>
DurationMs TimeAMinusB(TimePointT a, TimePointT b) {
  return std::chrono::duration_cast<DurationMs>(a - b);
}
