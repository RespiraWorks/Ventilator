// Shorthands for the very verbose types and functions from std::chrono.
#ifndef CHRONO_H
#define CHRONO_H

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

#endif // CHRONO_H
