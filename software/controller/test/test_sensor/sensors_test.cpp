/* Copyright 2020, RespiraWorks

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

/*
 * module contributors: verityRF
 *
 * The purpose of this module is to provide unit testing for the sensors
 * controller module.  Module is to be run on an x86 host and is not to be run
 * on an Arduino platform.
 */

#include "gtest_main.h"
#include "oxygen.h"
#include "pressure_sensors.h"
#include "venturi.h"

class MockAnalogInput {
 public:
  MockAnalogInput(Voltage& voltage) : voltage_(voltage) {}
  Voltage read() const { return voltage_; }
  Voltage& voltage_;
};

/// Voltage range for the ADC
/// \TODO this is assumed to be the same as in system_constants.h?
static constexpr Voltage VoltageRange{volts(3.3f)};

// Assuming ambient pressure of 101.3 kPa
static constexpr Pressure AmbientPressure{kPa(101.3f)};

// Maximum allowable delta between calculated sensor readings and the input.
static const Pressure ComparisonTolerancePressure{kPa(0.005f)};
static const VolumetricFlow ComparisonToleranceFlow{ml_per_sec(50)};
static const float ComparisonToleranceFIO2{0.001f};

// \TODO lots of assumptions here...
static constexpr float air_density{1.225f};  // kg/m^3
constexpr static Length venturi_port_diameter{millimeters(15.05f)};
constexpr static Length venturi_choke_diameter{millimeters(5.5f)};
constexpr static float venturi_correction{0.97f};
static VenturiFlowSensor typical_venturi{
    "", "", nullptr, venturi_port_diameter, venturi_choke_diameter, venturi_correction};

#define EXPECT_PRESSURE_NEAR(a, b) \
  EXPECT_NEAR((a).kPa(), (b).kPa(), ComparisonTolerancePressure.kPa())

#define EXPECT_FLOW_NEAR(a, b) \
  EXPECT_NEAR((a).ml_per_sec(), (b).ml_per_sec(), ComparisonToleranceFlow.ml_per_sec())

//@TODO: Finish writing more specific unit tests for this module

/*
 * @brief This method models the pressure to voltage transfer function of the
 * MPXV5004 series sensors.  The raw voltage coming out of the sensor would be
 * 5 * (...), but our PCB scales it down to 3.3f * (...) so that the pressure
 * range (0-4kPa) is in the voltage range 0-3.3V.
 */
static Voltage MPXV5004_PressureToVoltage(Pressure pressure) {
  return volts(VoltageRange.volts() * (0.2f * pressure.kPa() + 0.2f));
}

// @brief This method models the pressure to voltage transfer function of the
// MPXV5010 series sensors.  The raw voltage coming out of the sensor would be
// 5 * 0.45 * (...), but our board scales it down to 3.3f * (...) so that the
// pressure range (0-10kPa) is in the voltage range 0-3.3V.

static Voltage MPXV5010_PressureToVoltage(Pressure pressure) {
  return volts(VoltageRange.volts() * (0.09f * pressure.kPa() + 0.04f));
}
// This method models the oxygen sensor. Voltage reads partial pressure of O2
// and needs ambient pressure to modulate output voltage.
// Sensitivity is 0.061 V/pt at 1 atm and assumed to vary linearly with pressure
// Offset is assumed to be 10 mV
// Analog amplifier with gain 50 is used on the PCB
static Voltage FIO2ToVoltage(float fio2, Pressure p_amb) {
  return volts(fio2 * p_amb.atm() * 0.06f + 0.01f) * 50.0f;
}

TEST(SensorTests, TestMPXV5010DP) {
  // These pressure waveforms start at 0 kPa to simulate the system being in the
  // proper calibration state then they go over the sensor full ranges.
  std::vector<Pressure> pressures = {kPa(0.0f), kPa(0.5f), kPa(1.0f), kPa(1.5f), kPa(2.0f),
                                     kPa(2.5f), kPa(3.0f), kPa(3.5f), kPa(3.92f)};

  // First set the simulated analog signal to an ambient 0 kPa corresponding
  // voltage during calibration
  Voltage mock_voltage = MPXV5010_PressureToVoltage(kPa(0));
  MPXV5010DP<MockAnalogInput> sensor("sensor1", "blabla", {mock_voltage}, VoltageRange);
  sensor.set_zero();

  // Now to compare the pressure readings the sensor module is calculating
  // versus what the original pressure waveform was
  for (auto p : pressures) {
    SCOPED_TRACE("Pressure " + std::to_string(p.kPa()));
    mock_voltage = MPXV5010_PressureToVoltage(p);
    EXPECT_PRESSURE_NEAR(sensor.read(), p);
  }
}

TEST(SensorTests, TestMPXV5004DP) {
  // These pressure waveforms start at 0 kPa to simulate the system being in the
  // proper calibration state then they go over the sensor full ranges.
  std::vector<Pressure> pressures = {kPa(0.0f), kPa(0.5f), kPa(1.0f), kPa(1.5f), kPa(2.0f),
                                     kPa(2.5f), kPa(3.0f), kPa(3.5f), kPa(3.92f)};

  // First set the simulated analog signal to an ambient 0 kPa corresponding
  // voltage during calibration
  Voltage mock_voltage = MPXV5004_PressureToVoltage(kPa(0));
  MPXV5004DP<MockAnalogInput> sensor("sensor2", "blabla", {mock_voltage}, VoltageRange);
  sensor.set_zero();

  // Now to compare the pressure readings the sensor module is calculating
  // versus what the original pressure waveform was
  for (auto p : pressures) {
    SCOPED_TRACE("Pressure " + std::to_string(p.kPa()));
    mock_voltage = MPXV5004_PressureToVoltage(p);
    EXPECT_PRESSURE_NEAR(sensor.read(), p);
  }
}

