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

#include <cstddef>
#include <cstdint>

/// \TODO do we still have any use for this class?
namespace Flash {

void initialize();

// Erase a page of flash given the starting address of that page.
bool erase_page(uint32_t address);

// Write data to flash memory at the specified address
// Note that the STM32 only allows writing in multiples of 8 bytes
// to addresses that are a multiple of 8.
//
// @param addr - Address in flash memory at which to write.
//               NOTE - must be a multiple of 8
// @param data - Pointer to data to write
// @param ct   - Number of bytes to write
//               NOTE - must be a multiple of 8
bool write(uint32_t addr, void *data, size_t ct);

}  // namespace Flash
