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

#include "frame_detector.h"

#include <cmath>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "proto_traits.h"
#include "soft_rx_buffer.h"

using namespace std;

constexpr size_t BufferLength{20};
constexpr uint8_t Mark{static_cast<uint8_t>('.')};

using State = FrameDetector<SoftRxBuffer<BufferLength>, BufferLength>::State;
using FrameDetectorTest = FrameDetector<SoftRxBuffer<BufferLength>, BufferLength>;

TEST(FrameDetector, MarkFirstInLost) {
  SoftRxBuffer<BufferLength> rx_buf(Mark);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.begin());
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

TEST(FrameDetector, JunkWhileWaitForStart) {
  SoftRxBuffer<BufferLength> rx_buf(Mark);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.begin());
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(' ');
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::WaitForStartMarker, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(' ');
  ASSERT_EQ(State::WaitForStartMarker, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

TEST(FrameDetector, RxFullSizeFrame) {
  SoftRxBuffer<BufferLength> rx_buf(Mark);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.begin());
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  for (uint32_t i = 0; i < BufferLength - 2; i++) {
    rx_buf.put_byte(' ');
  }
  rx_buf.put_byte(Mark);

  EXPECT_EQ(State::WaitForStartMarker, frame_detector.get_state());
  ASSERT_TRUE(frame_detector.frame_available());
  ASSERT_EQ(string("                  "),
            string(reinterpret_cast<const char *>(frame_detector.take_frame())));
}

TEST(FrameDetector, RxComplete) {
  SoftRxBuffer<BufferLength> rx_buf(Mark);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.begin());
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  for (uint32_t i = 0; i < BufferLength; i++) {
    rx_buf.put_byte(' ');
  }

  // We've put too many bytes into the rx_buffer, causing RxComplete event
  // That should not happen as frames must fit into the buffer by design
  EXPECT_EQ(State::Lost, frame_detector.get_state());
  ASSERT_FALSE(frame_detector.frame_available());
}

TEST(FrameDetector, RxFrameIllegalyLong) {
  SoftRxBuffer<BufferLength> rx_buf(Mark);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.begin());
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  for (uint32_t i = 0; i < BufferLength - 1; i++) {
    rx_buf.put_byte(' ');
  }
  rx_buf.put_byte(Mark);

  EXPECT_EQ(State::ReceivingFrame, frame_detector.get_state());
  ASSERT_FALSE(frame_detector.frame_available());
}

TEST(FrameDetector, ErrorWhileRx) {
  SoftRxBuffer<BufferLength> rx_buf(Mark);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.begin());
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::DMA);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::Overrun);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::SerialFraming);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(Mark);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::Unknown);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

template <int BUF_BufferLength>
vector<string> fakeRx(string frame) {
  vector<string> ret;
  SoftRxBuffer<BUF_BufferLength> rx_buf(Mark);
  FrameDetector<SoftRxBuffer<BUF_BufferLength>, BUF_BufferLength> frame_detector(rx_buf);
  if (!frame_detector.begin()) {
    return ret;
  }

  for (auto &c : frame) {
    switch (c) {
      case 'F':
        frame_detector.on_rx_error(RxError::SerialFraming);
        break;
      case 'U':
        frame_detector.on_rx_error(RxError::Unknown);
        break;
      case 'D':
        frame_detector.on_rx_error(RxError::DMA);
        break;
      case 'O':
        frame_detector.on_rx_error(RxError::Overrun);
        break;
      default:
        rx_buf.put_byte(c);
    }

    if (frame_detector.frame_available()) {
      string s;
      auto length = static_cast<size_t>(frame_detector.frame_length());
      s.assign(reinterpret_cast<const char *>(frame_detector.take_frame()), length);
      ret.push_back(s);
    }
  }

  return ret;
}

TEST(FrameDetector, FuzzMoreMarkers) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaa..."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("..aaa...."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("...aaa.."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("....aaa."), testing::ElementsAre("aaa"));
}

TEST(FrameDetector, FramesTooLong) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaaaaaaaaaaaaaaaaaaa..aaa."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>(".aaaaaaaaaaaaaaaaaaa..aaa."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("....................aaa."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("...................aaa."), testing::ElementsAre("aaa"));
}

TEST(FrameDetector, FuzzyInputs) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaa..bbb..a."), testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("..aaa..bbb..a."), testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx<BufferLength>("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx<BufferLength>(".aaa....bbb.a."), testing::ElementsAre("aaa", "bbb"));
  EXPECT_THAT(fakeRx<BufferLength>("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss..aaaaa.sa.aaaa."),
              testing::ElementsAre("bbbb", "s", "ssss", "ss", "aaaaa"));
}

TEST(FrameDetector, FuzzyErrors) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaa..bUb..a."), testing::ElementsAre("aaa", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("..aaaF.bbb..a."), testing::ElementsAre("bbb", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx<BufferLength>("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx<BufferLength>(".aaa...Dbbb.a."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss.Oaaaaa.sa.aaaa."),
              testing::ElementsAre("bbbb", "s", "ssss", "ss"));
}

TEST(FrameDetector, FuzzRandomEvents) {
  string valid_frames = "..aa..bb..cc.";

  /* \todo make this exhaustive, rather than random, like:
int remainder = j;
for (int i = 0; i < 10; ++i) {
int r = remainder % 3;
remainder /= 3;
switch (r) { ... }
} */

  for (int j = 0; j < pow(3, 10); j++) {
    string noise = "";
    for (int i = 0; i < 10; i++) {
      int r = rand() % static_cast<int>(3);
      switch (r) {
        case 0:
          noise += ".";
          break;
        case 1:
          noise += "a";
          break;
        case 2:
          // \todo: does frame detector handle frames differently? If not we just
          // need "E"?
          noise += "F";
          break;
        default:
          noise += "b";
      }
    }
    EXPECT_THAT(fakeRx<4>(noise + valid_frames), testing::IsSupersetOf({"aa", "bb", "cc"}));
  }
}
