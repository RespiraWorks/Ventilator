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
