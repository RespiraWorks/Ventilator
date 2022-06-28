/* Copyright 2020-2022, RespiraWorks
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

#pragma once

#include <pb_decode.h>
#include <pb_encode.h>

#include <cstdint>

#include "checksum.h"
#include "framing_streams.h"
#include "network_protocol.pb.h"
#include "proto_traits.h"

inline size_t UnescapeFrame(uint8_t *source, size_t source_length, uint8_t *destination,
                            size_t destination_length) {
  size_t i = 0;
  bool is_escape = false;
  for (size_t j = 0; j < source_length; j++) {
    switch (source[j]) {
      case FramingMark:
        break;
      case FramingEscape:
        is_escape = true;
        break;
      default:
        if (i >= destination_length) {
          return 0;
        }
        if (is_escape) {
          is_escape = false;
          destination[i++] = EscapedChar(source[j]);
        } else {
          destination[i++] = source[j];
        }
        break;
    }
  }
  return i;
}

enum class DecodeResult : uint32_t { Success, ErrorFraming, ErrorCRC, ErrorSerialization };

// Decodes the given buffer into a protobuf object.
// Unescapes and checks CRC
// \returns
// DecodeResult::Success on success
// DecodeResult::ErrorFraming on unescaping error
// DecodeResult::ErrorCRC on CRC mismatch
// DecodeResult::ErrorSerialization on nanopb decode error
template <typename PbType>
DecodeResult DecodeFrame(uint8_t *buffer, size_t length, PbType *pb_object) {
  size_t decoded_length = UnescapeFrame(buffer, length, buffer, length);
  if (0 == decoded_length) {
    return DecodeResult::ErrorFraming;
  }
  if (!crc_ok(buffer, decoded_length)) {
    return DecodeResult::ErrorCRC;
  }
  pb_istream_t stream = pb_istream_from_buffer(buffer, decoded_length - 4);
  if (!pb_decode(&stream, ProtoTraits<PbType>::MsgDesc, pb_object)) {
    return DecodeResult::ErrorSerialization;
  }
  return DecodeResult::Success;
}

// Emulates the transmission process to count the number of bytes needed for encoding
template <typename PbType>
static size_t EncodedLength(uint8_t *buffer, size_t length) {
  CounterStream counter_stream;
  EscapeStream esc_stream(counter_stream);
  CrcStream crc_stream(esc_stream);
  StreamResponse r = {0, ResponseFlags::StreamSuccess};
  for (size_t i = 0; i < length; i++) {
    r += crc_stream.put(buffer[i]);
  }
  r += crc_stream.put(EndOfStream);
  return r.Count();
}

// Serializes given Protobuf object, adds crc and escapes it.
// The resulting byte stream is written to the OutputStream
// Returns number of bytes written.
template <typename PbType>
size_t EncodeFrame(const PbType &pb_object, OutputStream &output_stream) {
  uint8_t pb_buffer[ProtoTraits<PbType>::MaxSize];

  pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));
  if (!pb_encode(&stream, ProtoTraits<PbType>::MsgDesc, &pb_object)) {
    // TODO: Serialization failure; log an error or raise an alert.
    return 0;
  }
  size_t pb_length{stream.bytes_written};

  EscapeStream esc_stream(output_stream);
  CrcStream crc_stream(esc_stream);

  if (EncodedLength<PbType>(pb_buffer, pb_length) > output_stream.bytes_available_for_write()) {
    // We won't be able to fit in the whole frame this time, and we don't want any partial writes
    return 0;
  }

  StreamResponse ret;
  for (size_t i = 0; i < pb_length; i++) {
    ret += crc_stream.put(pb_buffer[i]);
  }
  ret += crc_stream.put(EndOfStream);
  return ret.Count();
}
