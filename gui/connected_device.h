#ifndef CONNECTED_DEVICE_H
#define CONNECTED_DEVICE_H

#include "../common/generated_libs/network_protocol/network_protocol.pb.h"
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

// TODO: Define a subclass of ConnectedDevice that talks to a real device
// over the serial port.

#endif // CONNECTED_DEVICE_H
