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

#ifndef VENTILATOR_DEFAULTS_H
#define VENTILATOR_DEFAULTS_H

// Ventilator parameter defaults
// FIXME Put some default fallback values that are realistic
#define RR_DEFAULT    (10.0)
#define TV_DEFAULT    (10.0)
#define PEEP_DEFAULT  (10.0)
#define ED_DEFAULT    (10.0)
#define ID_DEFAULT    (10.0)
#define PIP_DEFAULT   (10.0)
#define DWELL_DEFAULT (10.0)
#define IER_DEFAULT   (1.0)

#define PERIODIC_READINGS_DEFAULT   false

#define OPERATING_MODE_DEFAULT      operatingMode::medical
#define PERIODIC_DATA_MODE_DEFAULT  periodicMode::off
#define VENTILATOR_MODE_DEFAULT     ventilatorMode::PRVC
#define SOLENOID_NORMAL_STATE       solenoidNormaleState::normally_open

// Calibration parameter defaults

// PID parameter defaults

#define KP_DEFAULT   (2.0)
#define KI_DEFAULT   (8.0)
#define KD_DEFAULT   (0.0)

#endif // VENTILATOR_DEFAULTS_H
