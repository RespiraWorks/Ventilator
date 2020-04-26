/* Copyright 2020, Edwin Chiu

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

#include "respiration_ctrl.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "sensors.h"
#include "types.h"

static VentParams previous_params = VentParams_init_zero;

// Define Variables we'll be connecting to
static double Setpoint, Input, Output;

// Configure the PID
// Specify the links and initial tuning parameters
static float Kp = 2, Ki = 8, Kd = 0;
static PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void respiration_ctrl_init() {
  ACV_init(&Setpoint);
  PRVC_init(&Setpoint);
  previous_params = VentParams_init_zero;
}

void respiration_ctrl_handler(const VentParams &params,
                              SensorReadings *readings) {
  // JTravert: move readings here in order to have updated readings which will
  // be required for AVC during AVC handler
  // Store sensor readings so they can eventually be sent to the GUI.
  // This pressure is just from the patient sensor, converted to the right
  // units.
  // Convert [kPa] to [cm H2O] in place
  readings->pressure_cm_h2o =
      get_pressure_reading(PressureSensors::PATIENT_PIN) * 10.1974f;
  // TODO(anyone): This value is to be the integration over time of the below
  // flow_ml_per_min. That is, you take the value calculated for flow_ml_per_min
  // and integrate it over time to get total volume at the current time. Don't
  // think this necessarily has to be done on the controller though?
  readings->volume_ml = 0;
  // Flow rate is inhalation flow minus exhalation flow. Positive value is flow
  // into lungs, and negative is flow out of lungs.
  // Convert [meters^3/s] to [mL/min] in place
  readings->flow_ml_per_min =
      (pressure_delta_to_volumetric_flow(
           get_pressure_reading(PressureSensors::INHALATION_PIN)) -
       pressure_delta_to_volumetric_flow(
           get_pressure_reading(PressureSensors::EXHALATION_PIN))) *
      1000.0f * 1000.0f * 60.0f;

  // sequence of the respiration controller:
  // if previously off, start required mode
  // otherwise, run the required mode handler
  if (previous_params.mode == VentMode_OFF) {
    switch (params.mode) {
    case VentMode_PRESSURE_ASSIST:
      ACV_start(&Setpoint);
      break;
    case VentMode_PRESSURE_CONTROL:
      PRVC_start(&Setpoint);
      break;

    default:
      // TODO Flag error
      break;
    }
  } else {
    switch (params.mode) {
    case VentMode_PRESSURE_ASSIST:
      if (previous_params.mode != params.mode) {
        //@TODO: flag error and take appropriate measures? (TBD)
      } else {
        ACV_handler(*readings, &Setpoint);
      }
      break;

    case VentMode_PRESSURE_CONTROL:
      if (previous_params.mode != params.mode) {
        // TODO: flag error and take appropriate measures? (TBD)
      } else {
        PRVC_handler(*readings, &Setpoint);
      }
      break;

    case VentMode_OFF:
      switch (previous_params.mode) {
      case VentMode_PRESSURE_ASSIST:
        ACV_stop(&Setpoint);
        break;

      case VentMode_PRESSURE_CONTROL:
        PRVC_stop(&Setpoint);
        break;

      default:
        // TODO Flag error
        break;
      }
      break;

    default:
      // TODO Flag error
      break;
    }
  }

  //@TODO: The following are raw accesses that need to be updated to reference
  // the new modules because currently they do not have the benefits of things
  // like calibration, etc. that the module methods do. I didn't do this because
  // I have no clue what the desired units are for the control system.
  // Update PID Loop
  int16_t sensorValue = Hal.analogRead(DPSENSOR_PIN); // read sensor
  Input = map(sensorValue, 0, 1023, 0, 255);
  myPID.Compute();
  //@TODO: change this with call to blower driver once it is operational
  Hal.analogWrite(BLOWERSPD_PIN, static_cast<int>(Output));
  previous_params = params;
}
