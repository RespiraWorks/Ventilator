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

#include "actuators.h"
#include "commands.h"
#include "comms.h"
#include "controller.h"
#include "eeprom.h"
#include "hal.h"
#include "interface.h"
#include "network_protocol.pb.h"
#include "nvparams.h"
#include "sensors.h"
#include "system_timer.h"
#include "trace.h"
#include "version.h"
#include "watchdog.h"

using DUint32 = Debug::Variable::UInt32;
using DFloat = Debug::Variable::Float;
using DAccess = Debug::Variable::Access;

static DEBUG_STRING(dbg_version, "0_controller_version", DAccess::ReadOnly, Version::GitVersion,
                    "controller version at compile time: git describe --tags");
static DEBUG_STRING(dbg_git_branch, "0_controller_branch", DAccess::ReadOnly, Version::GitBranch,
                    "controller built from git branch");
/// \TODO: implement Bool type and use that?
static DUint32 dbg_git_dirty("0_controller_git_dirty", DAccess::ReadOnly, Version::GitDirty, "bool",
                             "Was code dirty (had uncommitted changes) at time of building?", "%s");
static DEBUG_STRING(dbg_pio_env, "0_controller_pio_env", DAccess::ReadOnly, Version::PioEnv,
                    "platformio environment used for building");
static DEBUG_STRING(dbg_build_time, "0_controller_build_time", DAccess::ReadOnly,
                    Version::BuildTime, "UTC timestamp at time of controller build");

// By default, the controller receives settings (on/off, pip, rr, etc.) from
// the GUI.  But you can also command the controller by setting the gui_foo
// DebugVars below.
static DUint32 forced_mode(
    "forced_mode", DAccess::ReadWrite, _VentMode_MAX + 1, "",
    "Overrides ventilation mode as commanded by GUI; see VentMode enum in network_protocol.proto."
    " If out of range, this and all of the other gui_foo variables are ignored.",
    "%s");
static DUint32 forced_breath_rate(
    "forced_breath_rate", DAccess::ReadWrite, 15, "breaths/min",
    "Target breath rate; overrides GUI setting when forced_mode is valid");
static DUint32 forced_peep("forced_peep", DAccess::ReadWrite, 5, "cmH2O",
                           "Target PEEP; overrides GUI setting when forced_mode is valid");
static DUint32 forced_pip("forced_pip", DAccess::ReadWrite, 15, "cmH2O",
                          "Target PIP; overrides GUI setting when forced_mode is valid");
static DFloat forced_ie_ratio("forced_ie_ratio", DAccess::ReadWrite, 0.66f, "ratio",
                              "Target I:E ratio; overrides GUI setting when forced_mode is valid");
static DFloat forced_fio2(
    "forced_fio2", DAccess::ReadWrite, 21, "%",
    "Target percent oxygen [21, 100]; overrides GUI setting when forced_mode is valid");

static Controller controller;
static ControllerStatus controller_status;
static Sensors sensors;
static NVParams::Handler nv_params;
static I2Ceeprom eeprom = I2Ceeprom(0x50, 64, 32768, &i2c1);
static Actuators actuators(0, 1);

// Global variables for the debug interface
static Debug::Trace trace;
// Create a handler for each of the known commands that the Debug Handler can
// link to.  This is a bit tedious but I can't find a simpler way.
static Debug::Command::ModeHandler mode_command;
static Debug::Command::PeekHandler peek_command;
static Debug::Command::PokeHandler poke_command;
static Debug::Command::VarHandler var_command;
static Debug::Command::TraceHandler trace_command(&trace);
static Debug::Command::EepromHandler eeprom_command(&eeprom);

static Debug::Interface debug(&trace, 12, Debug::Command::Code::Mode, &mode_command,
                              Debug::Command::Code::Peek, &peek_command, Debug::Command::Code::Poke,
                              &poke_command, Debug::Command::Code::Variable, &var_command,
                              Debug::Command::Code::Trace, &trace_command,
                              Debug::Command::Code::EepromAccess, &eeprom_command);

static SensorsProto AsSensorsProto(const SensorReadings &r, const ControllerState &c) {
  SensorsProto proto = SensorsProto_init_zero;
  proto.patient_pressure_cm_h2o = r.patient_pressure.cmH2O();
  proto.inflow_pressure_diff_cm_h2o = 0;   // \TODO field unused and obsolete, should change proto
  proto.outflow_pressure_diff_cm_h2o = 0;  // \TODO field unused and obsolete, should change proto
  proto.flow_ml_per_min = c.net_flow.ml_per_min();
  proto.volume_ml = c.patient_volume.ml();
  proto.breath_id = c.breath_id;
  proto.flow_correction_ml_per_min = c.flow_correction.ml_per_min();
  proto.fio2 = r.fio2;
  return proto;
}

