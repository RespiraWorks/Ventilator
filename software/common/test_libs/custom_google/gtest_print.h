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
#include <gtest/internal/gtest-port-arch.h>
#include <stdarg.h>

namespace testing {
namespace internal {
// GTestColor and ColoredPrintf has been made (private) static
// in a .cc file, we have to redefine them.
enum class GTestColor { kDefault, kRed, kGreen, kYellow };
// this is still public, use it.
extern bool ShouldUseColor(bool stdout_is_tty);
// copied code from gtest.cc
static const char* GetAnsiColorCode(GTestColor color) {
  switch (color) {
    case GTestColor::kRed:
      return "1";
    case GTestColor::kGreen:
      return "2";
    case GTestColor::kYellow:
      return "3";
    default:
      return nullptr;
  }
}

// copied code from gtest.cc
GTEST_ATTRIBUTE_PRINTF_(2, 3)
static void ColoredPrintf(GTestColor color, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

#if GTEST_OS_WINDOWS_MOBILE || GTEST_OS_ZOS || GTEST_OS_IOS || GTEST_OS_WINDOWS_PHONE || \
    GTEST_OS_WINDOWS_RT || defined(ESP_PLATFORM)
  const bool use_color = AlwaysFalse();
#else
  static const bool in_color_mode = ShouldUseColor(posix::IsATTY(posix::FileNo(stdout)) != 0);
  const bool use_color = in_color_mode && (color != GTestColor::kDefault);
#endif  // GTEST_OS_WINDOWS_MOBILE || GTEST_OS_ZOS

  if (!use_color) {
    vprintf(fmt, args);
    va_end(args);
    return;
  }

#if GTEST_OS_WINDOWS && !GTEST_OS_WINDOWS_MOBILE && !GTEST_OS_WINDOWS_PHONE && \
    !GTEST_OS_WINDOWS_RT && !GTEST_OS_WINDOWS_MINGW
  const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  // Gets the current text color.
  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
  GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
  const WORD old_color_attrs = buffer_info.wAttributes;
  const WORD new_color = GetNewColor(color, old_color_attrs);

  // We need to flush the stream buffers into the console before each
  // SetConsoleTextAttribute call lest it affect the text that is already
  // printed but has not yet reached the console.
  fflush(stdout);
  SetConsoleTextAttribute(stdout_handle, new_color);

  vprintf(fmt, args);

  fflush(stdout);
  // Restores the text color.
  SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#else
  printf("\033[0;3%sm", GetAnsiColorCode(color));
  vprintf(fmt, args);
  printf("\033[m");  // Resets the terminal to default.
#endif  // GTEST_OS_WINDOWS && !GTEST_OS_WINDOWS_MOBILE
  va_end(args);
}

}  // namespace internal
}  // namespace testing
#define PRINTF(...)                                                                           \
  do {                                                                                        \
    testing::internal::ColoredPrintf(testing::internal::GTestColor::kGreen, "[   INFO   ] "); \
    testing::internal::ColoredPrintf(testing::internal::GTestColor::kYellow, __VA_ARGS__);    \
  } while (0)

// C++ stream interface
class TestCout : public std::stringstream {
 public:
  ~TestCout() { PRINTF("%s\n", str().c_str()); }
};

#define TEST_COUT TestCout()
