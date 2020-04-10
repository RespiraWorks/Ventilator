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
#ifndef PID_H
#define PID_H

#include <PID_v1.h>
#include <stdint.h>

#define BLOWERSPD_PIN 3
#define BLOWER_HIGH 142
#define BLOWER_LOW 130
#define DPSENSOR_PIN A0

//state machine variables
#define INSPIRE_TIME 1600
#define INSPIRE_RATE 1
#define PIP 142
#define INSPIRE_DWELL 800
#define INSPIRE_DWELL_PRESSURE 140
#define EXPIRE_TIME 1000
#define EXPIRE_RATE 1
#define PEEP 130
#define EXPIRE_DWELL 600

//not implemented yet
#define AC 0
#define RR 0
#define IE 0

void pid_run();
void pid_init();

#endif  // PID_H
