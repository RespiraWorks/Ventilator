/* Copyright 2020-2022, RespiraWorks
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

#include <cstdlib>
#include <optional>

#include "circular_buffer.h"
#include "gtest_main.h"

// Test the buffer counts and rollover
TEST(CircularBuffer, Counts) {
  constexpr size_t BufferSize = 128;
  CircularBuffer<uint8_t, BufferSize> buff;

  ASSERT_EQ(buff.FreeCount(), BufferSize);
  ASSERT_EQ(buff.FullCount(), 0);
  ASSERT_EQ(buff.ContiguousDataCount(), 0);
  ASSERT_EQ(buff.ContiguousFreeCount(), BufferSize);

  // Add/remove bytes from the buffer and check counts along the way.
  // The full/free calculations change when the buffer wraps, so I
  // want to make sure I do this enough times to catch any errors in
  // that logic
  for (int i = 0; i < 100; i++) {
    int full = 0;
    int free = BufferSize;
    for (int j = 0; j < 20; j++) {
      // By putting j in the buffer, I can then check that it stays a FIFO
      // even when it wraps around
      bool ok = buff.Put(static_cast<uint8_t>(j));
      ASSERT_EQ(ok, true);

      full++;
      free--;
      ASSERT_EQ(buff.FreeCount(), free);
      ASSERT_EQ(buff.FullCount(), full);
    }

    for (int j = 0; j < 20; j++) {
      std::optional<uint8_t> ch = buff.Get();
      ASSERT_EQ(*ch, static_cast<uint8_t>(j));

      full--;
      free++;
      ASSERT_EQ(buff.FreeCount(), free);
      ASSERT_EQ(buff.FullCount(), full);
    }
  }
}

// Make sure the data we add to the buffer is the same
// as the data we get out of it
TEST(CircularBuffer, DataIO) {
  constexpr size_t BufferSize = 256;
  CircularBuffer<uint8_t, BufferSize> buff;

  uint8_t TestSet[BufferSize + 20];
  for (uint32_t i = 0; i < sizeof(TestSet); i++) TestSet[i] = static_cast<uint8_t>(rand());

  // Add data to the buffer until failure - multiple failures should not affect
  // our test results: the buffer's content stays unchanged during failure to
  // put more data
  for (int i = 0; i < BufferSize + 20; i++) {
    bool ok = buff.Put(TestSet[i]);
    if (i < BufferSize) {
      ASSERT_EQ(ok, true);
      ASSERT_EQ(buff.FullCount(), i + 1);
      ASSERT_EQ(buff.FreeCount(), BufferSize - 1 - i);
      ASSERT_EQ(buff.FullCount(), buff.ContiguousDataCount());
      ASSERT_EQ(buff.FreeCount(), buff.ContiguousFreeCount());
    } else {
      ASSERT_EQ(ok, false);
      ASSERT_EQ(buff.FullCount(), BufferSize);
      ASSERT_EQ(buff.FreeCount(), 0);
      ASSERT_EQ(buff.FullCount(), buff.ContiguousDataCount());
      ASSERT_EQ(buff.FreeCount(), buff.ContiguousFreeCount());
      break;
    }
  }

  // Retrieve all the data that was succesfully put in the buffer and check its
  // integrity
  for (int i = 0; i < BufferSize; i++) {
    std::optional<uint8_t> x = buff.Get();
    ASSERT_EQ(*x, TestSet[i]);
  }

  // The buffer should be empty now
  ASSERT_EQ(buff.FullCount(), 0);
  ASSERT_EQ(buff.FreeCount(), BufferSize);
  ASSERT_EQ(buff.Get(), std::nullopt);
  // failure to Get() has not changed anything
  ASSERT_EQ(buff.FullCount(), 0);
  ASSERT_EQ(buff.FreeCount(), BufferSize);
  ASSERT_EQ(buff.Get(), std::nullopt);
}

// Test that the buffer of minimal size (uint N = 0) is always empty
// This has no practical use but this way we make sure the template is safe.
TEST(CircularBuffer, Size0) {
  CircularBuffer<size_t, 0> buff;
  ASSERT_EQ(buff.FullCount(), 0);
  ASSERT_EQ(buff.FreeCount(), 0);
  ASSERT_EQ(buff.Put(0), false);
  ASSERT_EQ(buff.Get(), std::nullopt);
  ASSERT_EQ(buff.GetTailAddress(), nullptr);
  ASSERT_EQ(buff.ContiguousDataCount(), 0);
  ASSERT_EQ(buff.GetHeadAddress(), nullptr);
  ASSERT_EQ(buff.ContiguousFreeCount(), 0);
}

TEST(CircularBuffer, SmallBufferFullTest) {
  constexpr size_t BufferSize = 6;
  CircularBuffer<uint8_t, BufferSize> buff;

  ASSERT_EQ(buff.FullCount(), 0);
  ASSERT_EQ(buff.FreeCount(), BufferSize);
  ASSERT_EQ(buff.Get(), std::nullopt);
  ASSERT_EQ(buff.ContiguousDataCount(), 0);
  ASSERT_EQ(buff.ContiguousFreeCount(), BufferSize);

  volatile uint8_t *head = buff.GetHeadAddress();
  // Put a first element in the buffer.  This allows the next part to be just
  // one big for loop.
  uint8_t elem = 0;
  ASSERT_TRUE(buff.Put(elem++));
  ASSERT_EQ(*head, elem - 1);
  ASSERT_EQ(buff.FullCount(), 1);
  ASSERT_EQ(buff.FreeCount(), BufferSize - 1);
  ASSERT_EQ(buff.ContiguousDataCount(), 1);
  ASSERT_EQ(buff.ContiguousFreeCount(), BufferSize - 1);

  // note that we set max_contiguous_data to BufferSize + 1, which is not entirely
  // accurate but allows taking into account the existence of buffer[head_] (though it
  // is effectively inaccessible).
  size_t max_contiguous_data{BufferSize + 1};
  // all free space is contiguous at this point
  size_t contiguous_free_count{buff.FreeCount()};
  // keep track of whether the buffer is wrapping around, which means that when we pop
  // something, we need to increment contiguous_free_count.
  uint8_t buffer_wrapped{0};
  bool tail_at_zero{true};

  // This loop tests that for all configurations of a non-empty buffer,
  // we can put and get stuff as we want, and that stuff is properly handled.
  for (int i = 0; i < BufferSize + 1; i++) {
    // Fill the buffer and check the counts while we are at it.  Note there is
    // already one elem in buffer so we only add BufferSize - 1 new elements.
    for (int j = 1; j < BufferSize; j++) {
      head = buff.GetHeadAddress();
      ASSERT_EQ(buff.Put(elem++), true);
      ASSERT_EQ(*head, elem - 1);
      // maintain contiguous_free_count when adding data
      if (--contiguous_free_count == 0) {
        // either the buffer is full, or head was at the end of the buffer
        // and just went back to 0, either way, all free space is contiguous.
        contiguous_free_count = buff.FreeCount();
        if (contiguous_free_count == 0 && max_contiguous_data != BufferSize + 1) {
          // buffer is full and tail is not at 0, meaning the buffer wraps around
          buffer_wrapped = 1;
        }
      }
      ASSERT_EQ(buff.FullCount(), j + 1);
      ASSERT_EQ(buff.FreeCount(), BufferSize - j - 1);
      ASSERT_EQ(buff.ContiguousDataCount(), j < max_contiguous_data ? j + 1 : max_contiguous_data);
      ASSERT_EQ(buff.ContiguousFreeCount(), contiguous_free_count);
    }
    // Buffer must now be full, we can't put any more data in.
    ASSERT_EQ(buff.Put(0), false);
    ASSERT_EQ(buff.GetHeadAddress(), nullptr);
    // Pop all but one element and check the counts.
    // Because we leave 1 element, the next loop will iterate
    // with a different tail location: mod(tail - 2, BufferSize + 1).
    for (int j = 1; j < BufferSize; j++) {
      uint8_t by_address = *buff.GetTailAddress();
      std::optional<uint8_t> poped = buff.Get();

      // maintain max_contiguous_data and contiguous_free_count when pop'ing elements
      if (tail_at_zero) {
        // We just pop'ed the first element, we can now use buffer[BufferSize]
        contiguous_free_count++;
        tail_at_zero = false;
      }
      contiguous_free_count += buffer_wrapped;
      if (--max_contiguous_data == 0) {
        // we just pop'ed the element at the end of the buffer, tail is back at 0,
        // and the buffer no longer wraps around
        max_contiguous_data = BufferSize + 1;
        buffer_wrapped = 0;
        tail_at_zero = true;
      }

      ASSERT_EQ(by_address, *poped);
      ASSERT_EQ(*poped, static_cast<uint8_t>(elem - static_cast<uint8_t>(BufferSize + 1 - j)));
      ASSERT_EQ(buff.FullCount(), BufferSize - j);
      ASSERT_EQ(buff.FreeCount(), j);
      ASSERT_EQ(buff.ContiguousDataCount(),
                BufferSize - j < max_contiguous_data ? BufferSize - j : max_contiguous_data);
      ASSERT_EQ(buff.ContiguousFreeCount(), contiguous_free_count);
    }
  }

  // Flush the buffer and go back to head=tail=0.
  buff.Flush();
  ASSERT_EQ(buff.FullCount(), 0);
  ASSERT_EQ(buff.FreeCount(), BufferSize);
  ASSERT_EQ(buff.Get(), std::nullopt);
  ASSERT_EQ(buff.GetTailAddress(), nullptr);
  ASSERT_EQ(buff.ContiguousDataCount(), 0);
  ASSERT_EQ(buff.ContiguousFreeCount(), BufferSize);

  // Fill in the gap left by the original loop to include empty buffers.
  for (int i = 0; i < BufferSize + 1; i++) {
    ASSERT_EQ(buff.Put(elem), true);
    ASSERT_EQ(buff.FullCount(), 1);
    ASSERT_EQ(buff.FreeCount(), BufferSize - 1);
    // Buffer with one element is always contiguous
    ASSERT_EQ(buff.ContiguousDataCount(), 1);
    ASSERT_EQ(buff.ContiguousFreeCount(), BufferSize - (i == 0 || i == 6 ? 1 : i));

    std::optional<uint8_t> ch = buff.Get();
    ASSERT_EQ(*ch, elem++);
    ASSERT_EQ(buff.FullCount(), 0);
    ASSERT_EQ(buff.FreeCount(), BufferSize);
    ASSERT_EQ(buff.GetTailAddress(), nullptr);
    ASSERT_EQ(buff.ContiguousDataCount(), 0);
    SCOPED_TRACE(std::to_string(i));
    ASSERT_EQ(buff.ContiguousFreeCount(), BufferSize - (i == 6 ? 0 : i));

    // Trying to get an element from an empty buffer doesn't change anything,
    // whatever the head/tail
    ASSERT_EQ(buff.Get(), std::nullopt);
    ASSERT_EQ(buff.FullCount(), 0);
    ASSERT_EQ(buff.FreeCount(), BufferSize);
    ASSERT_EQ(buff.GetTailAddress(), nullptr);
    ASSERT_EQ(buff.ContiguousDataCount(), 0);
    ASSERT_EQ(buff.ContiguousFreeCount(), BufferSize - (i == 6 ? 0 : i));
  }
}
