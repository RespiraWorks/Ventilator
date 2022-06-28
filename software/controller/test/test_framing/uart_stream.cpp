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

#include "uart_stream.h"

#include "gtest/gtest.h"
#include "uart_mock.h"

TEST(FramingStreamsTests, UartStreamTrivial) {
  UART::MockChannel mock_uart;
  UartStream<2> uart_stream{&mock_uart};
  StreamResponse r;
  EXPECT_EQ(uart_stream.bytes_available_for_write(), 2);

  r = uart_stream.put(0);
  EXPECT_EQ(r.Count(), 1);
  EXPECT_EQ(r.Flags(), static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(uart_stream.bytes_available_for_write(), 1);
  EXPECT_FALSE(mock_uart.TxInProgress());
  EXPECT_EQ(mock_uart.GetTxByte(), std::nullopt);

  r = uart_stream.put(1);
  EXPECT_EQ(r.Count(), 1);
  EXPECT_EQ(r.Flags(), static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(uart_stream.bytes_available_for_write(), 2);
  EXPECT_TRUE(mock_uart.TxInProgress());
  EXPECT_EQ(*mock_uart.GetTxByte(), 0);
  EXPECT_EQ(*mock_uart.GetTxByte(), 1);
  EXPECT_EQ(mock_uart.GetTxByte(), std::nullopt);
  EXPECT_FALSE(mock_uart.TxInProgress());

  // fill the uart Transmit buffer
  uint8_t ch{0};
  while (mock_uart.TxFree() > 0) {
    mock_uart.Write(&ch, 1, &uart_stream);
  }

  r = uart_stream.put(2);
  EXPECT_EQ(r.Count(), 1);
  EXPECT_EQ(r.Flags(), static_cast<uint32_t>(ResponseFlags::StreamSuccess));
  EXPECT_EQ(uart_stream.bytes_available_for_write(), 1);

  r = uart_stream.put(3);
  EXPECT_EQ(r.Count(), 1);
  EXPECT_EQ(r.Flags(), static_cast<uint32_t>(ResponseFlags::WarningBufferFull));
  EXPECT_EQ(uart_stream.bytes_available_for_write(), 0);

  r = uart_stream.put(4);
  EXPECT_EQ(r.Count(), 0);
  EXPECT_EQ(r.Flags(), static_cast<uint32_t>(ResponseFlags::ErrorBufferFull));

  // empty buffer and check stream triggered a new transmission (on tx complete)
  for (size_t i = 0; i < mock_uart.GetBufferLength(); ++i) {
    EXPECT_EQ(*mock_uart.GetTxByte(), 0);
  }

  EXPECT_TRUE(mock_uart.TxInProgress());
  EXPECT_EQ(uart_stream.bytes_available_for_write(), 2);
  EXPECT_EQ(*mock_uart.GetTxByte(), 2);
  EXPECT_EQ(*mock_uart.GetTxByte(), 3);
  EXPECT_FALSE(mock_uart.TxInProgress());
  EXPECT_EQ(mock_uart.GetTxByte(), std::nullopt);
}

TEST(FramingStreamsTests, FullStackTrivial) {
  UART::MockChannel mock_uart;
  UartStream<20> uart_stream{&mock_uart};
  EscapeStream esc_stream(uart_stream);
  CrcStream crc_stream(esc_stream);

  StreamResponse r;

  uint8_t input_data[5] = {41, 42, FramingEscape, FramingMark, 66};

  size_t i{0};
  r = crc_stream.put(input_data[i++]);
  // added a begin frame marker
  ASSERT_EQ(r.Count(), 2);
  r = crc_stream.put(input_data[i++]);
  ASSERT_EQ(r.Count(), 1);
  r = crc_stream.put(input_data[i++]);
  // added an escape character
  ASSERT_EQ(r.Count(), 2);
  r = crc_stream.put(input_data[i++]);
  // added an escape character
  ASSERT_EQ(r.Count(), 2);
  r = crc_stream.put(input_data[i++]);
  ASSERT_EQ(r.Count(), 1);
  r = crc_stream.put(EndOfStream);
  // 4 bytes crc and end marker, we assume here crc didn't require an extra escape char
  ASSERT_EQ(r.Count(), 5);
  ASSERT_TRUE(mock_uart.TxInProgress());
  uint8_t transmitted[20] = {0};
  mock_uart.GetTxData(transmitted, 20);
  ASSERT_FALSE(mock_uart.TxInProgress());
  i = 0;
  ASSERT_EQ(transmitted[i++], FramingMark);
  ASSERT_EQ(transmitted[i++], 41);
  ASSERT_EQ(transmitted[i++], 42);
  ASSERT_EQ(transmitted[i++], FramingEscape);
  ASSERT_EQ(transmitted[i++], EscapedChar(FramingEscape));
  ASSERT_EQ(transmitted[i++], FramingEscape);
  ASSERT_EQ(transmitted[i++], EscapedChar(FramingMark));
  ASSERT_EQ(transmitted[i++], 66);
  // still assuming crc wasn't escaped
  ASSERT_EQ(transmitted[i + 4], FramingMark);
}
