#include "frame_detector.h"
#include "proto_traits.h"
#include "soft_rx_buffer.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>
using namespace std;

constexpr uint32_t LEN = 20;

using State = FrameDetector<SoftRxBuffer<LEN>, LEN>::State;
using FrameDetectorTest = FrameDetector<SoftRxBuffer<LEN>, LEN>;
constexpr uint8_t MARK = static_cast<uint8_t>('.');

SoftRxBuffer<LEN> rx_buf('.');

TEST(FrameDetector, TrivialRx) {
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

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(' ');
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(static_cast<uint32_t>(1), frame_detector.get_frame_length());
  uint8_t *frame = frame_detector.TakeFrame();
  EXPECT_EQ(0, memcmp(frame, " ", 1));
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, MarkFirstInLost) {
  FrameDetectorTest frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, JunkWhileWaitForStart) {
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

  frame_detector.OnRxError(RxError::Overrun);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.OnRxError(RxError::SerialFraming);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.OnRxError(RxError::Unknown);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

vector<string> fakeRx(const char *frame) {
  vector<string> ret;
  FrameDetectorTest frame_detector(rx_buf);
  if (!frame_detector.Begin()) {
    return ret;
  }

  for (uint32_t i = 0; i < strlen(frame); i++) {
    rx_buf.PutByte(frame[i]);
    if (frame_detector.is_frame_available()) {
      string s;
      s.assign(reinterpret_cast<char *>(frame_detector.TakeFrame()),
               static_cast<size_t>(frame_detector.get_frame_length()));
      ret.push_back(s);
    }
  }
  return ret;
}

TEST(FrameDetector, FuzzMoreMarkers) {

  vector<string> results = fakeRx(".aaa.");
  EXPECT_EQ(static_cast<unsigned int>(1), results.size());
  EXPECT_EQ(0, results[0].compare("aaa"));

  results = fakeRx("..aaa.");
  ASSERT_EQ(static_cast<unsigned int>(1), results.size());
  EXPECT_EQ(0, results[0].compare("aaa"));

  results = fakeRx("..aaa.");
  EXPECT_EQ(static_cast<unsigned int>(1), results.size());
  EXPECT_EQ(0, results[0].compare("aaa"));

  results = fakeRx("...aaa.");
  EXPECT_EQ(static_cast<unsigned int>(1), results.size());
  EXPECT_EQ(0, results[0].compare("aaa"));

  results = fakeRx(".aaaaaaaaaaaaaaaaaaaa..aaa.");
  EXPECT_EQ(static_cast<unsigned int>(1), results.size());
  EXPECT_EQ(0, results[0].compare("aaa"));

  results = fakeRx(".......................aaa.");
  EXPECT_EQ(static_cast<unsigned int>(1), results.size());
  EXPECT_EQ(0, results[0].compare("aaa"));
}