TEST(SensorTests, TestTeledyneR24) {
  // calibrate O2 sensor at 21% fio2, 1atm
  Voltage mock_voltage = FIO2ToVoltage(0.21f, atm(1.0f));
  TeledyneR24<MockAnalogInput> sensor("sensor3", "blabla", {mock_voltage});
  sensor.set_zero();

  // fio2 profile
  std::vector<float> fio2_settings = {0.0,  0.1f,  0.25f, 0.33f, 0.4f,
                                      0.6f, 0.66f, 0.75f, 0.9f,  1.0f};

  // sweep all fio2 settings and 1 atm pressure
  for (auto fio2 : fio2_settings) {
    SCOPED_TRACE("fio2 " + std::to_string(fio2));
    mock_voltage = FIO2ToVoltage(fio2, atm(1.0f));
    EXPECT_NEAR(sensor.read(AmbientPressure), fio2, ComparisonToleranceFIO2);
  }
  /// \TODO check the effect of ambient pressure once the system has a way to know ambient pressure
  ///     (either through a sensor or through a user input?)
}

/// \TODO write tests for the actual Venturi class with mock pressure sensor back-end.
// These tests expect Venturi Diameters of 15.05mm and 5.5mm with a "discharge
// coefficient" of 0.97.  If VENTURI_PORT_DIAM or VENTURI_CHOKE_DIAM change,
// this test will fail until you update it.
//
// Expected values from https://www.wolframalpha.com/input/?i=Venturi+flowmeter
TEST(SensorTests, TestVolumetricFlowCalculation) {
  EXPECT_FLOW_NEAR(typical_venturi.pressure_delta_to_flow(kPa(1.0f), air_density),
                   ml_per_sec(939.6f));
  EXPECT_FLOW_NEAR(typical_venturi.pressure_delta_to_flow(kPa(-1.0f), air_density),
                   ml_per_sec(-939.6f));
  EXPECT_FLOW_NEAR(typical_venturi.pressure_delta_to_flow(kPa(0.0f), air_density), ml_per_sec(0));
  EXPECT_FLOW_NEAR(typical_venturi.pressure_delta_to_flow(kPa(1.0e-7f), air_density),
                   ml_per_sec(0.2971f));
  EXPECT_FLOW_NEAR(typical_venturi.pressure_delta_to_flow(kPa(100.0f), air_density),
                   ml_per_sec(9396));
  EXPECT_FLOW_NEAR(typical_venturi.pressure_delta_to_flow(kPa(-100.0f), air_density),
                   ml_per_sec(-9396));
}

/// \TODO thid does not really test "total" flow in any way
TEST(SensorTests, TotalFlowCalculation) {
  // First set the simulated analog signal to an ambient 0 kPa corresponding
  // voltage during calibration
  Voltage mock_air_in = MPXV5004_PressureToVoltage(kPa(0));
  Voltage mock_oxy_in = MPXV5004_PressureToVoltage(kPa(0));
  Voltage mock_out = MPXV5004_PressureToVoltage(kPa(0));
  MPXV5004DP<MockAnalogInput> dp_air_in("dp_air_in", "blabla", {mock_air_in}, VoltageRange);
  MPXV5004DP<MockAnalogInput> dp_oxy_in("dp_oxy_in", "blabla", {mock_oxy_in}, VoltageRange);
  MPXV5004DP<MockAnalogInput> dp_out("dp_out", "blabla", {mock_out}, VoltageRange);
  dp_air_in.set_zero();
  dp_oxy_in.set_zero();
  dp_out.set_zero();

  VenturiFlowSensor air_in("air_in", "", &dp_air_in, venturi_port_diameter, venturi_choke_diameter,
                           venturi_correction);

  VenturiFlowSensor oxy_in("oxy_in", "", &dp_oxy_in, venturi_port_diameter, venturi_choke_diameter,
                           venturi_correction);

  VenturiFlowSensor out("out", "", &dp_out, venturi_port_diameter, venturi_choke_diameter,
                        venturi_correction);

  // These pressure waveforms start at 0 kPa to simulate the system being in the
  // proper calibration state then they go over the sensor full ranges with
  // less samples than for FullScaleReadings.
  std::vector<Pressure> pressures = {kPa(0.0f), kPa(0.5f), kPa(1.0f), kPa(2.0f), kPa(3.5f)};

  for (auto p_air_in : pressures) {
    for (auto p_oxy_in : pressures) {
      for (auto p_out : pressures) {
        mock_air_in = MPXV5004_PressureToVoltage(p_air_in);
        mock_oxy_in = MPXV5004_PressureToVoltage(p_oxy_in);
        mock_out = MPXV5004_PressureToVoltage(p_out);
        EXPECT_FLOW_NEAR(air_in.read(air_density),
                         typical_venturi.pressure_delta_to_flow(p_air_in, air_density));
        EXPECT_FLOW_NEAR(oxy_in.read(air_density),
                         typical_venturi.pressure_delta_to_flow(p_oxy_in, air_density));
        EXPECT_FLOW_NEAR(out.read(air_density),
                         typical_venturi.pressure_delta_to_flow(p_out, air_density));
      }
    }
  }
}

/// \TODO Make unit tests to test zeroing/calibration functionality with e.g. random initial values
