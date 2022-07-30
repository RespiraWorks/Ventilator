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

#include "v03system.h"

#include "flash.h"
#include "framing_spec_chars.h"
#include "hal.h"
#include "system_constants.h"
#include "system_timer.h"
#include "watchdog.h"

// NOTE - I can't do this in the constructor because the Hal needs to be initialized, and we need to
//  be able to write to the registers
void V03Sensors::init(ADC &adc) {
  // Here we create all sensors and initialize the adc.
  patient_pressure_sensor_.emplace("patient_pressure_", "for patient airway pressure",
                                   GPIO::AnalogInputPin(adc_channel(Sensor::PatientPressure), &adc),
                                   ADC::VoltageRange);
  fio2_sensor_.emplace("fio2", "Fraction of oxygen in supplied air",
                       GPIO::AnalogInputPin(adc_channel(Sensor::FIO2), &adc));
  air_influx_sensor_dp_.emplace(
      "air_influx_", "for ambient air influx",
      GPIO::AnalogInputPin(adc_channel(Sensor::AirInflowPressureDiff), &adc), ADC::VoltageRange);
  oxygen_influx_sensor_dp_.emplace(
      "oxygen_influx_", "for concentrated oxygen influx",
      GPIO::AnalogInputPin(adc_channel(Sensor::OxygenInflowPressureDiff), &adc), ADC::VoltageRange);
  outflow_sensor_dp_.emplace("outflow_", "for outflow",
                             GPIO::AnalogInputPin(adc_channel(Sensor::OutflowPressureDiff), &adc),
                             ADC::VoltageRange);

  // These require existing DP sensors to link to
  air_influx_sensor_.emplace("air_influx_", "for ambient air influx",
                             &air_influx_sensor_dp_.value(), VenturiPortDiameter,
                             VenturiChokeDiameter, VenturiCorrection);
  oxygen_influx_sensor_.emplace("oxygen_influx_", "for concentrated oxygen influx",
                                &oxygen_influx_sensor_dp_.value(), VenturiPortDiameter,
                                VenturiChokeDiameter, VenturiCorrection);
  outflow_sensor_.emplace("outflow_", "for outflow", &outflow_sensor_dp_.value(),
                          VenturiPortDiameter, VenturiChokeDiameter, VenturiCorrection);
}

void V03Sensors::calibrate() {
  /// \TODO It seems that we'll need to save calibration readings to non-volatile memory and provide
  //    operators with a way to shut off the device's blowers, open any necessary valves, and
  //    recalibrate.
  patient_pressure_sensor_->set_zero();
  air_influx_sensor_dp_->set_zero();
  oxygen_influx_sensor_dp_->set_zero();
  outflow_sensor_dp_->set_zero();
  fio2_sensor_->set_zero();
}

/// \TODO Add alarms if sensor value is out of expected range?

SensorReadings V03Sensors::get_readings() const {
  /// \TODO Potential Caution: Density of air slightly varies over temperature and altitude - need
  ///   mechanism to adjust based on delivery? Constant involving density of air. Density assumed at
  ///   15 deg. Celsius and 1 atm of pressure.
  ///   Sourced from https://en.wikipedia.org/wiki/Density_of_air
  static constexpr float AirDensity{1.225f};  // kg/m^3

  // Assuming ambient pressure of 101.3 kPa
  /// \TODO measure ambient pressure from an additional sensor and/or estimate from user input
  ///   (from altitude?)
  static constexpr Pressure AmbientPressure = kPa(101.3f);

  return {
      .patient_pressure = patient_pressure_sensor_->read(),
      .fio2 = fio2_sensor_->read(AmbientPressure),
      .air_inflow = air_influx_sensor_->read(AirDensity),
      .oxygen_inflow = oxygen_influx_sensor_->read(AirDensity),
      .outflow = outflow_sensor_->read(AirDensity),
  };
}

