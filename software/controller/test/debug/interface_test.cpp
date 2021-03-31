/* Copyright 2020-2021, RespiraWorks

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

#include "binary_utils.h"
#include "commands.h"
#include "interface.h"
#include "vars.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include <array>
#include <stdint.h>
#include <vector>

namespace Debug {

std::vector<uint8_t> Escape(const std::vector<uint8_t> &data) {
  std::vector<uint8_t> res;
  for (uint8_t ch : data) {
    if ((ch == static_cast<uint8_t>(SpecialChar::kEndTransfer)) ||
        (ch == static_cast<uint8_t>(SpecialChar::kEscape))) {
      res.push_back(static_cast<char>(SpecialChar::kEscape));
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
    if (ch == static_cast<uint8_t>(SpecialChar::kEscape)) {
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
std::vector<uint8_t> ProcessCmd(Interface *serial, std::vector<uint8_t> req,
                                ErrorCode expected_error = ErrorCode::kNone,
                                uint16_t crc_offset = 0) {
  std::vector<uint8_t> full_req;
  full_req.reserve(/*ESC*/ 1 + req.size() + /*CRC*/ 2 + /*TERM*/ 1);
  full_req.push_back(static_cast<uint8_t>(SpecialChar::kEscape));
  for (uint8_t ch : Escape(req)) {
    full_req.push_back(ch);
  }
  uint16_t crc = static_cast<uint16_t>(
      Interface::ComputeCRC(req.data(), req.size()) + crc_offset);
  uint8_t crc_bytes[2];
  u16_to_u8(crc, &crc_bytes[0]);
  full_req.push_back(crc_bytes[0]);
  full_req.push_back(crc_bytes[1]);
  full_req.push_back(static_cast<uint8_t>(SpecialChar::kEndTransfer));

  Hal.test_debugPutIncomingData(reinterpret_cast<const char *>(full_req.data()),
                                static_cast<uint16_t>(full_req.size()));
  for (int i = 0; i < 100 && !serial->Poll(); ++i) {
    // Wait for command to complete, advance sim time to allow timeout
    Hal.delay(milliseconds(10));
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
  EXPECT_EQ(static_cast<uint8_t>(SpecialChar::kEndTransfer), resp.back());
  resp.pop_back();

  // Verify error code and CRC
  EXPECT_EQ(static_cast<uint8_t>(expected_error), resp[0]);
  uint16_t expected_crc = Interface::ComputeCRC(resp.data(), resp.size() - 2);
  u16_to_u8(expected_crc, &crc_bytes[0]);
  uint16_t actual_crc = u8_to_u16(resp.data() + resp.size() - 2);
  EXPECT_EQ(expected_crc, actual_crc);

  // Trim the error code and CRC and return the raw command response
  resp.erase(resp.begin());
  resp.pop_back();
  resp.pop_back();

  // Once completed, the interface is in wait mode with no incoming data
  EXPECT_FALSE(serial->Poll());

  return resp;
}

TEST(Interface, Mode) {
  Trace trace;
  Command::ModeHandler mode_command;
  Interface serial(&trace, 2, Command::Code::kMode, &mode_command);

  std::vector<uint8_t> req = {static_cast<uint8_t>(Command::Code::kMode)};
  std::vector<uint8_t> resp = ProcessCmd(&serial, req);
  EXPECT_THAT(resp, testing::ElementsAre(static_cast<uint8_t>(0)));
}

uint32_t GetVarViaCmd(Interface *serial, uint16_t id) {
  uint8_t vid[2];
  u16_to_u8(id, &vid[0]);

  std::vector<uint8_t> req = {
      static_cast<uint8_t>(Command::Code::kVariable), // Cmd code
      uint8_t{1},                                     // GET
      vid[0], vid[1],                                 // var id
  };

  std::vector<uint8_t> resp = ProcessCmd(serial, req);
  return u8_to_u32(resp.data());
}

