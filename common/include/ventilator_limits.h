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

#ifndef VENTILATOR_LIMITS_H
#define VENTILATOR_LIMITS_H

// Used to check that parameters are within "sane" values
// TODO Fill these boundaries with realistic values

#define RR_MIN (0.0)
#define RR_MAX (100.0)

#define TV_MIN (0.0)
#define TV_MAX (100.0)

#define PEEP_MIN (0.0)
#define PEEP_MAX (100.0)

#define IER_MIN (0.1)
#define IER_MAX (10.0)

#define PIP_MIN (0.0)
#define PIP_MAX (100.0)

#define DWELL_MIN (0.0)
#define DWELL_MAX (100.0)


#endif // VENTILATOR_LIMITS_H
