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

#ifndef FLASH_H_
#define FLASH_H_

#include <cstddef>
#include <cstdint>

// Flash memory location & size info
inline constexpr uint32_t flash_start_addr{0x08000000};
inline constexpr size_t flash_size{32 * 1024};
inline constexpr size_t flash_page_size{2 * 1024};

#endif
