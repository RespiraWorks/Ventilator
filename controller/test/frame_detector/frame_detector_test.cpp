#include "frame_detector.h"
#include "proto_traits.h"
#include "soft_rx_buffer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cmath>
#include <string>
#include <vector>

using namespace std;

constexpr uint32_t LEN = 20;

using State = FrameDetector<SoftRxBuffer<LEN>, LEN>::State;
using FrameDetectorTest = FrameDetector<SoftRxBuffer<LEN>, LEN>;
constexpr uint8_t MARK = static_cast<uint8_t>('.');

TEST(FrameDetector, MarkFirstInLost) {
  SoftRxBuffer<LEN> rx_buf(MARK);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, JunkWhileWaitForStart) {
  SoftRxBuffer<LEN> rx_buf(MARK);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(' ');
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(' ');
  ASSERT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, RxFullSizeFrame) {
  SoftRxBuffer<LEN> rx_buf(MARK);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  for (uint32_t i = 0; i < LEN - 2; i++) {
    rx_buf.PutByte(' ');
  }
  rx_buf.PutByte(MARK);

  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
  ASSERT_TRUE(frame_detector.is_frame_available());
  ASSERT_EQ(string("                  "),
            string(reinterpret_cast<char *>(frame_detector.TakeFrame())));
}

TEST(FrameDetector, RxComplete) {
  SoftRxBuffer<LEN> rx_buf(MARK);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  for (uint32_t i = 0; i < LEN; i++) {
    rx_buf.PutByte(' ');
  }

  // We've put too many bytes into the rx_buffer, causing RxComplete event
  // That should not happen as frames must fit into the buffer by design
  EXPECT_EQ(State::LOST, frame_detector.get_state());
  ASSERT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, RxFrameIllegalyLong) {
  SoftRxBuffer<LEN> rx_buf(MARK);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  for (uint32_t i = 0; i < LEN - 1; i++) {
    rx_buf.PutByte(' ');
  }
  rx_buf.PutByte(MARK);

  EXPECT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  ASSERT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, ErrorWhileRx) {
  SoftRxBuffer<LEN> rx_buf(MARK);
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.OnRxError(RxError::DMA);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.OnRxError(RxError::OVERRUN);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.OnRxError(RxError::SERIAL_FRAMING);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.OnRxError(RxError::UNKNOWN);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

template <int BUF_LEN> vector<string> fakeRx(string frame) {
  vector<string> ret;
  SoftRxBuffer<BUF_LEN> rx_buf(MARK);
  FrameDetector<SoftRxBuffer<BUF_LEN>, BUF_LEN> frame_detector(rx_buf);
  if (!frame_detector.Begin()) {
    return ret;
  }

  for (auto &c : frame) {
    switch (c) {
    case 'F':
      frame_detector.OnRxError(RxError::SERIAL_FRAMING);
      break;
    case 'U':
      frame_detector.OnRxError(RxError::UNKNOWN);
      break;
    case 'D':
      frame_detector.OnRxError(RxError::DMA);
      break;
    case 'O':
      frame_detector.OnRxError(RxError::OVERRUN);
      break;
    default:
      rx_buf.PutByte(c);
    }

    if (frame_detector.is_frame_available()) {
      string s;
      auto length = static_cast<size_t>(frame_detector.get_frame_length());
      s.assign(reinterpret_cast<char *>(frame_detector.TakeFrame()), length);
      ret.push_back(s);
    }
  }

  return ret;
}

TEST(FrameDetector, FuzzMoreMarkers) {
  EXPECT_THAT(fakeRx<LEN>(".aaa..."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<LEN>("..aaa...."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<LEN>("...aaa.."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<LEN>("....aaa."), testing::ElementsAre("aaa"));
}

TEST(FrameDetector, FramesTooLong) {
  EXPECT_THAT(fakeRx<LEN>(".aaaaaaaaaaaaaaaaaaaa..aaa."),
              testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<LEN>(".aaaaaaaaaaaaaaaaaaa..aaa."),
              testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<LEN>("....................aaa."),
              testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx<LEN>("...................aaa."),
              testing::ElementsAre("aaa"));
}

TEST(FrameDetector, FuzzyInputs) {
  EXPECT_THAT(fakeRx<LEN>(".aaa..bbb..a."),
              testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx<LEN>("..aaa..bbb..a."),
              testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx<LEN>("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx<LEN>("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx<LEN>(".aaa....bbb.a."),
              testing::ElementsAre("aaa", "bbb"));
  EXPECT_THAT(
      fakeRx<LEN>("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss..aaaaa.sa.aaaa."),
      testing::ElementsAre("bbbb", "s", "ssss", "ss", "aaaaa"));
}

TEST(FrameDetector, FuzzyErrors) {
  EXPECT_THAT(fakeRx<LEN>(".aaa..bUb..a."), testing::ElementsAre("aaa", "a"));
  EXPECT_THAT(fakeRx<LEN>("..aaaF.bbb..a."), testing::ElementsAre("bbb", "a"));
  EXPECT_THAT(fakeRx<LEN>("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx<LEN>("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx<LEN>(".aaa...Dbbb.a."), testing::ElementsAre("aaa"));
  EXPECT_THAT(
      fakeRx<LEN>("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss.Oaaaaa.sa.aaaa."),
      testing::ElementsAre("bbbb", "s", "ssss", "ss"));
}

TEST(FrameDetector, FuzzRandomEvents) {
  string valid_frames = "..aa..bb..cc.";

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
        noise += "F";
        break;
      default:
        noise += "b";
      }
    }
    EXPECT_THAT(fakeRx<4>(noise + valid_frames),
                testing::IsSupersetOf({"aa", "bb", "cc"}));
  }
}
