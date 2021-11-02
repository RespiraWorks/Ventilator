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

#include "actuators.h"

void Actuators::execute(const ActuatorsState &desired_state) {
  // set blower PWM
  blower_->set(desired_state.blower_power);

  // Set the blower pinch valve position
  if (desired_state.blower_valve)
    blower_pinch_.SetOutput(*desired_state.blower_valve);
  else
    blower_pinch_.Disable();

  // Set the exhale pinch valve position
  if (desired_state.exhale_valve)
    exhale_pinch_.SetOutput(*desired_state.exhale_valve);
  else
    exhale_pinch_.Disable();

  psol_->set(desired_state.fio2_valve);
}

void Actuators::Init(Frequency cpu_frequency, NVParams::Handler *nv_params,
                     uint16_t blower_pinch_cal_offset, uint16_t exhale_pinch_cal_offset,
                     uint16_t blower_cal_offset, uint16_t psol_cal_offset) {
  // For now, the blower uses default calibration values, linearly spaced between 0 and 1
  blower_.emplace(BlowerChannel, BlowerFreq, cpu_frequency, "blower_", " of the blower");

  // Testing in Edwin's garage, we found that the psol was fully closed at
  // somewhere between 0.75 and 0.80 (i.e. definitely zero at 0.75 and probably
  // zero a bit above that) and fully open at 0.90.
  // \TODO: the values in the comment are inconsistent with the code, have Edwin confirm those.
  psol_.emplace(PSolChannel, PSolFreq, cpu_frequency, "psol_", " of the proportional solenoid",
                0.35f, 0.75f);

  // In case Init was called with nullptr, these fail silently
  blower_pinch_.LinkCalibration(nv_params, blower_pinch_cal_offset);
  exhale_pinch_.LinkCalibration(nv_params, exhale_pinch_cal_offset);
  blower_->LinkCalibration(nv_params, blower_cal_offset);
  psol_->LinkCalibration(nv_params, psol_cal_offset);
}

// Return true if all actuators are enabled and ready for action
bool Actuators::ready() { return blower_pinch_.IsReady() && exhale_pinch_.IsReady(); }
