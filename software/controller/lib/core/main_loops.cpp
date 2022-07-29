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

#include "main_loops.h"

#include "hal.h"
#include "system_constants.h"  // for cpu frequency
#include "system_timer.h"
#include "watchdog.h"

SensorsProto MainContainer::AsSensorsProto(const SensorReadings &r, const ControllerState &c) {
  SensorsProto proto = SensorsProto_init_zero;
  proto.patient_pressure_cm_h2o = r.patient_pressure.cmH2O();
  proto.inflow_pressure_diff_cm_h2o = 0;   /// \TODO field unused and obsolete, should change proto
  proto.outflow_pressure_diff_cm_h2o = 0;  /// \TODO field unused and obsolete, should change proto
  proto.flow_ml_per_min = c.net_flow.ml_per_min();
  proto.volume_ml = c.patient_volume.ml();
  proto.breath_id = c.breath_id;
  proto.flow_correction_ml_per_min = c.flow_correction.ml_per_min();
  proto.fio2 = r.fio2;
  return proto;
}

void MainContainer::init() {
  // Initialize hal first because it initializes the watchdog. See comment on HalApi::Init().
  hardware_layer.init_hal();

  comms.emplace(hardware_layer.comms_channel());
  debug.emplace(hardware_layer.debug_channel());
  // Create a handler for each of the known commands that the Debug Handler can link to.
  // This is a bit tedious, but I can't find a simpler way.
  debug->add_handler(Debug::Command::Code::Mode, &mode_command);
  debug->add_handler(Debug::Command::Code::Peek, &peek_command);
  debug->add_handler(Debug::Command::Code::Poke, &poke_command);
  debug->add_handler(Debug::Command::Code::Variable, &var_command);
  trace_command.emplace(&trace);
  debug->add_handler(Debug::Command::Code::Trace, &trace_command.value());
  eeprom_command.emplace(hardware_layer.eeprom());
  debug->add_handler(Debug::Command::Code::EepromAccess, &eeprom_command.value());

  hardware_layer.init_subsystems();

  comms->Initialize();

  // Calibrate the sensors after the pinch valves are properly homed to avoid the effects of the
  // blower (and to a lesser degree the pinch valves) moving during startup.
  hardware_layer.sensors()->calibrate();

  // After all initialization is done, ask the HAL to start our high priority thread.
  //  hal.StartLoopTimer(Controller::GetLoopPeriod(), [this]() -> void {
  //    return this->high_priority_task();
  //  });

  hal.set_timer15_callback([this]() { this->high_priority_trigger.interrupt_handler(); });

  high_priority_trigger.start(PeripheralID::Timer15, InterruptVector::Timer15, CPUFrequency,
                              Controller::GetLoopPeriod(),
                              [this]() { this->high_priority_task(); });
}

// This function handles all the high priority tasks which need to be called periodically.
// The HAL calls this function from a timer interrupt.
//
// NOTE - its important that anything being called from this function executes quickly.
// No busy waiting here.
void MainContainer::high_priority_task() {
  // Read the sensors
  SensorReadings sensor_readings = hardware_layer.sensors()->get_readings();

  // Run our PID loop
  auto [actuators_state, controller_state] = controller.Run(
      SystemTimer::singleton().now(), controller_status.active_params, sensor_readings);

  // \TODO update pb library to replace fan_power in ControllerStatus with
  //       actuators_state, and remove pressure_setpoint_cm_h2o from ControllerStatus

  // Update the outputs from the PID
  hardware_layer.actuators()->execute(actuators_state);

  // Update controller_status.  This is periodically sent back to the GUI.
  controller_status.sensor_readings = AsSensorsProto(sensor_readings, controller_state);
  controller_status.fan_power = actuators_state.blower_power;
  controller_status.pressure_setpoint_cm_h2o = controller_state.pressure_setpoint.cmH2O();

  // Sample any trace variables that are enabled
  trace.maybe_sample();

  Watchdog::pet();
}

void MainContainer::background_task() {
  controller_status.uptime_ms = SystemTimer::singleton().now().microsSinceStartup() / 1000;

  // Copy the current controller status with interrupts disabled to ensure that the data we send to
  // the GUI is self-consistent.
  ControllerStatus local_controller_status;
  {
    BlockInterrupts block;
    local_controller_status = controller_status;
  }

  comms->Handler(local_controller_status, &gui_status);

  // Override received gui_status from the RPi with values from DebugVars iff the forced_mode
  // DebugVar has a legal value.
  if (uint32_t mode = forced_mode.get(); mode >= _VentMode_MIN && mode <= _VentMode_MAX) {
    auto &p = gui_status.desired_params;
    p.mode = static_cast<VentMode>(mode);
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
  debug->Poll();

  // Update nv_params
  hardware_layer.non_volatile_memory()->Update(SystemTimer::singleton().now(),
                                               &gui_status.desired_params);
}
