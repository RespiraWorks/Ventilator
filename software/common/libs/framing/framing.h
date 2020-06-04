#ifndef __FRAMING_H__
#define __FRAMING_H__

#include "checksum.h"
#include "framing_spec_chars.h"
#include "framing_streams.h"
#include "network_protocol.pb.h"
#include "proto_traits.h"
#include <pb_decode.h>
#include <pb_encode.h>
#include <stdint.h>

uint32_t UnescapeFrame(uint8_t *source, uint32_t sourceLength, uint8_t *dest,
                       uint32_t destLength) {
  uint32_t i = 0;
  bool isEsc = false;
  for (uint32_t j = 0; j < sourceLength; j++) {
    switch (source[j]) {
    case FRAMING_MARK:
      break;
    case FRAMING_ESC:
      isEsc = true;
      break;
    default:
      if (i >= destLength) {
        return 0;
      }
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

enum DecodeResult { SUCCESS, ERROR_FRAMING, ERROR_CRC, ERROR_PB };

// Decodes the given buffer into a protobuf object
// Unescapes and checks CRC
// @returns
// DecodeResult::SUCCESS on success
// DecodeResult::ERROR_FRAMING on unescaping error
// DecodeResult::ERROR_CRC on CRC mismatch
// DecodeResult::ERROR_PB on nanopb decode error
template <typename PbType>
DecodeResult DecodeFrame(uint8_t *buf, uint32_t len, PbType *pb_object) {
  uint32_t decoded_length = UnescapeFrame(buf, len, buf, len);
  if (0 == decoded_length) {
    return DecodeResult::ERROR_FRAMING;
  }
  if (!crc_ok(buf, decoded_length)) {
    return DecodeResult::ERROR_CRC;
  }
  pb_istream_t stream = pb_istream_from_buffer(buf, decoded_length - 4);
  if (!pb_decode(&stream, ProtoTraits<PbType>::MsgDesc, pb_object)) {
    return DecodeResult::ERROR_PB;
  }
  return DecodeResult::SUCCESS;
}

// Emulates the transmission process to count the number of bytes needed for
// encoding
template <typename PbType>
static uint32_t EncodedLength(uint8_t *buf, uint32_t len) {
  CounterStream counter_stream;
  EscapeStream esc_stream(counter_stream);
  CrcStream crc_stream(esc_stream);
  StreamResponse r = {0, STREAM_SUCCESS};
  for (uint32_t i = 0; i < len; i++) {
    r += crc_stream.Put(buf[i]);
  }
  r += crc_stream.Put(END_OF_STREAM);
  return r.count_written;
}

// Serializes given Protobuf object, adds crc and escapes it.
// The resulting byte stream is written to the OutputStream
// Returns number of bytes written.
template <typename PbType>
uint32_t EncodeFrame(const PbType &pb_object, OutputStream &output_stream) {
  uint8_t pb_buffer[ProtoTraits<PbType>::MaxSize];

  pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));
  if (!pb_encode(&stream, ProtoTraits<PbType>::MsgDesc, &pb_object)) {
    // TODO: Serialization failure; log an error or raise an alert.
    return 0;
  }
  uint32_t pb_length = (uint32_t)(stream.bytes_written);

  EscapeStream esc_stream(output_stream);
  CrcStream crc_stream(esc_stream);

  if (EncodedLength<PbType>(pb_buffer, pb_length) >
      output_stream.BytesAvailableForWrite()) {
    // We won't be able to fit in the whole frame this time and we don't want
    // any partial writes
    return 0;
  }

  StreamResponse r = {0, 0};
  for (uint32_t i = 0; i < pb_length; i++) {
    r += crc_stream.Put(pb_buffer[i]);
  }
  r += crc_stream.Put(END_OF_STREAM);
  return r.count_written;
}
#endif
