#include "framing.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <stdlib.h>

void randomConversion() {
  constexpr uint32_t SLENGTH = 150;
  constexpr uint32_t DLENGTH = (SLENGTH + 4) * 2 + 2 + 1;
  uint8_t source_buf[SLENGTH];
  uint8_t dest_buf[DLENGTH];
  uint8_t decoded_buf[SLENGTH];
  for (unsigned int i = 0; i < SLENGTH; i++) {
    source_buf[i] = static_cast<uint8_t>(rand() % 255);
  }

  uint32_t frameLength = encodeFrame(source_buf, SLENGTH, dest_buf, DLENGTH);
  ASSERT_GT(frameLength, SLENGTH);
  uint32_t decodedLength =
      decodeFrame(dest_buf, frameLength, decoded_buf, SLENGTH);
  ASSERT_EQ(decodedLength, SLENGTH);
  int n = memcmp(source_buf, decoded_buf, SLENGTH);
  ASSERT_EQ(n, 0);
}

TEST(FramingTests, RandomBuffer) {
  for (int i = 0; i < 1; i++) {
    randomConversion();
  }
}

TEST(FramingTests, EncodingDestTooSmall) {
  uint8_t source_buf[] = {0, 1, 2, 3};
  uint8_t dest_buf[20];

  uint32_t frameLength =
      encodeFrame(source_buf, sizeof(source_buf), dest_buf, 5);
  ASSERT_EQ(frameLength, (uint32_t)0);
  frameLength = encodeFrame(source_buf, sizeof(source_buf), dest_buf, 6);
  ASSERT_GT(frameLength, (uint32_t)0);

  uint8_t source_buf2[] = {0, ESC, 1, MARK, 2, 3};

  frameLength = encodeFrame(source_buf2, sizeof(source_buf2), dest_buf, 7);
  ASSERT_EQ(frameLength, (uint32_t)0);
  frameLength = encodeFrame(source_buf2, sizeof(source_buf2), dest_buf, 10);
  ASSERT_GT(frameLength, (uint32_t)0);
}

TEST(FramingTests, DecodingDestTooSmall) {
  uint8_t source_buf[] = {MARK, 0, 1, 2, 3, MARK};
  uint8_t dest_buf[10];

  uint32_t frameLength =
      decodeFrame(source_buf, sizeof(source_buf), dest_buf, 3);
  ASSERT_EQ(frameLength, (uint32_t)0);
  frameLength = decodeFrame(source_buf, sizeof(source_buf), dest_buf, 4);
  ASSERT_GT(frameLength, (uint32_t)0);
}
