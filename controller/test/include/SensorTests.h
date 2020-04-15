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
 module contributors: verityRF
 The purpose of this module is to provide unit testing for the sensors controller module.
 Module is to be run on an x86 host and is not to be run on an Arduino platform.
*/

#ifndef SENSORTESTS_H
#define SENSORTESTS_H

/*
 * @brief Runs unit tests for the sensor module. Reports errors to stdout.
 */
void runAllSensorTests();

#endif // SENSORTESTS_H