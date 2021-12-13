/* Copyright 2021, RespiraWorks

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

#include "system_constants.h"
//////////////////////////////////////////////////////////////////
//                   SENSOR LOGICAL MAPPINGS                    //
//   Change these if you route your sensor tubing differently   //
//////////////////////////////////////////////////////////////////
GPIO::AdcChannel adc_channel(Sensor s) {
  switch (s) {
    case Sensor::PatientPressure:
      return InterimBoardAnalogPressure;
    case Sensor::OxygenInflowPressureDiff:
      return U3PatientPressure;
    case Sensor::AirInflowPressureDiff:
      return U4InhaleFlow;
    case Sensor::OutflowPressureDiff:
      return U5ExhaleFlow;
    case Sensor::FIO2:
      return InterimBoardOxygenSensor;
  }
  // Switch above covers all cases.
  __builtin_unreachable();
}
