/* Copyright 2020-2021, RespiraWorks

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

#include "rx_buf_uart_dma.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

constexpr size_t BufferLength{20};

using BufferType = RxBufferUartDma<BufferLength>;

/// \TODO: implement actual tests

TEST(UART_DMA_RxBuffer, Placeholder) {
  UartDma uart_dma;
  BufferType buffer(uart_dma);
}
