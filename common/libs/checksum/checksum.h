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

#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>

// Computes the fletcher16 checksum for a packet.
//
// The optional `state` param lets you chain together multiple calls to this
// function if you want to checksum one "logical message" which isn't in a
// single buffer.
//
//   uint16_t state = checksum_fletcher16(data0, data0_len);
//   state = checksum_fletcher16(data1, data1_len, state);
//   uint16_t result = checksum_fletcher16(data2, data2_len, state);
//
uint16_t checksum_fletcher16(const char *data, uint8_t count,
                             uint16_t state = 0);

// Computes check bytes for a fletcher16 checksum.
//
// Given a packet p and checksum(p) == c, check_bytes_fletcher16(c) returns two
// bytes [b1 b2] such that checksum(p | b1 | b2) == 0, where `|` represents
// concatenation.
inline uint16_t check_bytes_fletcher16(uint16_t checksum) {
  uint8_t f0 = static_cast<uint8_t>(checksum & 0xff);
  uint8_t f1 = static_cast<uint8_t>((checksum >> 8) & 0xff);
  uint8_t c0 = static_cast<uint8_t>(0xff - ((f0 + f1) % 0xff));
  uint8_t c1 = static_cast<uint8_t>(0xff - ((f0 + c0) % 0xff));
  return static_cast<uint16_t>(uint16_t{c0} << 8) | c1;
}

// Verifies the checksum of a packet.
//
// When creating packets, we append "check bytes" so that the whole packet
// (including the check bytes) has a checksum of 0.
inline bool checksum_check(const char *packet, uint8_t packet_len) {
  return checksum_fletcher16(packet, packet_len) == 0;
}

#endif // CHECKSUM_H
