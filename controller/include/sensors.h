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

 module contributors: verityRF, lee-matthews, Edwin Chiu
 The purpose of this module is to allow calibrated readings from the different pressure sensors in the ventilator design.
 It is designed to be used with the Arduino Nano and the MPXV5004GP and MPXV7002DP pressure sensors.
*/

#ifndef SENSORS_H
#define SENSORS_H

/*
 * @brief enum for the three different pressure sensors in the ventilator system
 */
enum class pressureSensors {
    PATIENT = 0, //patient pressure sensor
    INHALATION, //inhalation diff pressure sensor
    EXHALATION //exhalation diff pressure sensor
};

/*
 * @brief This method is to be called once on POR to initialize the module.
 *        It calls zero_sensors() for an initial calibration. Call before attempting to get sensor readings.
 */
void sensors_init();

/*
 * @brief This method calculates and saves the zero pressure reading for each of the three pressure sensors for calibration. Called once on POR by sensors_init().
 * @usage The method should be called regularly to minimize sensor errors due to environmental changes (like temperature).
 *        It is important to note that all non-differential sensors must be experience atmospheric pressure during calibration.
 *        Differential sensors must have the same pressure at both ports during calibration.
 *        The routine automatically turns off the blower prior to calibration; but there may be other valves/controls to be actuated to meet these requirements.
 */
void zero_sensors();

/*
 * @brief This method gets the specified calibrated sensor reading and performs simple averaging if configured to do so.
 * @param pressureSensor the sensor from the pressureSensor enum that a reading is desired from
 * @return The specified pressure sensor calibrated reading in kPa
 */
float get_pressure_reading(enum pressureSensors pressureSensor);

/*
 *  @brief Method for setting the number of samples to use for average during sensor zeroization.
 *  @param numAvgSamples the number of samples to use for averaging, between 1 and 32 inclusive
 */
void set_zero_avg_samples(int numAvgSamples);

/*
 *  @brief Method for getting the number of samples in use for averaging during sensor zeroization.
 *  @return A number between 1 and 32 inclusive.
 */
int get_zero_avg_samples();

/*
 *  @brief Method for setting the number of samples to use for average during sensor zeroization.
 *  @param numAvgSamples the number of samples to use for averaging, between 1 and 32 inclusive
 */
void set_sensor_avg_samples(int numAvgSamples);

/*
 *  @brief Method for getting the number of samples in use for averaging during calibrated sensor reads.
 *  @return A number between 1 and 32 inclusive.
 */
int get_sensor_avg_samples();

#endif // SENSORS_H