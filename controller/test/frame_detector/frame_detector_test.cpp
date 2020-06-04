#include "frame_detector.h"
#include "proto_traits.h"
#include "soft_rx_buffer.h"
#include "gtest/gtest.h"

constexpr uint32_t LEN = 20;

constexpr FrameDetector<SoftRxBuffer<LEN>, LEN>::State STATE_LOST =
    FrameDetector<SoftRxBuffer<LEN>, LEN>::State::LOST;
constexpr FrameDetector<SoftRxBuffer<LEN>, LEN>::State STATE_WAIT_START =
    FrameDetector<SoftRxBuffer<LEN>, LEN>::State::WAIT_START;
constexpr FrameDetector<SoftRxBuffer<LEN>, LEN>::State STATE_RX_FRAME =
    FrameDetector<SoftRxBuffer<LEN>, LEN>::State::RX_FRAME;

constexpr uint8_t MARK = static_cast<uint8_t>('.');

SoftRxBuffer<LEN> rx_buf('.');

TEST(FrameDetector, TrivialRx) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(' ');
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_WAIT_START, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(' ');
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_WAIT_START, frame_detector.get_state());
  EXPECT_TRUE(frame_detector.is_frame_available());
  EXPECT_EQ(static_cast<uint32_t>(1), frame_detector.get_frame_length());
  uint8_t *frame = frame_detector.TakeFrame();
  EXPECT_EQ(0, memcmp(frame, " ", 1));
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, MarkFirstInLost) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, JunkWhileWaitForStart) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(' ');
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_WAIT_START, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(' ');
  ASSERT_EQ(STATE_WAIT_START, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, RxComplete) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.onRxError(RxError_t::RX_ERROR_DMA);
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}

TEST(FrameDetector, ErrorWhileRx) {
  FrameDetector<SoftRxBuffer<LEN>, LEN> frame_detector(rx_buf);
  EXPECT_TRUE(frame_detector.Begin());
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.onRxError(RxError_t::RX_ERROR_TIMEOUT);
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.onRxError(RxError_t::RX_ERROR_DMA);
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.onRxError(RxError_t::RX_ERROR_OVR);
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.onRxError(RxError_t::RX_ERROR_SERIAL_FRAMING);
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  rx_buf.PutByte(MARK);
  ASSERT_EQ(STATE_RX_FRAME, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());

  frame_detector.onRxError(RxError_t::RX_ERROR_UNKNOWN);
  ASSERT_EQ(STATE_LOST, frame_detector.get_state());
  EXPECT_FALSE(frame_detector.is_frame_available());
}