TEST(Interface, GetVar) {
  uint32_t foo = 0xDEADBEEF;
  DebugVar var_foo("foo", &foo);
  uint32_t bar = 0xC0DEBABE;
  DebugVar var_bar("bar", &bar);

  Trace trace;
  Command::VarHandler var_command;
  Interface serial(&trace, 2, Command::Code::kVariable, &var_command);
  // Run a bunch of times with different expected results
  // to exercise buffer management.
  for (int i = 0; i < 100; ++i, ++foo, ++bar) {
    EXPECT_EQ(foo, GetVarViaCmd(&serial, var_foo.GetId()));
    EXPECT_EQ(bar, GetVarViaCmd(&serial, var_bar.GetId()));
  }
}

TEST(Interface, AwaitingResponseState) {
  Trace trace;
  TestEeprom eeprom_test(0x50, 64, 4096);
  Command::EepromHandler eeprom_command(&eeprom_test);
  Interface serial(&trace, 2, Command::Code::kEepromAccess, &eeprom_command);
  // EEPROM read command needs time to be processed
  std::vector<uint8_t> req = {
      static_cast<uint8_t>(Command::Code::kEepromAccess),
      0, // Read subcommand
      0, // address Least Significant Byte
      0, // address Most Significant Byte
      1, // length LSB
      0, // length MSB
  };

  // The helper function quietly passes through the AwaitingResponse state since
  // the test EEPROM sets processed to true immediately if the address is valid.
  std::vector<uint8_t> resp = ProcessCmd(&serial, req, ErrorCode::kNone);
  EXPECT_EQ(resp.size(), 1);

  // Requesting outside of memory leads to a timeout (test eeprom never sets
  // processed to true)
  req[3] = 0xFF;
  req[4] = 0xFF;
  resp = ProcessCmd(&serial, req, ErrorCode::kTimeout);
  EXPECT_EQ(resp.size(), 0);
}

TEST(Interface, Errors) {
  Trace trace;
  TestEeprom eeprom_test(0x50, 64, 4096);
  Command::ModeHandler mode_command;
  Command::PeekHandler peek_command;
  Command::PokeHandler poke_command;
  Command::VarHandler var_command;
  Command::TraceHandler trace_command(&trace);
  Command::EepromHandler eeprom_command(&eeprom_test);

  Debug::Interface serial(
      &trace, 12, Debug::Command::Code::kMode, &mode_command,
      Debug::Command::Code::kPeek, &peek_command, Debug::Command::Code::kPoke,
      &poke_command, Debug::Command::Code::kVariable, &var_command,
      Debug::Command::Code::kTrace, &trace_command,
      Debug::Command::Code::kEepromAccess, &eeprom_command);
  // Unknown command - If we ever develop new commands, make sure the command
  // code is too big.
  std::vector<uint8_t> req = {25};
  std::vector<uint8_t> resp =
      ProcessCmd(&serial, req, ErrorCode::kUnknownCommand);
  EXPECT_EQ(resp.size(), 0);

  // CRC Error
  req = {25};
  resp = ProcessCmd(&serial, req, ErrorCode::kCrcError, 1);
  EXPECT_EQ(resp.size(), 0);

  // Error returned by Command Handler
  req = {
      static_cast<uint8_t>(Command::Code::kVariable), // Cmd code
      1, 0xFF, 0xFF,                                  // GET and var id
  };
  resp = ProcessCmd(&serial, req, ErrorCode::kUnknownVariable);
  EXPECT_EQ(resp.size(), 0);

  // Command too short - we can't use the helper function for this as it adds
  // all the necessary data
  req = {1, 0, static_cast<uint8_t>(SpecialChar::kEndTransfer)};
  Hal.test_debugPutIncomingData(reinterpret_cast<const char *>(req.data()),
                                static_cast<uint16_t>(req.size()));
  for (int i = 0; i < 5 && !serial.Poll(); ++i) {
    // Wait for command to complete
  }
  // In that case, we actually expect no response
  uint16_t resp_len =
      Hal.test_debugGetOutgoingData(reinterpret_cast<char *>(resp.data()), 10);
  EXPECT_EQ(resp_len, 0);
}
} // namespace Debug
