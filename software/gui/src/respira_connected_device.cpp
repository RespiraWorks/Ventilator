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

#include "respira_connected_device.h"

#include "framing.h"
#include "qserial_output_stream.h"

// In Alpha Cycle controller transmits every 30ms, but sometimes it takes
// longer, 42 being a safe bet
static constexpr DurationMs InterframeTimeout = DurationMs(42);

// GuiStatus is about 150 bytes, resulting in 13ms tx time. Output buffer
// will usually swallow it immeadetely, but just in case we set a timeout.
static constexpr DurationMs WriteTimeout = DurationMs(15);

RespiraConnectedDevice::RespiraConnectedDevice(QString portName) : serialPortName_(portName) {}

RespiraConnectedDevice::~RespiraConnectedDevice() {
  if (serialPort_ != nullptr) {
    serialPort_->close();
  }
}

// Creates the QSerialPort in the current thread context.
// We can't provide it from outside context because of Qt mechanisms.
bool RespiraConnectedDevice::createPortMaybe() {
  if (serialPort_ != nullptr) return true;

  serialPort_ = std::make_unique<QSerialPort>();
  serialPort_->setPortName(serialPortName_);
  serialPort_->setBaudRate(QSerialPort::Baud115200);
  serialPort_->setDataBits(QSerialPort::Data8);
  serialPort_->setParity(QSerialPort::NoParity);
  serialPort_->setStopBits(QSerialPort::OneStop);
  serialPort_->setFlowControl(QSerialPort::NoFlowControl);

  if (!serialPort_->open(QIODevice::ReadWrite)) return false;

  if (!frame_detector_.begin()) return false;

  return true;
}

bool RespiraConnectedDevice::SendGuiStatus(const GuiStatus &gui_status) {
  if (!createPortMaybe()) {
    CRIT("Could not open serial port for sending {}", serialPortName_.toStdString());
    // TODO Raise an Alert?
    return false;
  }

  QSerialOutputStream output_stream{serialPort_};
  uint32_t encoded_length = EncodeFrame<GuiStatus>(gui_status, output_stream);
  if (0 == encoded_length) {
    // TODO Raise an Alert?
    CRIT("Could not frame serialized GuiStatus");
    return false;
  }

  if (!serialPort_->waitForBytesWritten(WriteTimeout.count())) {
    // TODO Raise an Alert?
    CRIT("Timeout while sending GuiStatus");
    return false;
  }
  return true;
}

bool RespiraConnectedDevice::ReceiveControllerStatus(ControllerStatus *controller_status) {
  if (!createPortMaybe()) {
    CRIT("Could not open serial port for reading {}", serialPortName_.toStdString());
    // TODO Raise an Alert?
    return false;
  }

  // wait for incomming data
  if (!serialPort_->waitForReadyRead(InterframeTimeout.count())) {
    // TODO Raise an Alert?
    CRIT("Timeout while waiting for a serial frame from Cycle Controller");
    return false;
  }

  if (serialPort_->bytesAvailable() > 0) {
    QByteArray raw_data = serialPort_->readAll();
    for (int i = 0; i < raw_data.size(); i++) {
      rx_buffer_.put_byte(raw_data.at(i));
    }
  }

  if (frame_detector_.frame_available()) {
    uint8_t *buffer = frame_detector_.take_frame();
    uint32_t length = frame_detector_.frame_length();
    DecodeResult result = DecodeFrame<ControllerStatus>(buffer, length, controller_status);

    if (DecodeResult::Success != result) {
      CRIT("Could not decode received data as a frame, error code: {}",
           static_cast<uint32_t>(result));
      // TODO: Raise an Alert?
      return false;
    }
  }

  return true;
}
