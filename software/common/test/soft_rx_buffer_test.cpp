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

#include "soft_rx_buffer.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

constexpr size_t BufferLength{20};
constexpr uint8_t FrameMarker{static_cast<uint8_t>('.')};

using BufferType = SoftRxBuffer<BufferLength>;

/// \TODO: implement actual tests

TEST(SoftRxBuffer, Placeholder) {
  BufferType buffer(FrameMarker);
  EXPECT_EQ(buffer.received_length(), 0);
}
