/* Copyright 2020-2021, RespiraWorks

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
#pragma once

/******************************************************************
 * Watchdog timer (see [RM] chapter 32).
 *
 * The watchdog timer will reset the system if it hasn't been
 * re-initialized within a specific amount of time.  It's used
 * to catch bugs that would otherwise hang the system.  When
 * the watchdog is enabled such a bug will reset the system
 * rather then let it hang indefinitely.
 *****************************************************************/

namespace Watchdog {
// Initializes watchdog, sets appropriate pins to Output, etc.
// Called by HAL when setting up loop timer
void initialize();

// Pets the watchdog, this makes the watchdog not reset the system for configured amount of time
void pet();
}  // namespace Watchdog