// This function handles all the high priority tasks which need to be called
// periodically.  The HAL calls this function from a timer interrupt.
//
// NOTE - its important that anything being called from this function executes
// quickly.  No busy waiting here.
static void HighPriorityTask(void *arg) {
  // Read the sensors
  SensorReadings sensor_readings = sensors.get_readings();

  // Run our PID loop
  auto [actuators_state, controller_state] = controller.Run(
      SystemTimer::singleton().now(), controller_status.active_params, sensor_readings);

  // TODO update pb library to replace fan_power in ControllerStatus with
  // actuators_state, and remove pressure_setpoint_cm_h2o from ControllerStatus

  // Update the outputs from the PID
  actuators.execute(actuators_state);

  // Update controller_status.  This is periodically sent back to the GUI.
  controller_status.sensor_readings = AsSensorsProto(sensor_readings, controller_state);
  controller_status.fan_power = actuators_state.blower_power;
  controller_status.pressure_setpoint_cm_h2o = controller_state.pressure_setpoint.cmH2O();

  // Sample any trace variables that are enabled
  debug.SampleTraceVars();

  Watchdog::pet();
}

// This function is the lower priority background loop which runs continuously
// after some basic system init.  Pretty much everything not time critical
// should go here.
[[noreturn]] static void BackgroundLoop() {
  // Sleep for a few seconds.  In the current iteration of the PCB, the fan
  // briefly turns on when the device starts up.  If we don't wait for the fan
  // to spin down, the sensors will miscalibrate.  This is a hardware issue
  // that will be fixed in the PCB revision after 0.2.
  // https://respiraworks.slack.com/archives/C011CJQV4Q7/p1591745893290300?thread_ts=1591745582.289600&cid=C011CJQV4Q7
  //
  // Take this opportunity while we're sleeping to home the pinch valves.  This
  // way we're guaranteed that they're ready before we start ventilating.
  Time sleep_start = SystemTimer::singleton().now();
  while (!actuators.ready() || SystemTimer::singleton().now() - sleep_start < seconds(10)) {
    actuators.execute({
        .fio2_valve = 0,
        .blower_power = 0,
        .blower_valve = 1,
        .exhale_valve = 1,
    });
    SystemTimer::singleton().delay(milliseconds(10));
    Watchdog::pet();
    debug.Poll();
  }

  // Calibrate the sensors.
  // This needs to be done before the sensors are used.
  sensors.calibrate();

  // Current controller status.
  // Updated when we receive data from the GUI, when sensors read data, etc.
  controller_status = ControllerStatus_init_zero;

  // Last-received status from the GUI.
  GuiStatus gui_status = GuiStatus_init_zero;

  // After all initialization is done, ask the HAL to start our high priority thread.
  hal.StartLoopTimer(Controller::GetLoopPeriod(), HighPriorityTask, nullptr);

  while (true) {
    controller_status.uptime_ms = SystemTimer::singleton().now().microsSinceStartup() / 1000;

    // Copy the current controller status with interrupts disabled to ensure that the data we
    // send to the GUI is self-consistent.
    ControllerStatus local_controller_status;
    {
      BlockInterrupts block;
      local_controller_status = controller_status;
    }

    CommsHandler(local_controller_status, &gui_status);

    // Override received gui_status from the RPi with values from DebugVars iff
    // the forced_mode DebugVar has a legal value.
    if (uint32_t m = forced_mode.get(); m >= _VentMode_MIN && m <= _VentMode_MAX) {
      auto &p = gui_status.desired_params;
      p.mode = static_cast<VentMode>(m);
      p.breaths_per_min = forced_breath_rate.get();
      p.peep_cm_h2o = forced_peep.get();
      p.pip_cm_h2o = forced_pip.get();
      p.inspiratory_expiratory_ratio = forced_ie_ratio.get();
      p.fio2 = forced_fio2.get() / 100.f;
    }

    // Copy the gui_status data into our controller status with interrupts disabled.
    // This ensures that the data is copied atomically
    {
      BlockInterrupts block;
      controller_status.active_params = gui_status.desired_params;
    }

    // Handle the debug serial interface
    debug.Poll();

    // Update nv_params
    nv_params.Update(SystemTimer::singleton().now(), &gui_status.desired_params);
  }
}

int main() {
  // Initialize hal first because it initializes the watchdog. See comment on HalApi::Init().
  hal.Init();

  // Init the pwm pins for actuators
  actuators.Init(HalApi::GetCpuFreq());

  // Locate our non-volatile parameter block in flash
  nv_params.Init(&eeprom);
  actuators.link(&nv_params, offsetof(NVParams::Structure, blower_pinch_cal),
                 offsetof(NVParams::Structure, exhale_pinch_cal),
                 offsetof(NVParams::Structure, blower_cal),
                 offsetof(NVParams::Structure, psol_cal));

  CommsInit();

  BackgroundLoop();
}
