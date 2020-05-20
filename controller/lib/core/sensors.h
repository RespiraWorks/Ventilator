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

module contributors: verityRF, jlebar, lee-matthews, Edwin Chiu

The purpose of this module is to allow calibrated readings from the different
pressure sensors in the ventilator design. It is designed to be used with the
Arduino Nano and the MPXV5004GP and MPXV7002DP pressure sensors.
*/

#ifndef SENSORS_H
#define SENSORS_H

#include "hal.h"
#include "network_protocol.pb.h"
#include "units.h"
#include <cmath>

static constexpr Duration SENSORS_SAMPLE_PERIOD = milliseconds(1);

class TVIntegrator {
public:
  void AddFlow(Time now, VolumetricFlow flow);
  Volume GetTV() const { return volume_; }

private:
  Time last_volume_update_time_ = Hal.now();
  VolumetricFlow last_flow_ = cubic_m_per_sec(0);
  Volume volume_ = ml(0);
};

// Template to create a first order low pass filter on strongly-typed data
// state_ is the output value of the lowpass filter
// Call creator with cutoff frequency and sample time to initialize the
// coefficient of the filter
//
// Creating a new filter class requires defining Update with the correct unit:
// Update(value){state_ = unit((alpha_*value.unit()+state_.unit())/(alpha_+1))
// ;}
//
template <class Q> class LowPassFilter {
public:
  LowPassFilter(float cutoff_frequency, Duration sample_time)
      : alpha_(cutoff_frequency * 2 * static_cast<float>(M_PI) *
               sample_time.seconds()){};
  Q getLowPassValue() { return state_; }

protected:
  Q state_;
  float alpha_;
};

// First order lowpass filter on flow
class FlowFilter : LowPassFilter<VolumetricFlow> {
public:
  using LowPassFilter<VolumetricFlow>::getLowPassValue;
  void Update(VolumetricFlow flow) {
    state_ = ml_per_min((alpha_ * flow.ml_per_min() + state_.ml_per_min()) /
                        (alpha_ + 1));
  };

private:
  using LowPassFilter<VolumetricFlow>::LowPassFilter;
};

// Provides calibrated sensor readings, including tidal volume (TV)
// integrated from flow.
class Sensors {
public:
  Sensors();

  // Perform some initial sensor calibration.  This function should
  // be called on system startup before any other sensor functions
  // are called.
  void Calibrate();

  // get the sensor readings (patient pressure, volumetric flow and tidal
  // volume) from the sensors
  SensorReadings GetSensorReadings();

  // min/max possible reading from MPXV5004GP pressure sensors
  // The canonical list of hardware in the device is: https://bit.ly/3aERr69
  inline constexpr static Pressure P_VAL_MIN = kPa(0.0f);
  inline constexpr static Pressure P_VAL_MAX = kPa(3.92f);

  /*
   * @brief Method implements Bernoulli's equation assuming the Venturi Effect.
   * https://en.wikipedia.org/wiki/Venturi_effect
   *
   * Solves for the volumetric flow rate since A1/A2, rho, and differential
   * pressure are known. Q = sqrt(2/rho) * (A1*A2) * 1/sqrt(A1^2-A2^2) *
   * sqrt(p1-p2); based on (p1 - p2) = (rho/2) * (v2^2 - v1^2); where A1 > A2
   *
   * @return the volumetric flow in [meters^3/s]. Can be negative, indicating
   * direction of flow, depending on how the differential sensor is attached to
   * the venturi.
   */
  static VolumetricFlow PressureDeltaToFlow(Pressure delta);

private:
  enum Sensor {
    PATIENT_PRESSURE,
    INFLOW_PRESSURE_DIFF,
    OUTFLOW_PRESSURE_DIFF,
  };
  // Keep this in sync with the Sensor enum!
  inline constexpr static int NUM_SENSORS = 3;

  static AnalogPin PinFor(Sensor s);
  Pressure ReadPressureSensor(Sensor s);

  // Calibrated average sensor values in a zero state.
  Voltage sensors_zero_vals_[NUM_SENSORS];

  FlowFilter flow_lowpass_ = FlowFilter(100.0f, SENSORS_SAMPLE_PERIOD);

  // State related to integrating volume from flow.
  TVIntegrator tv_integrator_;
};

#endif // SENSORS_H
