#ifndef __FRAMING_H
#define __FRAMING_H
#include <stdint.h>

constexpr uint8_t MARK = 0xE2;
constexpr uint8_t ESC = 0x27;

uint32_t encodeFrame(uint8_t *source, uint32_t sourceLength, uint8_t *dest,
                     uint32_t destLength);
uint32_t decodeFrame(uint8_t *source, uint32_t sourceLength, uint8_t *dest,
                     uint32_t destLength);

#endif
