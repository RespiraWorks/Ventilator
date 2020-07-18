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

#include "circular_buffer.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <optional>

// Just getting my feet wet with gtest
TEST(CircBuff, Counts) {
  CircBuff<uint8_t, 128> buff;

  // Note that a buffer with 128 bytes of storage can only actually
  // hold 127 bytes.  This is a side effect of how we represent a
  // full buffer.
  ASSERT_EQ(buff.FreeCt(), 127);
  ASSERT_EQ(buff.FullCt(), 0);

  // Add/remove bytes from the buffer and check counts along the way.
  // The full/free calculations change when the buffer wraps, so I
  // want to make sure I do this enough times to catch any errors in
  // that logic
  for (int i = 0; i < 100; i++) {
    int full = 0;
    int free = 127;
    for (int j = 0; j < 20; j++) {
      bool ok = buff.Put(0);
      ASSERT_EQ(ok, true);

      full++;
      free--;
      ASSERT_EQ(buff.FreeCt(), free);
      ASSERT_EQ(buff.FullCt(), full);
    }

    for (int j = 0; j < 20; j++) {
      std::optional<uint8_t> ch = buff.Get();
      ASSERT_EQ(*ch, 0);

      full--;
      free++;
      ASSERT_EQ(buff.FreeCt(), free);
      ASSERT_EQ(buff.FullCt(), full);
    }
  }
}

// Make sure the data we add to the buffer is the same
// as the data we get out of it
TEST(CircBuff, DataIO) {
  CircBuff<uint8_t, 128> buff;

  uint8_t TestSet[200];
  for (uint32_t i = 0; i < sizeof(TestSet); i++)
    TestSet[i] = static_cast<uint8_t>(rand());

  // Add data to the buffer until failure
  for (int i = 0; i < 128; i++) {
    bool ok = buff.Put(TestSet[i]);
    if (i < 127) {
      ASSERT_EQ(ok, true);
    }

    else {
      ASSERT_EQ(ok, false);
      break;
    }
  }

  for (int i = 0; i < 127; i++) {
    std::optional<uint8_t> x = buff.Get();
    ASSERT_EQ(*x, TestSet[i]);
  }

  // The buffer should be empty now
  ASSERT_EQ(buff.Get(), std::nullopt);
}

// TODO - some other good tests to add when there's time:
//
// - Test that when Put() and Get() fail, they have no effect
//
// - Test what happens when the buffer is empty or has size 1
//
// - Exhaustively test the state space of a small circular buffer (if it
//   works for a buffer of size 5, it probably also works for 128 - but
//   with size 5 it's easy to write a completely exhaustive test that
//   verifies that the buffer implements all operations correctly in states
//   with all combinations of (head, tail))
