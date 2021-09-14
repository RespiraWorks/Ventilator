/* Copyright 2021, RespiraWorks

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

#include <gtest/gtest.h>

namespace testing {
namespace internal {
enum GTestColor { COLOR_DEFAULT, COLOR_RED, COLOR_GREEN, COLOR_YELLOW };

extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
}  // namespace internal
}  // namespace testing
#define PRINTF(...)                                                                    \
  do {                                                                                 \
    testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); \
    testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__);    \
  } while (0)

// C++ stream interface
class TestCout : public std::stringstream {
 public:
  ~TestCout() { PRINTF("%s\n", str().c_str()); }
};

#define TEST_COUT TestCout()
