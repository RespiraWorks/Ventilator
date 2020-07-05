#include "chrono.h"
#include "connected_device.h"
#include "logger.h"
#include "network_protocol.pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include <QSerialPort>
#include <memory>

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
constexpr DurationMs INTER_FRAME_TIMEOUT_MS = DurationMs(42);

// GuiStatus is about 150 bytes, resulting in 13ms tx time. Output buffer
// will usually swallow it immeadetely, but just in case we set a timeout.
constexpr DurationMs WRITE_TIMEOUT_MS = DurationMs(15);

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
    if (serialPort_ != nullptr)
      return true;

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
    return true;
  }

  bool SendGuiStatus(const GuiStatus &gui_status) override {
    if (!createPortMaybe()) {
      CRIT("Could not open serial port for sending {}",
           serialPortName_.toStdString());
      // TODO Raise an Alert?
      return false;
    }

    uint8_t tx_buffer[GuiStatus_size];

    pb_ostream_t stream = pb_ostream_from_buffer(tx_buffer, sizeof(tx_buffer));
    if (!pb_encode(&stream, GuiStatus_fields, &gui_status)) {
      // TODO Raise an Alert?
      CRIT("Could not serialize GuiStatus");
      return false;
    }

    serialPort_->write((const char *)tx_buffer, stream.bytes_written);

    if (!serialPort_->waitForBytesWritten(WRITE_TIMEOUT_MS.count())) {
      // TODO Raise an Alert?
      CRIT("Timeout while sending GuiStatus");
      return false;
    }
    return true;
  }

  bool ReceiveControllerStatus(ControllerStatus *controller_status) override {
    if (!createPortMaybe()) {
      CRIT("Could not open serial port for reading {}",
           serialPortName_.toStdString());
      // TODO Raise an Alert?
      return false;
    }

    // wait for incomming data
    if (!serialPort_->waitForReadyRead(INTER_FRAME_TIMEOUT_MS.count())) {
      // TODO Raise an Alert?
      CRIT("Timeout while waiting for a serial frame from Cycle Controller");
      return false;
    }

    QByteArray responseData = serialPort_->readAll();

    // continue reading characters until we don't see
    // any data for long enough to estimate end of packet silence
    while (serialPort_->waitForReadyRead(INTER_FRAME_TIMEOUT_MS.count() / 5)) {
      responseData += serialPort_->readAll();
    }

    pb_istream_t stream = pb_istream_from_buffer(
        (const uint8_t *)responseData.data(), responseData.length());

    if (!pb_decode(&stream, ControllerStatus_fields, controller_status)) {
      CRIT("Could not de-serialize received data as Controller Status");
      // TODO: Raise an Alert?
      return false;
    }

    return true;
  }

private:
  std::unique_ptr<QSerialPort> serialPort_ = nullptr;
  QString serialPortName_;
};
