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

/// This is a template implementation file.
/// It is included by the corresponding header.

template<size_t S>
FrameDetector<S>::FrameDetector(RxBuffer& t) : rx_buffer_(&t)
{}

template<size_t S>
bool FrameDetector<S>::begin()
{
  state_ = State::Lost;
  frame_available_ = false;
  return rx_buffer_->begin(this);
}

template<size_t S>
void FrameDetector<S>::on_char_match_with_data()
{
  switch (state_)
  {
    case State::Lost:
      // We have received something more before this marker.
      // We assume this is the frame end marker, so we wait for start.
      state_ = State::WaitForStartMarker;
      restart_rx();
      return;
    case State::WaitForStartMarker:
      // Received some junk while waiting for start marker but should have been just silence.
      state_ = State::Lost;
      restart_rx();
      return;
    case State::ReceivingFrame:
      // Yes, we got data, thus we got the frame we can pass further.
      process_received_data();
      state_ = State::WaitForStartMarker;
      restart_rx();
      return;
  }
  // Switch above covers all cases.
  __builtin_unreachable();
}

template<size_t S>
void FrameDetector<S>::on_char_match_with_markers_only()
{
  switch (state_)
  {
    case State::Lost:
      // Received just this marker.
      // We were lucky to get to Lost state in the inter-frame silence,
      // so we assume this marker is the start of frame.
      state_ = State::ReceivingFrame;
      return;
    case State::WaitForStartMarker:
      // Received the marker we are waiting for.
      state_ = State::ReceivingFrame;
      return;
    case State::ReceivingFrame:
      // Received repeated marker char.
      // We have received a 0-length frame. Ignore it and continue receiving frame bytes.
      restart_rx();
      return;
  }
  // Switch above covers all cases.
  __builtin_unreachable();
}

template<size_t S>
void FrameDetector<S>::on_character_match()
{
  marker_count_++;

  if (data_received()) {
    on_char_match_with_data();
  } else if (only_markers_received()) {
    on_char_match_with_markers_only();
  } else {
    // Should never end up here
    // DMA is not working?
    // TODO alert, safe reset
    restart_rx();
  }
}

template<size_t S>
void FrameDetector<S>::on_rx_error([[maybe_unused]] RxError e)
{
  switch (state_)
  {
    case State::Lost:
    case State::WaitForStartMarker:
// no change
      break;
    case State::ReceivingFrame:
      state_ = State::Lost;
      restart_rx();
      break;
  }
}

template<size_t S>
void FrameDetector<S>::on_rx_complete()
{
// We should never reach the full read of rx buffer.
// If we get here, this means there are no marker chars in the stream, so we are lost.
  state_ = State::Lost;
  restart_rx();
}

template<size_t S>
const uint8_t* FrameDetector<S>::take_frame()
{
  frame_available_ = false;
  return frame_buffer_;
}

template<size_t S>
size_t FrameDetector<S>::frame_length() const
{ return frame_buf_length_; }

template<size_t S>
bool FrameDetector<S>::frame_available() const
{ return frame_available_; }

template<size_t S>
void FrameDetector<S>::restart_rx()
{
  marker_count_ = 0;
  rx_buffer_->restart_rx(this);
}

template<size_t S>
bool FrameDetector<S>::data_received()
{ return rx_buffer_->received_length() > marker_count_; }

template<size_t S>
bool FrameDetector<S>::only_markers_received()
{ return rx_buffer_->received_length() == marker_count_; }

template<size_t S>
void FrameDetector<S>::process_received_data()
{
  // We strip incoming markers. First marker is stripped by restarting RX after frame start MARK,
  // i.e. the first marker is not saved in RxBuffer; and now here we strip the last marker
  // (which was saved to RxBuffer) by taking the received_length() - 1
  frame_buf_length_ = rx_buffer_->received_length() - marker_count_;
  memcpy(frame_buffer_, rx_buffer_->get() + marker_count_ - 1, frame_buf_length_);
  frame_available_ = true;
}
