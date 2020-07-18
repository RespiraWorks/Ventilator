/* Copyright 2020, RespiraWorks

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

#include "debug.h"
#include "vars.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <array>
#include <stdint.h>
#include <vector>

namespace {
std::vector<uint8_t> Escape(const std::vector<uint8_t> &data) {
  std::vector<uint8_t> res;
  for (uint8_t ch : data) {
    if ((ch == static_cast<uint8_t>(DbgSpecial::TERM)) ||
        (ch == static_cast<uint8_t>(DbgSpecial::ESC))) {
      res.push_back(static_cast<char>(DbgSpecial::ESC));
    }
    res.push_back(ch);
  }
  return res;
}

std::vector<uint8_t> Unescape(const std::vector<uint8_t> &data) {
  std::vector<uint8_t> res;
  bool escape_next_byte;
  for (uint8_t ch : data) {
    if (escape_next_byte) {
      res.push_back(ch);
      escape_next_byte = false;
      continue;
    }
    if (ch == static_cast<uint8_t>(DbgSpecial::ESC)) {
      escape_next_byte = true;
      continue;
    }
    res.push_back(ch);
  }
  return res;
}

// Takes an unframed command request, i.e. command code + command payload,
// unescaped.
// Returns the unframed command response payload, i.e. unescaped,
// without the error code and crc.
std::vector<uint8_t> ProcessCmd(DebugSerial *serial, std::vector<uint8_t> req) {
  std::vector<uint8_t> full_req;
  full_req.reserve(/*ESC*/ 1 + req.size() + /*CRC*/ 2 + /*TERM*/ 1);
  full_req.push_back(static_cast<uint8_t>(DbgSpecial::ESC));
  for (uint8_t ch : Escape(req)) {
    full_req.push_back(ch);
  }
  uint16_t crc = DebugSerial::CalcCRC(req.data(), req.size());
  uint8_t crc_bytes[2];
  u16_to_u8(crc, &crc_bytes[0]);
  full_req.push_back(crc_bytes[0]);
  full_req.push_back(crc_bytes[1]);
  full_req.push_back(static_cast<uint8_t>(DbgSpecial::TERM));

  Hal.test_debugPutIncomingData(reinterpret_cast<const char *>(full_req.data()),
                                static_cast<uint16_t>(full_req.size()));
  for (int i = 0; i < 100 && !serial->Poll(); ++i) {
    // Wait for command to complete
  }

  std::vector<uint8_t> escaped_resp(500);
  uint16_t resp_len = Hal.test_debugGetOutgoingData(
      reinterpret_cast<char *>(escaped_resp.data()),
      static_cast<uint16_t>(escaped_resp.size()));
  escaped_resp.erase(escaped_resp.begin() + resp_len, escaped_resp.end());
  EXPECT_GE(escaped_resp.size(), size_t{3} /* err code + crc */);

  // Unescape response
  std::vector<uint8_t> resp = Unescape(escaped_resp);
  EXPECT_GE(resp.size(), size_t{3} /* err code + crc */);
  EXPECT_EQ(static_cast<uint8_t>(DbgSpecial::TERM), resp.back());
  resp.pop_back();

  // Verify error code and CRC
  EXPECT_EQ(static_cast<uint8_t>(DbgErrCode::OK), resp[0]);
  uint16_t expected_crc = DebugSerial::CalcCRC(resp.data(), resp.size() - 2);
  u16_to_u8(expected_crc, &crc_bytes[0]);
  uint16_t actual_crc = u8_to_u16(resp.data() + resp.size() - 2);
  EXPECT_EQ(expected_crc, actual_crc);

  // Trim the error code and CRC and return the raw command response
  resp.erase(resp.begin());
  resp.pop_back();
  resp.pop_back();

  return resp;
}

TEST(Debug, Mode) {
  DebugSerial serial;
  std::vector<uint8_t> req = {static_cast<uint8_t>(DbgCmdCode::MODE)};
  std::vector<uint8_t> resp = ProcessCmd(&serial, req);
  EXPECT_THAT(resp, testing::ElementsAre(static_cast<uint8_t>(0)));
}

uint32_t GetVarViaCmd(DebugSerial *serial, uint16_t id) {
  uint8_t vid[2];
  u16_to_u8(id, &vid[0]);

  std::vector<uint8_t> req = {
      static_cast<uint8_t>(DbgCmdCode::VAR), // Cmd code
      uint8_t{1},                            // GET
      vid[0], vid[1],                        // var id
  };

  std::vector<uint8_t> resp = ProcessCmd(serial, req);
  return u8_to_u32(resp.data());
}

TEST(Debug, GetVar) {
  uint32_t foo = 0xDEADBEEF;
  DebugVar var_foo("foo", &foo);
  uint32_t bar = 0xC0DEBABE;
  DebugVar var_bar("bar", &bar);

  DebugSerial serial;
  // Run a bunch of times with different expected results
  // to exercise buffer management.
  for (int i = 0; i < 100; ++i, ++foo, ++bar) {
    EXPECT_EQ(foo, GetVarViaCmd(&serial, var_foo.GetId()));
    EXPECT_EQ(bar, GetVarViaCmd(&serial, var_bar.GetId()));
  }
}
} // namespace
