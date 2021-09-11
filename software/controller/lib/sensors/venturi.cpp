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

#include "venturi.h"

#include <cmath>

float pow2(float f) { return f * f; }

// \TODO: put this in math header and have it tested
// Returns an area in meters squared.
float diameter_to_area_m2(Length diameter) {
  return static_cast<float>(M_PI) / 4.0f * pow2(diameter.meters());
};

VenturiFlowSensor::VenturiFlowSensor(const char* name, const char* help_supplement,
                                     PressureSensor* pressure_sensor, Length venturi_port_diameter,
                                     Length venturi_choke_diameter, float venturi_correction)
    : FlowSensor(name, help_supplement),
      pressure_sensor_(pressure_sensor),
      venturi_correction_(venturi_correction) {
  port_area_ = diameter_to_area_m2(venturi_port_diameter);
  choke_area_ = diameter_to_area_m2(venturi_choke_diameter);
}

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
VolumetricFlow VenturiFlowSensor::read(const HalApi& hal_api, float air_density) const {
  auto ret = pressure_delta_to_flow(pressure_sensor_->read(hal_api), air_density);
  dbg_flow_.set(ret.ml_per_sec());
  return ret;
}

VolumetricFlow VenturiFlowSensor::pressure_delta_to_flow(Pressure delta, float air_density) const {
  return cubic_m_per_sec(venturi_correction_ *
                         std::copysign(std::sqrt(std::abs(delta.kPa()) * 1000.0f), delta.kPa()) *
                         std::sqrt(2 / air_density) * port_area_ * choke_area_ /
                         std::sqrt(pow2(port_area_) - pow2(choke_area_)));
}
