/* Copyright 2020, RespiraWorks

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

#include "hal.h"

// Provide an implementation of __cxa_pure_virtual, which is called when you
// try to invoke a pure-virtual function.  Without this, the default
// implementation of __cxa_pure_virtual calls malloc.
//
// If we ever make unit tests run on device, this may need to move into
// controller/lib, and every library (or, every library that has a pure virtual
// function) will need to depend on it.  That's a little tricky since
// platformio discovers library dependencies via header files, and all this
// really needs is to be linked in.  But we could e.g. just include a dummy
// header in the putative library from each file with a pure virtual function.
// We'd probably also need to remove the dependency on hal, so this could be
// used from libraries that don't link with hal.
extern "C" void __cxa_pure_virtual() { Hal.reset_device(); }
