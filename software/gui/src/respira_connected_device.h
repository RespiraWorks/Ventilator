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

#include <QDataStream>
#include <QSerialPort>
#include <memory>

#include "checksum.h"
#include "chrono.h"
#include "connected_device.h"
#include "frame_detector.h"
#include "framing.h"
#include "logger.h"
#include "network_protocol.pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "proto_traits.h"
#include "qserial_output_stream.h"
#include "soft_rx_buffer.h"

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

// In Alpha Cycle controller transmits every 30ms, but sometimes it takes
// longer, 42 being a safe bet
static constexpr DurationMs InterframeTimeout{DurationMs(42)};

// GuiStatus is about 150 bytes, resulting in 13ms tx time. Output buffer
// will usually swallow it immeadetely, but just in case we set a timeout.
static constexpr DurationMs WriteTimeout{DurationMs(15)};

// Size of the rx buffer is set asuming a corner case where EVERY GuiStatus
// byte and CRC32 will be escaped + two marker chars; this is too big, but
// safe.
static constexpr uint32_t RxFrameLengthMax{ProtoTraits<ControllerStatus>::MaxFrameSize};

static SoftRxBuffer<RxFrameLengthMax> rx_buffer_(FramingMark);
static FrameDetector<SoftRxBuffer<RxFrameLengthMax>, RxFrameLengthMax> frame_detector_(rx_buffer_);

class RespiraConnectedDevice : public ConnectedDevice {
 public:
  RespiraConnectedDevice(QString portName) : serialPortName_(portName) {}

  ~RespiraConnectedDevice() {
    if (serialPort_ != nullptr) {
      serialPort_->close();
    }
  }

  // Creates the QSerialPort in the current thread context.
  // We can't provide it from outside context because of Qt mechanisms.
  bool createPortMaybe() {
    if (serialPort_ != nullptr) return true;

    serialPort_ = std::make_unique<QSerialPort>();
    serialPort_->setPortName(serialPortName_);
    serialPort_->setBaudRate(QSerialPort::Baud115200);
    serialPort_->setDataBits(QSerialPort::Data8);
    serialPort_->setParity(QSerialPort::NoParity);
    serialPort_->setStopBits(QSerialPort::OneStop);
    serialPort_->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort_->open(QIODevice::ReadWrite)) {
      return false;
    }
    if (!frame_detector_.begin()) {
      return false;
    }
    return true;
  }

  static constexpr auto EncodeGuiStatusFrame = EncodeFrame<GuiStatus>;

  bool SendGuiStatus(const GuiStatus &gui_status) override {
    if (!createPortMaybe()) {
      CRIT("Could not open serial port for sending {}", serialPortName_.toStdString());
      // TODO Raise an Alert?
      return false;
    }

    QSerialOutputStream output_stream = QSerialOutputStream(serialPort_);
    uint32_t encoded_length = EncodeGuiStatusFrame(gui_status, output_stream);
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

  static constexpr auto DecodeControllerStatusFrame{DecodeFrame<ControllerStatus>};

  bool ReceiveControllerStatus(ControllerStatus *controller_status) override {
    qCritical() << "ReceiveControllerStatus";
    if (!createPortMaybe()) {
      CRIT("Could not open serial port for reading {}", serialPortName_.toStdString());
      // TODO Raise an Alert?
      return false;
    }

    //    wait for incomming data
    if (!serialPort_->waitForReadyRead(InterframeTimeout.count())) {
      // TODO Raise an Alert?
      CRIT("Timeout while waiting for a serial frame from Cycle Controller");
      return false;
    }

    if (serialPort_->bytesAvailable() > 0) {
      qCritical() << "Serial data available";
      QByteArray raw_data = serialPort_->readAll();
      qCritical() << "size " << raw_data.size();
      for (int i = 0; i < raw_data.size(); i++) {
        // qCritical() << i << ":" << (uint8_t)raw_data.at(i);
        rx_buffer_.put_byte(raw_data.at(i));
      }
    } else {
      qCritical() << "No bytes";
    }

    if (frame_detector_.frame_available()) {
      uint8_t *buf = frame_detector_.take_frame();
      uint32_t len = frame_detector_.frame_length();
      CRIT("Got frame");

      DecodeResult result = DecodeControllerStatusFrame(buf, len, controller_status);

      if (DecodeResult::Success != result) {
        CRIT("Could not decode received data as a frame, error code: {}", result);
        // TODO: Raise an Alert?
        return false;
      }
      CRIT("Frame decoded!!!!!");
    }

    return true;
  }

 private:
  std::unique_ptr<QSerialPort> serialPort_{nullptr};
  QString serialPortName_;
};
