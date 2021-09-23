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

#pragma once

#include <cstdint>
#include <cstring>

#include "rx_buffer.h"

/**
 * FrameDetector detects frames of this format:
 * ...MbbbbbbbbbbbM...
 * Where M is frame MARK byte and b is any other byte. It is responsibility of sender to ensure that
 * MARK characters appear in the incoming stream to only mark the start and the end of the frame.
 *
 * Correct stream of frames will look like this:
 * ...MbbbbbbMMbbbbbbbbMMbbbbbbbbbMMbbbbbM...
 *
 * RxBuffer is responsible for underlying reception and buffering of incoming data. Also RxBuffer
 * provides callbacks described by RxListener interface. In particular:
 *
 *   on_character_match is called when MARK character is received
 *   on_rx_complete is called when the requested number of bytes was received
 *   on_rx_error is called when an error occurs in the underlying reception infrastructure
 *
 * FrameDetector is modeled as a Finite State Machine having states:
 *
 *   Lost - FrameDetector is lost in the stream of incoming bytes and will interpret the next MARK
 *          byte as an end of the frame.
 *   WaitForStartMarker - FrameDetector is synced and is waiting for MARK byte that will denote the
 *          start of the frame
 *   ReceivingFrame - FrameDetector is synced and will interpret the next MARK byte as an end of the
 *          frame and will interpret the bytes in RxBuffer as a frame.
 *
 * on_rx_error will cause FrameDetector to transition to the Lost state, frame being received will
 *      be ignored:
 *          ...MbbbbbbEbbbbMMbbbbbbbM
 *             ^-----------^ this frame will be ignored
 *                          ^-------^ this frame will be received
 *
 * on_rx_complete should not happen in normal reception. When FrameEncoder orders a reception from
 *      RxBuffer, it specifies maximum length of the frame expected, also FrameEncoder restarts
 *      reception upon receiving OnCharMatch callback. Thus, receiving OnRxComplete callback means
 *      that we get no MARK characters in the stream because sender is sending garbage or a frame
 *      that is longer than the expected length.
 *
 * FrameDetector keeps a copy of the last successfully received frame and a flag denoting if the
 * last received frame was read by the caller.
 * */

template <size_t FrameBufferLength>
class FrameDetector : public RxListener {
 public:
  enum class State {
    Lost,                // Frame detector does not know where it is in the stream of bytes coming
                         // into RxBuffer
    WaitForStartMarker,  // Frame detector is waiting for a frame start marker
    ReceivingFrame       // Frame detector is receiving a frame
  };

  explicit FrameDetector(RxBuffer *);

  // Starts frame detector
  [[nodiscard]] bool begin();

  void on_char_match_with_data();

  void on_char_match_with_markers_only();

  // Callback method called when RxBuffer receives a marker character;
  // Note: MARK character is already written to RxBuffer so all conditions
  // evaluating rx_buffer_.received_length() take that into account
  void on_character_match() override;

  // Callback method called when underlying Rx system experiences an error
  void on_rx_error(RxError) override;

  // Callback method called when RxBuffer is full
  void on_rx_complete() override;

  // Returns the last successfully detected frame, resets the frame_available flag.
  // If frame_available was returning true before this call, it will return false afterwards
  const uint8_t *take_frame();

  // Returns the length of the last successfully detected frame.
  size_t frame_length() const;

  // Returns true if a new frame was detected and this frame is available for read.
  bool frame_available() const;

  /// Using protected so that derived class with back-door can be used for testing
 protected:
  RxBuffer *rx_buffer_;  /// \TODO: const ptr?
  State state_{State::Lost};
  bool frame_available_{false};
  uint8_t frame_buffer_[FrameBufferLength] = {0};
  size_t frame_buf_length_{0};
  size_t marker_count_{0};

  void restart_rx();
  bool data_received();
  bool only_markers_received();
  void process_received_data();
};

#include "frame_detector.tpp"
