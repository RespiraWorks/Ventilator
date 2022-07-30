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

#include "commands.h"
#include "comms.h"
#include "controller.h"
#include "high_priority_trigger.h"
#include "interface.h"
#include "network_protocol.pb.h"
#include "trace.h"
#include "v03system.h"
#include "vars.h"

/// \TODO make this a template class that expects some implementation of AbstractSystem
class MainContainer {
 public:
  static void high_priority_callback(void *instance);

  void init();
  void high_priority_task();
  void background_task();
  static SensorsProto AsSensorsProto(const SensorReadings &r, const ControllerState &c);

  System hardware_layer;
  Controller controller;
  // Current controller status, updated when we receive data from the GUI, sensors read data, etc.
  ControllerStatus controller_status = ControllerStatus_init_zero;
  // Last-received status from the GUI.
  GuiStatus gui_status = GuiStatus_init_zero;
  std::optional<Comms> comms;
  std::optional<Debug::Interface> debug;
  Debug::Trace trace;
  HighPriorityTrigger high_priority_trigger;

  Debug::Command::ModeHandler mode_command;
  Debug::Command::PeekHandler peek_command;
  Debug::Command::PokeHandler poke_command;
  Debug::Command::VarHandler var_command;
  std::optional<Debug::Command::TraceHandler> trace_command;
  std::optional<Debug::Command::EepromHandler> eeprom_command;

  using DUint32 = Debug::Variable::UInt32;
  using DFloat = Debug::Variable::Float;
  using DAccess = Debug::Variable::Access;

  // By default, the controller receives settings (on/off, pip, rr, etc.) from the GUI.  But you
  // can also command the controller by setting the forced_foo variables below.
  DUint32 forced_mode{"forced_mode",
                      DAccess::ReadWrite,
                      _VentMode_MAX + 1,
                      "",
                      "Overrides ventilation mode as commanded by GUI; see VentMode enum in "
                      "network_protocol.proto. If out of range, this and all of the other gui_foo "
                      "variables are ignored.",
                      "%s"};
  DUint32 forced_breath_rate{"forced_breath_rate", DAccess::ReadWrite, 15, "breaths/min",
                             "Target breath rate; overrides GUI setting when forced_mode is valid"};
  DUint32 forced_peep{"forced_peep", DAccess::ReadWrite, 5, "cmH2O",
                      "Target PEEP; overrides GUI setting when forced_mode is valid"};
  DUint32 forced_pip{"forced_pip", DAccess::ReadWrite, 15, "cmH2O",
                     "Target PIP; overrides GUI setting when forced_mode is valid"};
  DFloat forced_ie_ratio{"forced_ie_ratio", DAccess::ReadWrite, 0.66f, "ratio",
                         "Target I:E ratio; overrides GUI setting when forced_mode is valid"};
  DFloat forced_fio2{"forced_fio2", DAccess::ReadWrite, 21, "%",
                     "Target percent oxygen [21, 100]; overrides GUI setting when forced_mode is "
                     "valid"};
};
