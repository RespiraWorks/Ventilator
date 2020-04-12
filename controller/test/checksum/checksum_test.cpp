#include <stdint.h>

#include "unity.h"
#include "checksum.h"

void test_Empty() {
  uint16_t sum1 = 0;
  uint16_t sum2 = 0;
  uint16_t actual = checksum_fletcher16(&sum1, &sum2, /*data=*/NULL, /*count=*/0);
  TEST_ASSERT_EQUAL_INT16(0, actual);
}

void test_OneByte() {
  uint16_t sum1 = 0;
  uint16_t sum2 = 0;
  char data[1] = {'a'};
  uint16_t actual = checksum_fletcher16(&sum1, &sum2, data, /*count=*/1);
  TEST_ASSERT_EQUAL_INT16(24929, actual);
}

int main () {
    UNITY_BEGIN();
    RUN_TEST(test_Empty);
    RUN_TEST(test_OneByte);
    return UNITY_END();
}
