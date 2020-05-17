#include <stdint.h>

constexpr uint8_t MARK = 0xE2;
constexpr uint8_t ESC = 0x27;

inline bool shouldEscape(uint8_t b) { return MARK == b || ESC == b; }

uint32_t encodeFrame(uint8_t *source, uint_fast32_t sourceLength, uint8_t *dest,
                     uint_fast32_t destLength) {
  if (destLength <= sourceLength + 2 + 4) {
    return 0; // to buffer has to be bigger at least 2 marker and + 4 CRC bytes
  }

  uint32_t i = 0;
  dest[i++] = MARK;
  for (uint32_t j = 0; j < sourceLength; j++) {
    if (shouldEscape(source[j])) {
      dest[i++] = ESC;
      dest[i++] = source[j] ^ 0x20;
    } else {
      dest[i++] = source[j];
    }
  }
  dest[i++] = MARK;
  return i;
}

uint32_t decodeFrame(uint8_t *source, uint32_t sourceLength, uint8_t *dest,
                     uint32_t destLength) {
  uint32_t i = 0;
  bool isEsc = false;
  for (uint32_t j = 0; j < sourceLength; j++) {
    switch (source[j]) {
    case MARK:
      break;
    case ESC:
      isEsc = true;
      break;
    default:
      if (isEsc) {
        isEsc = false;
        dest[i++] = source[j] ^ 0x20;
      } else {
        dest[i++] = source[j];
      }
      break;
    }
  }
  return i;
}
