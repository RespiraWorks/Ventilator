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

#include "serialIO.h"

void serialIO_init() {
	// initialize serial communications at 115200 bps
    Serial.begin(115200, SERIAL_8N1);
}

void serialIO_send(enum data_type type, enum data_id id, unsigned char *data, uint8_t len) {

    unsigned char metadata[3];
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    uint16_t csum;
    uint8_t c0,c1,f0,f1;

    metadata[0] = ((unsigned char ) type) & 0xff; // DATA_TYPE
    metadata[1] = (unsigned char )  id; // DATA_ID
    metadata[2] = (unsigned char )  len; // LEN

    
    Fletcher16_calc(&sum1, &sum2, metadata, sizeof(metadata));
    csum = Fletcher16_calc(&sum1, &sum2, data, len);

    // Calculate check bytes
    // TODO Can this be optimised?
    f0 = csum & 0xff;
    f1 = (csum >> 8) & 0xff;
    c0 = 0xff - ((f0 + f1) % 0xff);
    c1 = 0xff - ((f0 + c0) % 0xff);
    
    Serial.write(metadata, sizeof(metadata));  // Send DATA_TYPE, DATA_ID, LEN
    Serial.write(data, len);  // Send DATA

    // Send checksum
    Serial.write(c0);
    Serial.write(c1);
}


static uint16_t Fletcher16_calc(uint16_t *sum1, uint16_t *sum2,  unsigned char *data, uint8_t count )
{
    uint8_t index;

    for (index = 0; index < count; ++index)
    {
        *sum1 = ((*sum1) + data[index]) % 255;
        *sum2 = ((*sum2) + (*sum1)) % 255;
    }

    return ((*sum2) << 8) | *sum1;
}
