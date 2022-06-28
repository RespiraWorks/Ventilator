/* Copyright 2020-2022, RespiraWorks
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

#include <cstdint>

static constexpr uint8_t FramingMark{0xE2};
static constexpr uint8_t FramingEscape{0x27};

// The caracter following FramingEscape char is altered prior to being put to the stream
// for ease of decoding.  We use a self-inverse function, to allow encoding and decoding
// with the same EscapedChar function: EscapedChar(EscapedChar(b))==b;
inline uint8_t EscapedChar(uint32_t b) { return static_cast<uint8_t>(b ^ 0x20); }