void V03Actuators::init(NVParams::Handler *nv_params) {
  blower_pinch_.emplace("blower_valve_", " of the blower pinch valve", BlowerValveMotorIndex);
  exhale_pinch_.emplace("exhale_valve_", " of the exhale pinch valve", ExhaleValveMotorIndex);

  // For now, the blower uses default calibration values, linearly spaced between 0 and 1
  blower_.emplace(BlowerChannel, BlowerFreq, CPUFrequency, "blower_", " of the blower");

  psol_.emplace(PSolChannel, PSolFreq, CPUFrequency, "psol_", " of the proportional solenoid",
                "position", PSolClosed, PSolOpen);

  buzzer_.emplace(BuzzerChannel, BuzzerFreq, CPUFrequency, "buzzer_", "of the buzzer", "volume",
                  BuzzerOff, MaxBuzzerVolume);

  // In case init was called with nullptr, these fail silently
  blower_pinch_->LinkCalibration(nv_params, offsetof(NVParams::Structure, blower_pinch_cal));
  exhale_pinch_->LinkCalibration(nv_params, offsetof(NVParams::Structure, exhale_pinch_cal));
  blower_->LinkCalibration(nv_params, offsetof(NVParams::Structure, blower_cal));
  psol_->LinkCalibration(nv_params, offsetof(NVParams::Structure, psol_cal));

  /// \todo abstract mappings; move them to system_constants.h
  // Configure LED pins as outputs
  // PC13 - red
  // PC14 - yellow
  // PC15 - green
  led_red_.emplace(GPIO::Port::C, 13);
  led_yellow_.emplace(GPIO::Port::C, 14);
  led_green_.emplace(GPIO::Port::C, 15);
}

// Return true if all actuators are enabled and ready for action
bool V03Actuators::ready() {
  return blower_pinch_.has_value() && blower_pinch_->IsReady() && exhale_pinch_.has_value() &&
         exhale_pinch_->IsReady() && blower_.has_value() && psol_.has_value() &&
         led_red_.has_value() && led_yellow_.has_value() && led_green_.has_value();
  // but not buzzer
}

// Assumes all actuators have been emplaced.
// If the pinch valves are not homed yet, this will eventually home them.
void V03Actuators::execute(const ActuatorsState &desired_state) {
  // set blower PWM
  blower_->set(desired_state.blower_power);

  // Set the blower pinch valve position
  if (desired_state.blower_valve.has_value()) {
    blower_pinch_->SetOutput(*desired_state.blower_valve);
  } else {
    blower_pinch_->Disable();
  }

  // Set the exhale pinch valve position
  if (desired_state.exhale_valve.has_value()) {
    exhale_pinch_->SetOutput(*desired_state.exhale_valve);
  } else {
    exhale_pinch_->Disable();
  }

  psol_->set(desired_state.fio2_valve);

  desired_state.led_red ? led_red_->set() : led_red_->clear();
  desired_state.led_yellow ? led_yellow_->set() : led_yellow_->clear();
  desired_state.led_green ? led_green_->set() : led_green_->clear();
}

void V03Actuators::beep(float volume) {
  if (buzzer_.has_value()) {
    buzzer_->set(volume);
  }
}

/// \TODO move constants to system_constants.h, so they can be reused in integration tests
V03System::V03System()
    : rpi_uart_(UART::Base::UART3, DMA::Base::DMA1, FramingMark),
      debug_uart_(UART::Base::UART2),
      i2c1_(I2C::Base::I2C1, DMA::Base::DMA2),
      eeprom_(0x50, 64, 32768, &i2c1_) {}

void V03System::init_hal() {
  hal.Init(CPUFrequency);

  // We use one of the basic timers (timer 6) for general system timing. We configure it to count
  // every 100ns and generate an interrupt every millisecond. Note that this assumes the cpu
  // frequency is a multiple of 10MHz. Basic timers are documented in [RM] chapter 29.
  SystemTimer::singleton().initialize(PeripheralID::Timer6, InterruptVector::Timer6, CPUFrequency);
}

