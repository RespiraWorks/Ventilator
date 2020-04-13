#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>

#include "checksum.h"
#include "unity.h"

void test_KnownQuantities() {
  TEST_ASSERT_EQUAL_INT16(0, checksum_fletcher16(NULL, 0));
  TEST_ASSERT_EQUAL_INT16(0, checksum_fletcher16("", 0));
  TEST_ASSERT_EQUAL_INT16(24929, checksum_fletcher16("a", 1));
  TEST_ASSERT_EQUAL_INT16(51440, checksum_fletcher16("abcde", 5));
  TEST_ASSERT_EQUAL_INT16(8279, checksum_fletcher16("abcdef", 6));
  TEST_ASSERT_EQUAL_INT16(1575, checksum_fletcher16("abcdefgh", 8));
  TEST_ASSERT_EQUAL_INT16(0xfefe, checksum_fletcher16("\xff\xfe", 2));
}

void test_CheckBytes() {
  uint16_t csum = checksum_fletcher16("abcde", 5);

  uint16_t checkBytes = check_bytes_fletcher16(csum);
  char c0 = checkBytes >> 8;
  char c1 = checkBytes & 0xff;
  csum = checksum_fletcher16(&c0, 1, csum);
  csum = checksum_fletcher16(&c1, 1, csum);
  TEST_ASSERT_EQUAL_INT16(csum, 0);
}

// Repeatedly flip a random bit in `data` and check that
//  - a one-bit flip never yields the same checksum, and
//  - the same checksum doesn't appear "too often" overall.
void test_BitFlips() {
  srand(0);

  char data[32];
  memset(&data, '\0', sizeof(data));

  // Start lastChecksum at -1 because our first test will be checksum'ing the
  // all-zeroes buffer, which should yield 0.
  uint16_t lastChecksum = -1;

  const int32_t numTests = int32_t{1} << 16;
  int32_t singleBitFlipCollisions = 0;
  std::map<int16_t, int32_t> collisions;
  for (int32_t i = 0; i < numTests; ++i) {
    uint16_t checksum = checksum_fletcher16(data, sizeof(data));
    collisions[checksum]++;
    if (checksum == lastChecksum) {
      singleBitFlipCollisions++;
    }

    lastChecksum = checksum;
    int bitToFlip = rand() % (8 * sizeof(data));
    uint8_t byteMask = 1 << (bitToFlip % 8);
    data[bitToFlip / 8] ^= byteMask;
  }

  printf("%d/%d collisions after flipping a single bit.\n",
         singleBitFlipCollisions, numTests);
  if (singleBitFlipCollisions > 0) {
    TEST_FAIL_MESSAGE("Got at least one collision from one-bit flips; is the "
                      "checksum broken?\n");
  }

  // Find the checksum with the most collisions.  There shouldn't be "too
  // many".
  using MapElem = decltype(*collisions.begin());
  auto it = std::max_element(
      collisions.begin(), collisions.end(),
      [](MapElem a, MapElem b) { return a.second < b.second; });
  int16_t maxCollisionHash = it->first;
  int32_t maxCollisions = it->second;
  double maxCollisionsFrac = 1.0 * maxCollisions / numTests;
  printf("%d/%d collisions on worst checksum, 0x%4x\n", maxCollisions, numTests,
         maxCollisionHash);
  if (maxCollisionsFrac >= 0.0002) {
    TEST_FAIL_MESSAGE(
        "Too many collisions on worst checksum; is the checksum broken?");
  }
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_KnownQuantities);
  RUN_TEST(test_BitFlips);
  return UNITY_END();
}
