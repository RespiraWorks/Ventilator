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

#include "solenoid.h"

void solenoid_init() {
    pinMode(O_SOLENOID, OUTPUT);
}

void solenoid_open() {
    if(parameters_getSolenoidNormalState() == solenoidNormaleState::normally_open) {
        digitalWrite(O_SOLENOID, LOW);
    }
    else {
        digitalWrite(O_SOLENOID, HIGH);
    }
}

void solenoid_close() {
    if(parameters_getSolenoidNormalState() == solenoidNormaleState::normally_open) {
        digitalWrite(O_SOLENOID, HIGH);
    }
    else {
        digitalWrite(O_SOLENOID, LOW);
    }
}
