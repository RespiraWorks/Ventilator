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

#include <QSerialPort>
#include <memory>

#include "chrono.h"
#include "connected_device.h"
#include "logger.h"
#include "network_protocol.pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"

// Connects to system serial port, does nanopb serialization/deserialization
// of GuiStatus and ControllerStatus and provides methods to send/receive
// these opbejcts over serial port.

// NOTE: Both SendGuiStatus and ReceiveControllerStatus are blocking.

// NOTE: it is important to run both SendGuiStatus and ReceiveControllerStatus
// from the same thread because they share the same QSerialPort. We can't
// create two QSerialPorts for each of them separately as both of them
// would try to lock the system serial port resource, one will fail.
// This has to be researched a bit more as we specify if we are opening
// QSerialPort for reading or for writing or both, thus it might work to open
// QIODevice::Read in Send thread and QIODevice::Write in Receive thread

class RespiraConnectedDevice : public ConnectedDevice {
 public:
  RespiraConnectedDevice(QString portName);

  ~RespiraConnectedDevice();

  // Creates the QSerialPort in the current thread context.
  // We can't provide it from outside context because of Qt mechanisms.
  bool createPortMaybe();

  bool SendGuiStatus(const GuiStatus &gui_status) override;

  bool ReceiveControllerStatus(ControllerStatus *controller_status) override;

 private:
  std::unique_ptr<QSerialPort> serialPort_ = nullptr;
  QString serialPortName_;
};
