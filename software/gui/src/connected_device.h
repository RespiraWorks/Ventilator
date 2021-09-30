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

#include <functional>

#include "network_protocol.pb.h"

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
