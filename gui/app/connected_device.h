#ifndef CONNECTED_DEVICE_H
#define CONNECTED_DEVICE_H

#include "network_protocol.pb.h"
#include <functional>

// Represents a connection to the device running the controller.
class ConnectedDevice {
public:
  virtual ~ConnectedDevice() = default;

  // Sends the GuiStatus to the controller. May block.
  virtual bool SendGuiStatus(const GuiStatus &gui_status) = 0;
  // Reads a ControllerStatus from the controller. May block.
  // TODO: Make sure both functions can't block indefinitely.
  virtual bool ReceiveControllerStatus(ControllerStatus *controller_status) = 0;
};

// A fake version of ConnectedDevice backed by a lambda for testing.
class FakeConnectedDevice : public ConnectedDevice {
public:
  FakeConnectedDevice(std::function<void(const GuiStatus &)> send_fn,
                      std::function<void(ControllerStatus *)> receive_fn)
      : send_fn_(send_fn), receive_fn_(receive_fn) {}
  ~FakeConnectedDevice() = default;

  bool SendGuiStatus(const GuiStatus &gui_status) override {
    send_fn_(gui_status);
    return true;
  }
  bool ReceiveControllerStatus(ControllerStatus *controller_status) override {
    receive_fn_(controller_status);
    return true;
  }

private:
  std::function<void(const GuiStatus &)> send_fn_;
  std::function<void(ControllerStatus *)> receive_fn_;
};

#endif // CONNECTED_DEVICE_H
