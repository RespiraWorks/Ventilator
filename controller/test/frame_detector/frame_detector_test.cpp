#include "frame_detector.h"
#include "proto_traits.h"
#include "soft_rx_buffer.h"
#include "gtest/gtest.h"

constexpr uint32_t LEN = 20;

using State = FrameDetector<SoftRxBuffer<LEN>, LEN>::State;

constexpr uint8_t MARK = static_cast<uint8_t>('.');

SoftRxBuffer<LEN> rx_buf('.');

TEST(FrameDetector, TrivialRx) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
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
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, JunkWhileWaitForStart) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
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
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
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
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
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
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
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

  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
  ASSERT_TRUE(frame_detector.is_frame_available());
}

void fakeRx(const char *frame) {
  for (uint32_t i = 0; i < strlen(frame); i++) {
    rx_buf.PutByte(frame[i]);
  }
}

TEST(FrameDetector, FuzzMoreMarkers) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  fakeRx(".aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());

  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
  fakeRx("..aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());

  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
  fakeRx("...aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());

  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
  fakeRx("....aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());

  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
  fakeRx(".aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
  fakeRx("..aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
  fakeRx("...aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());

  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
  fakeRx(".aaaaaaaaaaaaaaaaaaaa..aaa.");
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(State::WAIT_FOR_START_MARKER, frame_detector.get_state());
}

TEST(FrameDetector, ErrorWhileRx) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(State::LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(State::RECEIVING_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.OnRxError(RxError::TIMEOUT);
  ASSERT_EQ(State::LOST, frame_detector.get_state());
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

  frame_detector.OnRxError(RxError::OVERFLOW);
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
