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

#pragma once

#include "pressure_sensors.h"
#include "sensor_base.h"

class VenturiFlowSensor : public FlowSensor {
 public:
  VenturiFlowSensor(const char* name, const char* help_supplement, PressureSensor* pressure_sensor,
                    Length venturi_port_diameter, Length venturi_choke_diameter,
                    float venturi_correction);

  /// \param air_density in units of kg/m^3, will depend on temperature and pressure
  VolumetricFlow read(const HalApi& hal_api, float air_density) const override;

  /// This is exposed as static so the math can be tested without HAL
  VolumetricFlow pressure_delta_to_flow(Pressure delta, float air_density) const;

 private:
  PressureSensor* pressure_sensor_;

  float port_area_;
  float choke_area_;

  /// \TODO: define and explain this
  float venturi_correction_;
};
