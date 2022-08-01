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

// Sensor includes
#include "adc.h"
#include "oxygen.h"
#include "pressure_sensors.h"
#include "sensors.h"
#include "venturi.h"

// Actuator includes
#include "actuators.h"
#include "pinch_valve.h"
#include "pwm_actuator.h"

// System includes
#include "nvparams.h"
#include "system.h"
#include "uart_dma.h"
#include "uart_soft.h"
#include "vars.h"

// Provides calibrated sensor readings
class V03Sensors : public AbstractSensors {
 public:
  V03Sensors() = default;

  // Performs general init.  This function should be called on system startup before any other
  // sensor functions are called.
  void init(ADC& adc);

  // Performs calibration of the sensors. For now, since sensor calibration are not stored in
  // EEPROM, this function should be called immediately after init, but it shouldn't stay that way
  // (to accommodate for ventilator restart while linked to a patient). Assume system quiescence,
  // sufficient pause before this step.
  /// \TODO store calibration in EEPROM and have this called only upon user request to make certain
  //    the vent is not linked to a patient
  void calibrate() override;

  // Read the sensors.
  SensorReadings get_readings() const override;

 private:
  // Fundamental sensors. Because those use GPIO Pins, we need to delay instantiation to after
  // the Hal has been properly initialized. We use std::optional to achieve that, and emplace them
  // within init function
  std::optional<MPXV5010DP<GPIO::AnalogInputPin>> patient_pressure_sensor_{std::nullopt};
  std::optional<TeledyneR24<GPIO::AnalogInputPin>> fio2_sensor_{std::nullopt};
  std::optional<MPXV5004DP<GPIO::AnalogInputPin>> air_influx_sensor_dp_{std::nullopt};
  std::optional<MPXV5004DP<GPIO::AnalogInputPin>> oxygen_influx_sensor_dp_{std::nullopt};
  std::optional<MPXV5004DP<GPIO::AnalogInputPin>> outflow_sensor_dp_{std::nullopt};

  // These require existing DP sensors to link to.
  std::optional<VenturiFlowSensor> air_influx_sensor_{std::nullopt};
  std::optional<VenturiFlowSensor> oxygen_influx_sensor_{std::nullopt};
  std::optional<VenturiFlowSensor> outflow_sensor_{std::nullopt};
};

using Sensors = V03Sensors;

class V03Actuators : public AbstractActuators {
 public:
  V03Actuators() = default;

  // Creates pwm actuators and links actuators calibration tables to nv_params.
  void init(NVParams::Handler* nv_params);

  // Returns true if the actuators are ready for action or false if they aren't (for example pinch
  // valves are homing). The system should be kept in a safe state until this returns true.
  bool ready() override;

  // Causes passed state to be applied to the actuators
  // Assumes precondition ready() == true
  void execute(const ActuatorsState& desired_state) override;

  void beep(float volume) override;

 private:
  // Actuators use pwm pins or spi channels and need to be instantiated after HAL, therefore we use
  // std::optional to delay the instantiation within init function.
  std::optional<PinchValve> blower_pinch_{std::nullopt};
  std::optional<PinchValve> exhale_pinch_{std::nullopt};
  std::optional<PwmActuator> blower_{std::nullopt};
  std::optional<PwmActuator> psol_{std::nullopt};

  // buzzer is made public to allow easier manipulation (not through actuators.execute()).
  std::optional<PwmActuator> buzzer_{std::nullopt};

  std::optional<GPIO::DigitalOutputPin> led_red_{std::nullopt};
  std::optional<GPIO::DigitalOutputPin> led_yellow_{std::nullopt};
  std::optional<GPIO::DigitalOutputPin> led_green_{std::nullopt};
};

using Actuators = V03Actuators;

class V03System : public AbstractSystem {
 public:
  V03System();

  void init_hal() override;

  void init_subsystems() override;

  UART::Channel* comms_channel() override { return &rpi_uart_; }

  UART::Channel* debug_channel() override { return &debug_uart_; }

  I2Ceeprom* eeprom() override { return &eeprom_; }

  NVParams::Handler* non_volatile_memory() override { return &nv_params_; }

  AbstractSensors* sensors() override { return &sensors_; }

  AbstractActuators* actuators() override { return &actuators_; }

 private:
  UART::DMAChannel rpi_uart_;
  UART::SoftChannel debug_uart_;
  I2C::Channel i2c1_;
  I2Ceeprom eeprom_;
  NVParams::Handler nv_params_;
  ADC adc_;
  Sensors sensors_;
  Actuators actuators_;

  using DUint32 = Debug::Variable::UInt32;
  using DAccess = Debug::Variable::Access;
  DUint32 pcb_id0_{"pcb_id_0", DAccess::ReadOnly, 0, "", "PCB version id pin 0"};
  DUint32 pcb_id1_{"pcb_id_1", DAccess::ReadOnly, 0, "", "PCB version id pin 1"};
};

using System = V03System;
