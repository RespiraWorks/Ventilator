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

#include "soft_rx_buffer.h"

#include "gtest/gtest.h"

constexpr size_t BufferLength{20};
constexpr uint8_t FrameMarker{static_cast<uint8_t>('.')};

using BufferType = SoftRxBuffer<BufferLength>;

class MockListener : public RxListener {
 public:
  void on_rx_complete() override { completes++; }
  void on_character_match() override { matches++; }
  void on_rx_error(RxError) override {}
  size_t matches{0};
  size_t completes{0};
};

TEST(SoftRxBuffer, InitializesEmpty) {
  BufferType buffer(FrameMarker);
  EXPECT_EQ(buffer.received_length(), 0);
}

TEST(SoftRxBuffer, FillsUpToLimit) {
  BufferType buffer(FrameMarker);
  for (size_t i = 0; i < BufferLength; ++i) {
    buffer.put_byte('x');
    EXPECT_EQ(buffer.received_length(), i + 1);
  }
  EXPECT_EQ(buffer.received_length(), BufferLength);
  // Beyond this point, putting more bytes is impossible
  buffer.put_byte('x');
  EXPECT_EQ(buffer.received_length(), BufferLength);
  buffer.put_byte('x');
  EXPECT_EQ(buffer.received_length(), BufferLength);
}

TEST(SoftRxBuffer, FillsDataInSequence) {
  BufferType buffer(FrameMarker);
  buffer.put_byte('a');
  EXPECT_EQ(buffer.get()[0], 'a');
  buffer.put_byte('b');
  EXPECT_EQ(buffer.get()[1], 'b');
  buffer.put_byte('c');
  EXPECT_EQ(buffer.get()[2], 'c');
}

TEST(SoftRxBuffer, RestartOverwitesBuffer) {
  BufferType buffer(FrameMarker);
  buffer.put_byte('a');
  buffer.put_byte('b');
  buffer.put_byte('c');
  EXPECT_EQ(buffer.received_length(), 3);
  buffer.restart_rx(nullptr);
  EXPECT_EQ(buffer.received_length(), 0);
  buffer.put_byte('x');
  buffer.put_byte('y');
  buffer.put_byte('z');
  EXPECT_EQ(buffer.get()[0], 'x');
  EXPECT_EQ(buffer.get()[1], 'y');
  EXPECT_EQ(buffer.get()[2], 'z');
}

TEST(SoftRxBuffer, MatchCallbackWorks) {
  BufferType buffer(FrameMarker);
  MockListener listener;
  buffer.begin(&listener);
  buffer.put_byte('a');
  buffer.put_byte('b');
  buffer.put_byte('c');
  EXPECT_EQ(listener.matches, 0);
  buffer.put_byte(FrameMarker);
  EXPECT_EQ(listener.matches, 1);
  buffer.put_byte('x');
  buffer.put_byte('y');
  buffer.put_byte('z');
  EXPECT_EQ(listener.matches, 1);
  buffer.put_byte(FrameMarker);
  EXPECT_EQ(listener.matches, 2);

  // But not after it's full
  while (buffer.received_length() < BufferLength) {
    buffer.put_byte('x');
  }
  EXPECT_EQ(listener.matches, 2);
  buffer.put_byte(FrameMarker);
  buffer.put_byte(FrameMarker);
  buffer.put_byte(FrameMarker);
  EXPECT_EQ(listener.matches, 2);
}

TEST(SoftRxBuffer, RxCompleteCallbackWorks) {
  BufferType buffer(FrameMarker);
  MockListener listener;
  buffer.begin(&listener);
  while (buffer.received_length() < (BufferLength - 1)) {
    buffer.put_byte('x');
  }
  EXPECT_EQ(listener.completes, 0);
  buffer.put_byte('x');
  EXPECT_EQ(listener.completes, 1);

  // And keeps calling...
  buffer.put_byte('x');
  EXPECT_EQ(listener.completes, 2);

  // ...until cleared
  buffer.restart_rx(&listener);
  buffer.put_byte('x');
  EXPECT_EQ(listener.completes, 2);
}
