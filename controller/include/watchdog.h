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

#ifndef WATCHDOG_H
#define WATCHDOG_H

// The watchdog checks for hangs / crashes.  It resets the device if
// watchdog_handler() is not called for "too long".
//
// This module also exposes reset_device(), which immediately restarts the
// device.
//
// Implementation note: reset_device() restarts the controller by setting a
// short watchdog timeout and then entering an infinite loop; when the short
// timeout expires, the device restarts.  When the device boots back up, the
// docs [0] claim that this timeout will be retained.  You should therefore
// **call watchdog_init() very early in startup**, to reset the timer back to a
// normal, longer value.
//
// [0] https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
void watchdog_init();
void watchdog_handler();
[[noreturn]] void reset_device();

#endif // WATCHDOG_H
