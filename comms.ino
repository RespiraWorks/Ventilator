/* Copyright 2020, Edwin Chiu

  This file is part of FixMoreLungs.

  FixMoreLungs is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FixMoreLungs is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FixMoreLungs.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "comms.h"

void comms_init() {
  // initialize serial communications at 115200 bps
  Serial.begin(115200);
}

void comms_send(double Setpoint, int sensorValue) {
  Serial.print("C"); //output to monitor
  Serial.write(int(map(Setpoint, 0, 255, 0, 1023)) >> 8); //output to monitor
  Serial.write(int(map(Setpoint, 0, 255, 0, 1023)) & 0xff); //output to monitor
  Serial.write(int(sensorValue) >> 8); //output to monitor
  Serial.write(int(sensorValue) & 0xff); //output to monitor
}
