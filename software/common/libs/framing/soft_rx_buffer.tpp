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

////////////////////////////////////////////////////////////////////////////////
/// WARNING: Do not include this file as header, nor compile it as a source! ///
///                                                                          ///
/// This is a template implementation file.                                  ///
/// It is included by the corresponding header.                              ///
////////////////////////////////////////////////////////////////////////////////

template<size_t RxBytesMax>
SoftRxBuffer<RxBytesMax>::SoftRxBuffer(uint8_t match_char)
    : match_char_(match_char)
{}

template<size_t RxBytesMax>
void SoftRxBuffer<RxBytesMax>::restart_rx(RxListener* listener) {
  rx_index_ = 0;
  rx_listener_ = listener;
}

template<size_t RxBytesMax>
bool SoftRxBuffer<RxBytesMax>::begin(RxListener* listener) {
  restart_rx(listener);
  return true;
};

template<size_t RxBytesMax>
size_t SoftRxBuffer<RxBytesMax>::received_length() const
{ return rx_index_; }

template<size_t RxBytesMax>
uint8_t* SoftRxBuffer<RxBytesMax>::get()
{ return rx_buffer_; }

template<size_t RxBytesMax>
void SoftRxBuffer<RxBytesMax>::put_byte(const uint8_t byte) {
  if (rx_index_ < RxBytesMax) {
    rx_buffer_[rx_index_++] = byte;
    if (byte == match_char_) {
      if (rx_listener_) {
        rx_listener_->on_character_match();
      }
    }
  }

  // cannot be `else` since rx_index_ may be incremented in above block
  if (rx_index_ >= RxBytesMax) {
    if (rx_listener_) {
      rx_listener_->on_rx_complete();
    }
  }
}
