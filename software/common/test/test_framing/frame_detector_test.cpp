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

#include "frame_detector.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "soft_rx_buffer.h"

static constexpr size_t BufferLength{20};
static constexpr uint8_t FrameMarker{static_cast<uint8_t>('.')};

/// Derived class to expose internal state of FrameDetector
class ExposedFrameDetector : public FrameDetector<BufferLength> {
 public:
  ExposedFrameDetector(RxBuffer *t) : FrameDetector<BufferLength>(t) {}
  State get_state() const { return state_; }
};

using State = ExposedFrameDetector::State;

// Common test fixture
class FrameDetectorTest : public testing::Test {
 protected:
  void SetUp() override { EXPECT_TRUE(frame_detector.begin()); }
  void TearDown() override {}
  virtual ~FrameDetectorTest() {}

  void fill(const std::string &data) {
    for (auto &c : data) {
      rx_buf.put_byte(c);
    }
  }

  SoftRxBuffer<BufferLength> rx_buf{FrameMarker};
  ExposedFrameDetector frame_detector{&rx_buf};
};

/* Important assumptions:
 * SoftRxBuffer will only use FrameDetector callbacks when
 *  - it encounters a FrameMaker, or
 *  - when the buffer is full
 * These assumptions are assured by SoftRxBuffer tests.
 * Therefore, here we will only test for state machine transitions under those
 * two conditions.
 */

TEST_F(FrameDetectorTest, InitialStateIsLost) {
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

TEST_F(FrameDetectorTest, StartsReceivingOnMarker) {
  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

TEST_F(FrameDetectorTest, JunkBeforeMarkerImpliesMiddleOfFrame) {
  fill("anything");
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::WaitForStartMarker, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

TEST_F(FrameDetectorTest, JunkWhileWaitForStart) {
  rx_buf.put_byte(' ');
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(FrameMarker);
  // assumed to be an end of frame, since we were in the lost state
  ASSERT_EQ(State::WaitForStartMarker, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(' ');
  ASSERT_EQ(State::WaitForStartMarker, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

TEST_F(FrameDetectorTest, RxFullSizeFrame) {
  rx_buf.put_byte(FrameMarker);
  // because we shouldn't have received data between frames
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  for (uint32_t i = 0; i < BufferLength - 2; i++) {
    rx_buf.put_byte(' ');
  }
  rx_buf.put_byte(FrameMarker);

  EXPECT_EQ(State::WaitForStartMarker, frame_detector.get_state());
  ASSERT_TRUE(frame_detector.frame_available());
  ASSERT_EQ(std::string("                  "),
            std::string(reinterpret_cast<const char *>(frame_detector.take_frame())));
}

TEST_F(FrameDetectorTest, RxComplete) {
  rx_buf.put_byte(FrameMarker);
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

TEST_F(FrameDetectorTest, RxFrameIllegalyLong) {
  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  for (uint32_t i = 0; i < BufferLength - 1; i++) {
    rx_buf.put_byte(' ');
  }
  rx_buf.put_byte(FrameMarker);

  EXPECT_EQ(State::ReceivingFrame, frame_detector.get_state());
  ASSERT_FALSE(frame_detector.frame_available());
}

TEST_F(FrameDetectorTest, ErrorWhileRx) {
  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::DMA);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::Overrun);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::SerialFraming);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  rx_buf.put_byte(FrameMarker);
  ASSERT_EQ(State::ReceivingFrame, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());

  frame_detector.on_rx_error(RxError::Unknown);
  ASSERT_EQ(State::Lost, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.frame_available());
}

template <int BUF_BufferLength>
std::vector<std::string> fakeRx(std::string frame) {
  std::vector<std::string> ret;
  SoftRxBuffer<BUF_BufferLength> rx_buf(FrameMarker);
  FrameDetector<BUF_BufferLength> frame_detector(&rx_buf);
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
      std::string s;
      auto length = static_cast<size_t>(frame_detector.frame_length());
      s.assign(reinterpret_cast<const char *>(frame_detector.take_frame()), length);
      ret.push_back(s);
    }
  }

  return ret;
}

TEST_F(FrameDetectorTest, FuzzMoreMarkers) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaa..."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("..aaa...."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("...aaa.."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("....aaa."), testing::ElementsAre("aaa"));
}

TEST_F(FrameDetectorTest, FramesTooLong) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaaaaaaaaaaaaaaaaaaa..aaa."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>(".aaaaaaaaaaaaaaaaaaa..aaa."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("....................aaa."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("...................aaa."), testing::ElementsAre("aaa"));
}

TEST_F(FrameDetectorTest, FuzzyInputs) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaa..bbb..a."), testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("..aaa..bbb..a."), testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx<BufferLength>("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx<BufferLength>(".aaa....bbb.a."), testing::ElementsAre("aaa", "bbb"));
  EXPECT_THAT(fakeRx<BufferLength>("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss..aaaaa.sa.aaaa."),
              testing::ElementsAre("bbbb", "s", "ssss", "ss", "aaaaa"));
}

TEST_F(FrameDetectorTest, FuzzyErrors) {
  EXPECT_THAT(fakeRx<BufferLength>(".aaa..bUb..a."), testing::ElementsAre("aaa", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("..aaaF.bbb..a."), testing::ElementsAre("bbb", "a"));
  EXPECT_THAT(fakeRx<BufferLength>("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx<BufferLength>("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx<BufferLength>(".aaa...Dbbb.a."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<BufferLength>("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss.Oaaaaa.sa.aaaa."),
              testing::ElementsAre("bbbb", "s", "ssss", "ss"));
}

TEST_F(FrameDetectorTest, FuzzRandomEvents) {
  std::string valid_frames = "..aa..bb..cc.";

  /* \todo make this exhaustive, rather than random, like:
int remainder = j;
for (int i = 0; i < 10; ++i) {
int r = remainder % 3;
remainder /= 3;
switch (r) { ... }
} */

  for (int j = 0; j < pow(3, 10); j++) {
    std::string noise = "";
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
