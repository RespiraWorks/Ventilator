/* Copyright 2020, RespiraWorks

  This file is part of VentilatorSoftware.

  VentilatorSoftware is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  VentilatorSoftware is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with VentilatorSoftware.  If not, see <https://www.gnu.org/licenses/>.
*/

/*
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