void V03System::init_subsystems() {
  /// \todo abstract mappings; move them to system_constants.h
  // Configure PCB ID pins as inputs.
  // PB1  - ID0
  // PA12 - ID1
  GPIO::DigitalInputPin pcb_pin0(GPIO::Port::B, 1);
  GPIO::DigitalInputPin pcb_pin1(GPIO::Port::A, 12);
  pcb_id0_.set(pcb_pin0.get());
  pcb_id1_.set(pcb_pin1.get());

  /// \TODO move constants to system_constants.h, so they can be reused in integration tests
  // [PCBsp] Lists UART pins for comms with the rPi: PB10 (TX), PB11 (RX), PB13 (RTS) and PB14 (CTS)
  rpi_uart_.Initialize(GPIO::Port::B, /*tx_pin=*/10, /*rx_pin=*/11, /*rts_pin=*/13, /*cts_pin=*/14,
                       GPIO::AlternativeFunction::AF7, CPUFrequency, UARTBaudRate);
  // The Nucleo board also includes a secondary serial port that's indirectly connected to its USB
  // connector.  This port is connected to the STM32 USART2 at pins PA2 (TX) and PA3 (RX) and has
  // no HW flow control (rts/cts)
  debug_uart_.Initialize(GPIO::Port::A, /*tx_pin=*/2, /*rx_pin=*/3,
                         /*rts_pin=*/std::nullopt, /*cts_pin=*/std::nullopt,
                         GPIO::AlternativeFunction::AF7, CPUFrequency, UARTBaudRate);

  // [PCBsp] lists I2C1 pins : SCL=PB8 and SDA=PB9
  i2c1_.Initialize(I2C::Speed::Fast, GPIO::Port::B, /*scl_pin=*/8, /*sda_pin=*/9,
                   GPIO::AlternativeFunction::AF4);

#if defined(BARE_STM32)
  hal.bind_channels(&i2c1_, &rpi_uart_, &debug_uart_);
#endif

  Interrupts::singleton().EnableInterrupts();
  StepMotor::OneTimeInit();

  // Locate our non-volatile parameter block in flash
  nv_params_.Init(&eeprom_);
  // Init the pwm pins for actuators and link calibration tables
  actuators_.init(&nv_params_);

  sensors_.init(adc_);

  /// \TODO fault somehow if this returns false
  [[maybe_unused]] bool buffer_size_sufficient = adc_.initialize(CPUFrequency);

  // Sleep for a few seconds.  In the current iteration of the PCB, the fan briefly turns on when
  // the device starts up.  If we don't wait for the fan to spin down, the sensors will
  // miscalibrate.  This is a hardware issue that will be fixed in the PCB revision after 0.2.
  // https://respiraworks.slack.com/archives/C011CJQV4Q7/p1591745893290300?thread_ts=1591745582.289600&cid=C011CJQV4Q7
  //
  // Take this opportunity while we're sleeping to home the pinch valves.  This way we're guaranteed
  // that they're ready before we start ventilating.
  Time sleep_start = SystemTimer::singleton().now();
  while (!actuators_.ready() || SystemTimer::singleton().now() - sleep_start < seconds(10)) {
    actuators_.execute({
        .fio2_valve = 0,
        .blower_power = 0,
        .blower_valve = 1,
        .exhale_valve = 1,
    });
    SystemTimer::singleton().delay(milliseconds(10));
    Watchdog::pet();
  }

  /// We wait 20ms from power-on-reset for pressure sensors to warm up.
  ///
  /// TODO: Is 20ms the right amount of time?  We're basing it on the data sheet for MPXV7002,
  ///  https://www.nxp.com/docs/en/data-sheet/MPXV7002.pdf table 1, last entry.  But we're not
  ///  actually using that pressure sensor, we're using MPXV5004DP!  The 5004DP datasheet doesn't
  ///  say anything about a startup time.  20ms is probably fine, but we should verify.
  ///
  /// TODO: This is unsafe if/when the controller starts up while connected to a patient!
  ///  Calibration is valid only if the physical system is quiescent, but if a patient is attached
  ///  (or if the blower was running just a few milliseconds ago), obviously that's not true.
  SystemTimer::singleton().delay(milliseconds(20));
}
