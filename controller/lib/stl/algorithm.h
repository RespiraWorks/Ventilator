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
#ifndef ALGORITHM_H
#define ALGORITHM_H

// This file is a "Polyfill" for some functions in <algorithm>, which is
// unavailable on Arduino.

// Undefine evil min/max macros defined by Arduino.h.  These are prone to the
// following bug:
//
//   int a = min(b, fn());
//   assert(a <= b);
//
// You'd expect this assert to always be true, but it might fail!  The min
// macro expands to
//
//   int a = b < fn() ? b : fn();
//
// and you can see that this might evaluate fn() twice.  There's no guarantee
// that the second return is equal to the first.
//
// Use stl::min/max instead.
#ifndef TEST_MODE
#include <Arduino.h>
#endif

#undef min
#undef max

namespace stl {

template <typename T> constexpr const T &min(const T &a, const T &b) {
  return a < b ? a : b;
}

template <typename T> constexpr const T &max(const T &a, const T &b) {
  return a > b ? a : b;
}

} // namespace stl
#endif // ALGORITHM_H
