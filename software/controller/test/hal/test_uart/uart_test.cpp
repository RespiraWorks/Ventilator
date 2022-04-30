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
#include "gtest_main.h"
#include "uart_mock.h"

TEST(UartTest, Init) {
  UART::MockChannel test_uart;
  ASSERT_EQ(test_uart.RxFull(), 0);
  ASSERT_EQ(test_uart.TxFree(), test_uart.GetBufferLength());
}

TEST(UartTest, Rx) {
  UART::MockChannel test_uart;

  uint8_t read_buffer[test_uart.GetBufferLength()];

  // keep this as a multiple of 2
  size_t bytes_added{50};
  // put bytes in buffer
  for (uint8_t i = static_cast<uint8_t>(bytes_added); i > 0; --i) {
    test_uart.PutRxByte(i);
  }
  ASSERT_EQ(test_uart.RxFull(), bytes_added);
  // retrieve half of those
  ASSERT_EQ(test_uart.Read(read_buffer, bytes_added / 2), bytes_added / 2);
  // the other half remains
  ASSERT_EQ(test_uart.RxFull(), bytes_added / 2);
  for (uint8_t i = 0; i < 25; ++i) {
    ASSERT_EQ(read_buffer[i], bytes_added - i);
  }
  // retrieve the other half through longer Read
  ASSERT_EQ(test_uart.Read(read_buffer, bytes_added), bytes_added / 2);
  // buffer is now empty
  ASSERT_EQ(test_uart.RxFull(), 0);
  for (uint8_t i = 0; i < 25; ++i) {
    ASSERT_EQ(read_buffer[i], bytes_added / 2 - i);
  }
  ASSERT_EQ(test_uart.Read(read_buffer, bytes_added), 0);
}

TEST(UartTest, Tx) {
  UART::MockChannel test_uart;

  // create a buffer slightly bigger than the tx buffer
  size_t buffer_size{test_uart.GetBufferLength() + 1};
  uint8_t send_buffer[buffer_size];
  for (size_t i = 0; i < buffer_size; ++i) {
    send_buffer[i] = static_cast<uint8_t>(i);
  }

  // reasonably sized write
  size_t bytes_added{50};
  ASSERT_EQ(test_uart.Write(send_buffer, bytes_added), bytes_added);
  ASSERT_TRUE(test_uart.TxInProgress());
  ASSERT_EQ(test_uart.TxFree(), test_uart.GetBufferLength() - bytes_added);
  for (uint8_t i = 0; i < bytes_added; ++i) {
    ASSERT_EQ(*test_uart.GetTxByte(), i);
  }

  // we gathered all bytes from the Tx buffer, Tx is no longer in progress
  ASSERT_FALSE(test_uart.TxInProgress());

  // buffer should be empty again
  ASSERT_EQ(test_uart.TxFree(), test_uart.GetBufferLength());
  // fill buffer through long write
  ASSERT_EQ(test_uart.Write(send_buffer, buffer_size), test_uart.GetBufferLength());
  // check that we cannot write to buffer anymore
  ASSERT_EQ(test_uart.Write(send_buffer, 1), 0);
  // check contents of buffer
  for (size_t i = 0; i < test_uart.GetBufferLength(); ++i)
    ASSERT_EQ(*test_uart.GetTxByte(), static_cast<uint8_t>(i));
}
