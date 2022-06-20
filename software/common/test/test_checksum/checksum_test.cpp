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

#include "checksum.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <map>

#include "gtest_main.h"

TEST(Checksum32, KnownValues) {
  EXPECT_EQ((uint32_t)0, soft_crc32(NULL, 0));
  EXPECT_EQ((uint32_t)0, soft_crc32(reinterpret_cast<const uint8_t *>(""), 0));
  EXPECT_EQ((uint32_t)0xC808931C, soft_crc32(reinterpret_cast<const uint8_t *>("a"), 1));
  EXPECT_EQ((uint32_t)0x47A393F8, soft_crc32(reinterpret_cast<const uint8_t *>("abcde"), 5));
  EXPECT_EQ((uint32_t)0x9DBDD91C, soft_crc32(reinterpret_cast<const uint8_t *>("abcdef"), 6));
  EXPECT_EQ((uint32_t)0x321FBEF4, soft_crc32(reinterpret_cast<const uint8_t *>("abcdefgh"), 8));
}

TEST(Checksum32, BitFlips) {
  srand(0);

  uint8_t data[32];
  memset(&data, '\0', sizeof(data));

  // Start lastChecksum at -1 because our first test will be checksum'ing the
  // all-zeroes buffer, which should yield 0.
  uint32_t lastChecksum = -1;

  const int32_t numTests = int32_t{1} << 16;
  int32_t singleBitFlipCollisions = 0;
  std::map<int32_t, int32_t> collisions;
  for (int32_t i = 0; i < numTests; ++i) {
    uint32_t checksum = soft_crc32(data, sizeof(data));
    collisions[checksum]++;
    if (checksum == lastChecksum) {
      singleBitFlipCollisions++;
    }

    lastChecksum = checksum;
    int bitToFlip = rand() % static_cast<int>(8 * sizeof(data));
    uint8_t byteMask = static_cast<uint8_t>(1 << (bitToFlip % 8));
    data[bitToFlip / 8] = static_cast<char>(data[bitToFlip / 8] ^ byteMask);
  }

  EXPECT_EQ(0, singleBitFlipCollisions) << "Got at least one collision from one-bit flips; is the "
                                           "checksum broken?";

  // Find the checksum with the most collisions.  There shouldn't be "too many".
  using MapElem = decltype(*collisions.begin());
  auto it = std::max_element(collisions.begin(), collisions.end(),
                             [](MapElem a, MapElem b) { return a.second < b.second; });
  int32_t maxCollisions = it->second;
  double maxCollisionsFrac = 1.0 * maxCollisions / numTests;
  EXPECT_LE(maxCollisionsFrac, 0.0002)
      << "Too many collisions on worst checksum; is the checksum broken?";
}

TEST(Checksum32, CrcOkLength) {
  EXPECT_FALSE(crc_ok(reinterpret_cast<const uint8_t *>("a\xC8\x08\x93\x1C"), 0));
  EXPECT_FALSE(crc_ok(reinterpret_cast<const uint8_t *>("\xC8\x08\x93\x1C"), 4));
  EXPECT_TRUE(crc_ok(reinterpret_cast<const uint8_t *>("a\xC8\x08\x93\x1C"), 5));
}
