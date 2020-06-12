#include "frame_detector.h"
#include "proto_traits.h"
#include "soft_rx_buffer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
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

vector<string> fakeRx(string frame) {
  vector<string> ret;
  SoftRxBuffer<LEN> rx_buf(MARK);
  FrameDetectorTest frame_detector(rx_buf);
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
  EXPECT_THAT(fakeRx(".aaa..."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx("..aaa...."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx("...aaa.."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx("....aaa."), testing::ElementsAre("aaa"));
}

TEST(FrameDetector, FramesTooLong) {
  EXPECT_THAT(fakeRx(".aaaaaaaaaaaaaaaaaaaa..aaa."),
              testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx(".aaaaaaaaaaaaaaaaaaa..aaa."),
              testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx("....................aaa."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx("...................aaa."), testing::ElementsAre("aaa"));
}

TEST(FrameDetector, FuzzyInputs) {
  EXPECT_THAT(fakeRx(".aaa..bbb..a."), testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx("..aaa..bbb..a."),
              testing::ElementsAre("aaa", "bbb", "a"));
  EXPECT_THAT(fakeRx("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx(".aaa....bbb.a."), testing::ElementsAre("aaa", "bbb"));
  EXPECT_THAT(fakeRx("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss..aaaaa.sa.aaaa."),
              testing::ElementsAre("bbbb", "s", "ssss", "ss", "aaaaa"));
}

TEST(FrameDetector, FuzzyErrors) {
  EXPECT_THAT(fakeRx(".aaa..bUb..a."), testing::ElementsAre("aaa", "a"));
  EXPECT_THAT(fakeRx("..aaaF.bbb..a."), testing::ElementsAre("bbb", "a"));
  EXPECT_THAT(fakeRx("aaa.bbb.a."), testing::ElementsAre());
  EXPECT_THAT(fakeRx("aaa.....bbb.a."), testing::ElementsAre("bbb"));
  EXPECT_THAT(fakeRx(".aaa...Dbbb.a."), testing::ElementsAre("aaa"));
  EXPECT_THAT(fakeRx("aa.bbb.b...bbbb..s.ss.s...ssss..ss.ssss.Oaaaaa.sa.aaaa."),
              testing::ElementsAre("bbbb", "s", "ssss", "ss"));
}
