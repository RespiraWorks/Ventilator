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

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

// Flash memory location & size info
const uint32_t flash_start_addr = 0x08000000;
const uint32_t flash_size = 0x00080000;
const uint32_t flash_page_size = 0x00000800;

// Main program memory area
const uint32_t flash_code_start = 0x08000000;
const uint32_t flash_code_size = 0x0007f000;

// Last two flash pages used for parameter storage
const uint32_t flash_params_start = 0x0807f000;
const uint32_t flash_params_size = 0x00001000;

#endif
