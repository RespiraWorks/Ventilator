#pragma once

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
 * Lost - FrameDetector is lost in the stream of incoming bytes and will
 * interpret the next MARK byte as an end of the frame.
 * WaitForStartMarker - FrameDetector is synced and is waiting for MARK byte
 * that will denote the start of the frame
 * ReceivingFrame - FrameDetector is synced and will interpret the next MARK
 * byte as an end of the frame and will interpret the bytes in RxBuffer as a
 * frame.
 *
 * OnRxError will cause FrameDetector to transition to the Lost state, frame
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

template <class RxBuffer, int FrameBufferLength>
class FrameDetector : public RxListener {
public:
  enum class State {
    Lost, // Frame detector does not know where it is in the stream of
          // bytes coming into RxBuffer
    WaitForStartMarker, // Frame detector is waiting for a frame start marker
    ReceivingFrame      // Frame detector is receiving a frame
  };

  explicit FrameDetector(RxBuffer &t) : rx_buffer_(t){};

  // Starts frame detector
  [[nodiscard]] bool Begin() {
    state_ = State::Lost;
    frame_available_ = false;
    return rx_buffer_.Begin(this);
  }

  void OnCharMatchWithData() {
    switch (state_) {
    case State::Lost:
      // we have received something more before this marker,
      // we assume, this is the frame end marker, so wait
      // for start
      state_ = State::WaitForStartMarker;
      RestartRX();
      break;
    case State::WaitForStartMarker:
      // some junk received while waiting for the start marker,
      // but should have been just silence
      state_ = State::Lost;
      RestartRX();
      break;
    case State::ReceivingFrame:
      // yes, we got data, thus we got the frame we can pass further
      ProcessReceivedData();
      state_ = State::WaitForStartMarker;
      RestartRX();
      break;
    default:
      state_ = State::Lost;
      RestartRX();
      break;
    }
  }

  void OnCharMatchMarkersOnly() {
    switch (state_) {
    case State::Lost:
      // We have received just this marker
      // In this case, we were lucky to get to Lost state in the interframe
      // silence, assume this marker is the start of the frame
      state_ = State::ReceivingFrame;
      break;
    case State::WaitForStartMarker:
      // we have received the marker we are waiting for
      state_ = State::ReceivingFrame;
      break;
    case State::ReceivingFrame:
      // repeated marker char received
      // this means we have received a 0 lenght frame
      // ignore it and continue receiving frame bytes
      RestartRX();
      break;
    default:
      state_ = State::Lost;
      RestartRX();
      break;
    }
  }

  // Callback method called when RxBuffer receives a marker character;
  // Note, MARK character is already written to RxBuffer so all conditions
  // evaluating rx_buffer_.ReceivedLength() take that into account
  void OnCharacterMatch() override {
    marker_count_++;

    if (IsDataReceived()) {
      OnCharMatchWithData();
    } else if (IsOnlyMarkersReceived()) {
      OnCharMatchMarkersOnly();
    } else {
      // Should never end up here
      // DMA is not working?
      // TODO alert, safe reset
      RestartRX();
    }
  }

  // Callback method called when underlying Rx system experiences an error
  void OnRxError([[maybe_unused]] RxError e) override {
    switch (state_) {
    case State::Lost:
    case State::WaitForStartMarker:
      // no change
      break;
    case State::ReceivingFrame:
      state_ = State::Lost;
      RestartRX();
      break;
    }
  }

  // Callback method called when RxBuffer is full
  void OnRxComplete() override {
    // We should never reach the full read of rx buffer.
    // If we get here, this means, there are no marker
    // chars in the stream, so we are lost
    state_ = State::Lost;
    RestartRX();
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
  State state_ = State::Lost;
  bool frame_available_ = false;
  uint8_t frame_buf_[FrameBufferLength];
  uint32_t frame_buf_length_ = 0;
  uint32_t marker_count_ = 0;

  void RestartRX() {
    marker_count_ = 0;
    rx_buffer_.RestartRX(this);
  }

  bool IsDataReceived() { return rx_buffer_.ReceivedLength() > marker_count_; }
  bool IsOnlyMarkersReceived() {
    return rx_buffer_.ReceivedLength() == marker_count_;
  }
  void ProcessReceivedData() {
    // We strip incoming markers. First marker is stripped by restarting RX
    // after frame start MARK, i.e. the first marker is not saved in RxBuffer;
    // and now here we strip the last marker (which was saved to RxBuffer) by
    // taking the ReceivedLength() - 1
    frame_buf_length_ = rx_buffer_.ReceivedLength() - marker_count_;
    memcpy(frame_buf_, rx_buffer_.get() + marker_count_ - 1, frame_buf_length_);
    frame_available_ = true;
  }
};
