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

#pragma once

#include <cstdint>
#include <optional>

#include "frame_detector.h"
#include "network_protocol.pb.h"
#include "proto_traits.h"
#include "uart_dma.h"
#include "uart_rx_buffer.h"
#include "uart_stream.h"
#include "vars.h"

/// \TODO Either template the Comms class and/or have it take a truly abstract Channel. This class
///   only requires that UartRxBuffer accept it (see what functions it wants) HAL expects it to have
///   UARTInterruptHandler, TxDMAInterruptHandler, RxDMAInterruptHandler

// This module periodically sends messages to the GUI device and receives
// messages from the GUI.  The only way it communicates with other modules is
// by modifying the gui_status pointer in CommsHandler.
class Comms {
 public:
  explicit Comms(UART::Channel *uart);

  void Initialize();

  // `controller_status` should be the controller's current status.  It's sent
  // periodically to the GUI.  When we receive a message from the GUI, we update
  // gui_status accordingly.
  void Handler(const ControllerStatus &controller_status, GuiStatus *gui_status);

 private:
  // Our outgoing (framed) ControllerStatus proto is stored in tx_buffer.
  UartStream<ProtoTraits<ControllerStatus>::MaxFrameSize> tx_buffer_;
  std::optional<Time> last_tx_{std::nullopt};

  // Our incoming (framed) GuiStatus proto is buffered in rx_buffer.
  static constexpr size_t RxBufferSize{ProtoTraits<GuiStatus>::MaxFrameSize};
  UartRxBuffer<RxBufferSize> rx_buffer_;
  Time last_rx_{microsSinceStartup(0)};
  FrameDetector<RxBufferSize> frame_detector_;

  void ProcessTx(const ControllerStatus &controller_status);
  void ProcessRx(GuiStatus *gui_status);

  static constexpr Duration TxInterval{milliseconds(30)};

  // Introduce counters on some conditions to allow checking that everything is going according
  // to plan.
  Debug::Variable::UInt32 crc_errors_{"comms_crc_errors", Debug::Variable::Access::ReadOnly, 0, "",
                                      "Counter of checksum errors when decoding gui status"};
  Debug::Variable::UInt32 framing_errors_{"comms_framing_errors", Debug::Variable::Access::ReadOnly,
                                          0, "",
                                          "Counter of framing errors when decoding gui status"};
  Debug::Variable::UInt32 serialization_errors_{
      "comms_serialize_errors", Debug::Variable::Access::ReadOnly, 0, "",
      "Counter of proto serialization errors when decoding gui status"};

  // These allow us to read the frame detectors counters from frame_detector_.
  Debug::Variable::Primitive32 received_frames_{
      "comms_received_frames", Debug::Variable::Access::ReadOnly, &frame_detector_.received_frames,
      "", "Counter of frames received from the gui"};
  Debug::Variable::Primitive32 lost_frames_{"comms_lost_frames", Debug::Variable::Access::ReadOnly,
                                            &frame_detector_.lost_frames, "",
                                            "Counter of frames lost in comms with the gui"};
  Debug::Variable::Primitive32 empty_frames_{
      "comms_empty_frames", Debug::Variable::Access::ReadOnly, &frame_detector_.empty_frames, "",
      "Counter of empty frames received from the gui (should stay 0)"};
  Debug::Variable::Primitive32 overflowed_buffer_{
      "comms_overflowed_buffer_", Debug::Variable::Access::ReadOnly,
      &frame_detector_.overflowed_buffer, "",
      "Counter of frame detector buffer overflow errors (should stay 0)"};
  Debug::Variable::Primitive32 unknown_errors_{
      "comms_unknown_errors", Debug::Variable::Access::ReadOnly, &frame_detector_.unknown_errors,
      "", "Counter of unknown errors when decoding gui status"};
};
