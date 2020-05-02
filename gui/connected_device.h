#ifndef CONNECTED_DEVICE_H
#define CONNECTED_DEVICE_H

#include "../common/generated_libs/network_protocol/network_protocol.pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include <functional>

// Represents a connection to the device running the controller.
class ConnectedDevice {
public:
  virtual ~ConnectedDevice() = default;

  // Sends the GuiStatus to the controller. May block.
  virtual void SendGuiStatus(const GuiStatus &gui_status) = 0;
  // Reads a ControllerStatus from the controller. May block.
  // TODO: Make sure both functions can't block indefinitely.
  virtual void ReceiveControllerStatus(ControllerStatus *controller_status) = 0;
};

// A fake version of ConnectedDevice backed by a lambda for testing.
class FakeConnectedDevice : public ConnectedDevice {
public:
  FakeConnectedDevice(std::function<void(const GuiStatus &)> send_fn,
                      std::function<void(ControllerStatus *)> receive_fn)
      : send_fn_(send_fn), receive_fn_(receive_fn) {}
  ~FakeConnectedDevice() = default;

  void SendGuiStatus(const GuiStatus &gui_status) override {
    send_fn_(gui_status);
  }
  void ReceiveControllerStatus(ControllerStatus *controller_status) override {
    receive_fn_(controller_status);
  }

private:
  std::function<void(const GuiStatus &)> send_fn_;
  std::function<void(ControllerStatus *)> receive_fn_;
};

#include <QSerialPort>

// TODO: Define a subclass of ConnectedDevice that talks to a real device
// over the serial port.

class RespiraConnectedDevice : public ConnectedDevice {

  RespiraConnectedDevice(QString portName, uint32_t timeout_ms)
      : m_serial(new QSerialPort()), m_waitTimeout(timeout_ms) {
    m_serial->setPortName(portName);
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_serial.open(QIODevice::ReadWrite)
  }

public:
  void SendGuiStatus(const GuiStatus &gui_status) override {

    uint8_t tx_buffer[GuiStatus_size];
    pb_ostream_t stream = pb_ostream_from_buffer(tx_buffer, sizeof(tx_buffer));
    if (!pb_encode(&stream, GuiStatus_fields, &gui_status)) {
      // TODO: Serialization failure; log an error or raise an alert.
      return;
    }

    serial->write(tx_buffer, stream.bytes_written);

    if (!serial->waitForBytesWritten(m_waitTimeout)) {
      // TODO communication failure, port closed? Log an error and raise an
      // alert
      return;
    }
  }

  void ReceiveControllerStatus(ControllerStatus *controller_status) override {
    // Drop everything we have in buffer right now as we are not sure if this is
    // a full frame or a part of it.
    while (m_serial->waitForReadyRead(interframe_time_ms)) {
      m_serial->clear(QSerialPort::Input);
    }

    if (serial.waitForReadyRead(frame_timeout)) {
      QByteArray responseData = serial.readAll();
      // we rely that in some characters will come in during
      // `inter_character_timeout` but this is fragile as probably OS can delay
      // some characters occasionally
      while (serial.waitForReadyRead(inter_character_timeout)) {
        responseData += serial.readAll();
      }

      pb_istream_t stream =
          pb_istream_from_buffer(responseData.data(), responseData.length());
      if (!pb_decode(&stream, ControllerStatus_fields, controller_status)) {
        // TODO: Log an error.
      }
    } else {
      // TODO frame from CycleController is not on schedule, raise an alert
      return;
    }
  }

private:
  QSerialPort *m_serial = nullptr;
  uint32_t m_waitTimeout;
}

#endif // CONNECTED_DEVICE_H
