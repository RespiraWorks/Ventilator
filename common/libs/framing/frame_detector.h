#ifndef __FRAME_DETECTOR__
#define __FRAME_DETECTOR__

#include "serial_listeners.h"
#include <stdint.h>
#include <string.h>

/**
 * FrameDetector detects frames of this format:
 * ...MbbbbbbbbbbbM...
 * Where M is frame MARK byte and b is any other byte. It is responsibility of
 * sender to ensure that MARK characters appear in the incoming stream to only
 * mark the start and the end of the frame.
 *
 * Correct stream of frames will look like this:
 * ...MbbbbbbMMbbbbbbbbMMbbbbbbbbbMMbbbbbM...
 *
 * RxBuffer is responsible for underlying reception and buffering of incoming
 * data. Also RxBuffer provides callbacks described by RxListener interface. In
 * particular:
 * OnCharacterMatch is called when MARK character is received
 * OnRxComplete is called when the requested number of bytes was received
 * OnRxError is called when an error occurs in the underlying reception
 * infrastructure
 *
 * FrameDetector is modeled as a Finite State Machine having states:
 * LOST - FrameDetector is lost in the stream of incoming bytes and will
 * interpret the next MARK byte as an end of the frame.
 * WAIT_FOR_START_MARKER - FrameDetector is synced and is waiting for MARK byte
 * that will denote the start of the frame RECEIVING_FRAME - FrameDetector is
 * synced and will interpret the next MARK byte as an end of the frame and will
 * interpret the bytes in RxBuffer as a frame.
 *
 * OnRxError will cause FrameDetector to transition to the LOST state, frame
 * being received will be ignored.
 * ...MbbbbbbEbbbbMMbbbbbbbM
 *    ^-----------^ this frame will be ignored
 *                 ^-------^ this frame will be received
 *
 * OnRxComplete should not happen in normal reception. When FrameEncoder orders
 * a reception from RxBuffer, it specifies maximum length of the frame expected,
 * also FrameEncoder restarts reception upon receiving OnCharMatch callback.
 * Thus, receiving OnRxComplete callback means that we get no MARK characters in
 * the stream because sender is sending garbage or a frame that is longer than
 * the expected length.
 *
 * FrameDetector keeps a copy of the last successfully received frame and a flag
 * denoting if the last received frame was read by the caller.
 * */

template <class RxBuffer, int FRAME_BUF_LEN>
class FrameDetector : public RxListener {
public:
  enum class State {
    LOST, // Frame detector does not know where it is in the stream of
          // bytes coming into RxBuffer
    WAIT_FOR_START_MARKER, // Frame detector is waiting for a frame start marker
    RECEIVING_FRAME        // Frame detector is receiving a frame
  };

  FrameDetector(RxBuffer &t) : rx_buffer_(t){};

  // Starts frame detector
  [[nodiscard]] bool Begin() {
    state_ = State::LOST;
    return rx_buffer_.Begin(this);
  }

  // Callback method called when RxBuffer is full
  void OnRxComplete() override {
    // We should never reach the full read of rx buffer.
    // If we get here, this means, there are no marker
    // chars in the stream, so we are lost
    error_counter_++;
    state_ = State::LOST;
    rx_buffer_.RestartRX(this);
  }

  // Callback method called when we receive a marker character
  void OnCharacterMatch() override {
    switch (state_) {
    case State::LOST:
      if (rx_buffer_.ReceivedLength() > 1) {
        // if we have received something before this marker,
        // we assume, this is the frame end marker, so wait
        // for start
        state_ = State::WAIT_FOR_START_MARKER;
      } else if (rx_buffer_.ReceivedLength() == 1) {
        // if we were lucky to get lost in the interframe silence,
        // assume this is the start of the frame
        state_ = State::RECEIVING_FRAME;
      } else {
        // TODO alert, safe reset
        // Should never end up here
        // DMA is not working?
      }
      break;
    case State::WAIT_FOR_START_MARKER:
      if (rx_buffer_.ReceivedLength() == 1) {
        state_ = State::RECEIVING_FRAME;
      } else {
        // some junk received while waiting for start marker,
        // but should have been just silence
        error_counter_++;
        state_ = State::LOST;
      }
      break;
    case State::RECEIVING_FRAME:
      // end marker received, check if we got something
      if (rx_buffer_.ReceivedLength() > 1) {
        // yes, we got data, thus we got the frame we can pass further
        processReceivedData();
        state_ = State::WAIT_FOR_START_MARKER;
      } else {
        // repeated marker char received
        // ignore it and continue receiving frame bytes
      }
      break;
    }
    rx_buffer_.RestartRX(this);
  }

  // Callback method called when underlying Rx system experiences an error
  void OnRxError([[maybe_unused]] RxError e) override {
    switch (state_) {
    case State::LOST:
    case State::WAIT_FOR_START_MARKER:
      // no change
      break;
    case State::RECEIVING_FRAME:
      state_ = State::LOST;
      break;
    }
    error_counter_++;
  }

  // Returns the last successfully detected frame, resets the frame_available
  // flag. I.e. if frame_available was returning true before this call, it will
  // return false after it
  uint8_t *TakeFrame() {
    frame_available_ = false;
    return frame_buf_;
  }

  // Returns the length of the last successfully detected frame
  uint32_t get_frame_length() { return frame_buf_length_; }

  // Returns true if a new frame was detected and this frame is available for
  // read
  bool is_frame_available() { return frame_available_; }

#ifdef TEST_MODE
  State get_state() { return state_; }
#endif

private:
  RxBuffer &rx_buffer_;
  State state_ = State::LOST;
  uint32_t error_counter_ = 0;
  bool frame_available_ = false;
  uint8_t frame_buf_[FRAME_BUF_LEN];
  uint32_t frame_buf_length_ = 0;

  void processReceivedData() {
    // we strip markers from the stream, but that does not influence the frame
    // decoder code
    frame_buf_length_ = rx_buffer_.ReceivedLength() - 1;
    memcpy(frame_buf_, rx_buffer_.get(), frame_buf_length_);
    frame_available_ = true;
  }
};

#endif
