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

#include "comms.h"

#include <algorithm>  // for std::min

#include "framing.h"
#include "system_timer.h"

Comms::Comms(UART::Channel *uart)
    : tx_buffer_(uart), rx_buffer_(uart), frame_detector_(&rx_buffer_) {}

void Comms::Initialize() { rx_buffer_.begin(&frame_detector_); }

void Comms::ProcessTx(const ControllerStatus &controller_status) {
  // Encode current status into tx_buffer_, and sends them over to the uart if enough time has
  // passed
  if ((last_tx_ == std::nullopt || SystemTimer::singleton().now() - *last_tx_ > TxInterval)) {
    size_t bytes_written = EncodeFrame<ControllerStatus>(controller_status, tx_buffer_);
    if (bytes_written > 0) {
      last_tx_ = SystemTimer::singleton().now();
    }
  }
  // TODO: alarm if we haven't been able to send a status for too long
}

void Comms::ProcessRx(GuiStatus *gui_status) {
  if (frame_detector_.frame_available()) {
    auto *buffer = frame_detector_.take_frame();
    size_t length = frame_detector_.frame_length();
    GuiStatus new_gui_status = GuiStatus_init_zero;
    auto result = DecodeFrame<GuiStatus>(buffer, length, &new_gui_status);
    switch (result) {
      case DecodeResult::Success:
        *gui_status = new_gui_status;
        last_rx_ = SystemTimer::singleton().now();
        break;
      case DecodeResult::ErrorCRC:
        crc_errors_ += 1;
        break;
      case DecodeResult::ErrorFraming:
        framing_errors_ += 1;
        break;
      case DecodeResult::ErrorSerialization:
        serialization_errors_ += 1;
        break;
        __builtin_unreachable();
    }
  }
  // TODO: alarm if we haven't received a status for too long
}

void Comms::Handler(const ControllerStatus &controller_status, GuiStatus *gui_status) {
  ProcessTx(controller_status);
  ProcessRx(gui_status);
}
