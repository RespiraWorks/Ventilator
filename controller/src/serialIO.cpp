/* Copyright 2020, Edwin Chiu

  This file is part of FixMoreLungs.

  FixMoreLungs is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FixMoreLungs is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FixMoreLungs.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <Arduino.h>

#include "serialIO.h"

void serialIO_init() {
  // Initialize serial communications at 115200 bps.
  Serial.begin(115200, SERIAL_8N1);
}

bool serialIO_dataAvailable() {
  return (Serial.available() > 0) ? true : false;
}

void serialIO_readByte(char *buffer) {
  // NOTE: This assumes that a byte is ready in the buffer
  Serial.readBytes(buffer, 1);
}

void serialIO_send(enum msgType type, enum dataID id, char *data, uint8_t len) {
  char metadata[3] = {
      (char)((char)type & 0xff),
      (char)id,
      (char)len,
  };

  // Calculate checksum and check bytes.
  uint16_t csum = checksum_fletcher16(metadata, sizeof(metadata));
  csum = checksum_fletcher16(data, len, csum);
  uint16_t check_bytes = check_bytes_fletcher16(csum);

  // Send the packet: [DATA_TYPE, DATA_ID, LEN, DATA, check bytes].
  Serial.write(metadata, sizeof(metadata));
  Serial.write(data, len);
  Serial.write(static_cast<uint8_t>(check_bytes >> 8));
  Serial.write(static_cast<uint8_t>(check_bytes & 0xff));
}
