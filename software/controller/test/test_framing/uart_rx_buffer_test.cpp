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

#include "uart_rx_buffer.h"

#include "frame_detector.h"
#include "framing_spec_chars.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "uart_mock.h"

constexpr size_t BufferLength{20};

using BufferType = UartRxBuffer<BufferLength>;

TEST(UART_RxBuffer, Trivial) {
  UART::MockChannel uart(FramingMark);
  BufferType buffer(&uart);

  uart.PutRxByte(25);
  ASSERT_EQ(buffer.received_length(), 1);
  ASSERT_EQ(*buffer.get(), 25);

  uart.PutRxByte(25);
  buffer.restart_rx(nullptr);
  ASSERT_EQ(buffer.received_length(), 0);
}

TEST(UART_RxBuffer, WithRxListener) {
  UART::MockChannel uart(FramingMark);
  BufferType buffer(&uart);
  FrameDetector<BufferLength> frame_detector(&buffer);

  buffer.begin(&frame_detector);
  uart.PutRxByte(FramingMark);
  uart.PutRxByte(42);
  // being the first byte, character match has no consequence on buffer
  ASSERT_EQ(buffer.received_length(), 2);
  uart.PutRxByte(FramingMark);
  // character match restarted the rx_buffer
  ASSERT_EQ(buffer.received_length(), 0);
  // frame detector now has the contents of the buffer, stripped of the frame markers
  ASSERT_EQ(*frame_detector.take_frame(), 42);
}
