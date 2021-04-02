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

#include "commands.h"
#include "trace.h"
#include "gtest/gtest.h"
#include <array>

namespace Debug::Command {

static constexpr size_t kResponseSize{100};
// helper function that checks the buffer output
void CheckBufferOutput(std::array<uint8_t, kResponseSize> response,
                       uint32_t response_length, size_t sample_size,
                       uint32_t initial_value) {
  for (int i = 0; i < response_length / sample_size; ++i) {
    uint8_t expected_value[sample_size];
    u32_to_u8(static_cast<uint32_t>(i + initial_value), &expected_value[0]);
    u32_to_u8(static_cast<uint32_t>(10 * (i + initial_value)),
              &expected_value[4]);
    // if you add variables, you'll need to assign expected_value[8] and so on
    for (int j = 0; j < sample_size; ++j) {
      EXPECT_EQ(response[i * sample_size + j], expected_value[j]);
    }
  }
}

TEST(TraceHandler, Flush) {
  // define some debug variables
  uint32_t i = 0;
  FnDebugVar var_x(
      VarType::UINT32, "x", "", "", [&] { return i; },
      [&](uint32_t value) { (void)value; });
  FnDebugVar var_y(
      VarType::UINT32, "x", "", "", [&] { return i * 10; },
      [&](uint32_t value) { (void)value; });

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  // activate trace
  trace.SetTracedVarId<1>(var_x.GetId());
  trace.SetTracedVarId<3>(var_y.GetId());
  trace.Start(); // Enable tracing

  trace.MaybeSample();
  EXPECT_EQ(trace.GetNumSamples(), 1);
  EXPECT_TRUE(trace.GetStatus());

  std::array flush_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kFlush)};
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  Context flush_context = {.request = flush_command.data(),
                           .request_length = std::size(flush_command),
                           .response = response.data(),
                           .max_response_length = std::size(response),
                           .response_length = 0,
                           .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&flush_context));
  EXPECT_TRUE(processed);
  EXPECT_FALSE(trace.GetStatus());

  // Actual flush happens on the next start
  trace.Start();
  EXPECT_EQ(trace.GetNumSamples(), 0);
}

TEST(TraceHandler, Read) {
  static constexpr size_t kExtraSamples{5};

  // define some debug variables
  uint32_t i = 0;
  FnDebugVar var_x(
      VarType::UINT32, "x", "", "", [&] { return i; },
      [&](uint32_t value) { (void)value; });
  FnDebugVar var_y(
      VarType::UINT32, "y", "", "", [&] { return i * 10; },
      [&](uint32_t value) { (void)value; });

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  // read with no vars ==> nothing to report
  std::array read_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kDownload)};
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  Context read_context = {.request = read_command.data(),
                          .request_length = std::size(read_command),
                          .response = response.data(),
                          .max_response_length = kResponseSize,
                          .response_length = 0,
                          .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&read_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(read_context.response_length, 0);

  trace.SetTracedVarId<1>(var_x.GetId());
  trace.SetTracedVarId<3>(var_y.GetId());

  // start the trace (using debug command)
  std::array start_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kStart)};
  processed = false;
  Context start_context = {.request = start_command.data(),
                           .request_length = std::size(start_command),
                           .response = response.data(),
                           .max_response_length = kResponseSize,
                           .response_length = 0,
                           .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&start_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(start_context.response_length, 0);
  EXPECT_EQ(trace.GetStatus(), 1);

  // trace with no samples (tracing active) ==> nothing to report
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&read_context));
  EXPECT_EQ(read_context.response_length, 0);

  // fill the buffer to a bigger size than the max response length
  // note we are sampling 2 variables that are 4 bytes each
  uint32_t num_vars = trace.GetNumActiveVars();
  size_t sample_size = num_vars * sizeof(uint32_t);
  for (i = 0; i < kResponseSize / sample_size + kExtraSamples; ++i) {
    trace.MaybeSample();
  }

  // Take advantage of having a full buffer to test CountSamples command
  std::array num_samples_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kCountSamples)};
  processed = false;
  Context num_samples_context = {.request = num_samples_command.data(),
                                 .request_length =
                                     std::size(num_samples_command),
                                 .response = response.data(),
                                 .max_response_length = kResponseSize,
                                 .response_length = 0,
                                 .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&num_samples_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(num_samples_context.response_length, 4);
  EXPECT_EQ(trace.GetNumSamples(), u8_to_u32(response.data()));

  // issue a new read command
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&read_context));
  // we are only returning entire samples
  EXPECT_EQ(read_context.response_length,
            kResponseSize - kResponseSize % sample_size);
  size_t read_samples = read_context.response_length / sample_size;
  // check response
  CheckBufferOutput(response, read_context.response_length, sample_size, 0);

  // reset response pointer and response_length before issuing a new read
  read_context.response = response.data();
  read_context.response_length = 0;
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&read_context));
  EXPECT_EQ(read_context.response_length, kExtraSamples * sample_size);
  // check response
  CheckBufferOutput(response, read_context.response_length, sample_size,
                    static_cast<uint32_t>(read_samples));

  // expect trace to still be running but with no samples in buffer
  EXPECT_TRUE(trace.GetStatus());
  EXPECT_EQ(trace.GetNumSamples(), 0);
}

TEST(TraceHandler, SettersAndGetters) {
  // define debug variables
  DebugUInt32 var_x("x");
  DebugUInt32 var_y("y");

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  // Set trace var ID for var 1
  std::array<uint8_t, 4> set_var1_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kSetVarId), 1, 0, 0};
  u16_to_u8(var_y.GetId(), &set_var1_command[2]);
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  Context set_var1_context = {.request = set_var1_command.data(),
                              .request_length = std::size(set_var1_command),
                              .response = response.data(),
                              .max_response_length = kResponseSize,
                              .response_length = 0,
                              .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&set_var1_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(set_var1_context.response_length, 0);
  // check that the trace now points to desired var
  EXPECT_EQ(u8_to_u16(&set_var1_command[2]), trace.GetTracedVarId<1>());

  // Get trace var ID for var 1
  std::array<uint8_t, 2> get_var1_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kGetVarId), 1};
  processed = false;
  Context get_var1_context = {.request = get_var1_command.data(),
                              .request_length = std::size(get_var1_command),
                              .response = response.data(),
                              .max_response_length = kResponseSize,
                              .response_length = 0,
                              .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&get_var1_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_var1_context.response_length, 2);

  EXPECT_EQ(trace.GetTracedVarId<1>(), u8_to_u16(response.data()));

  // Get trace var ID for var 2 (un-associated)
  std::array<uint8_t, 2> get_var2_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kGetVarId), 2};
  processed = false;
  Context get_var2_context = {.request = get_var2_command.data(),
                              .request_length = std::size(get_var2_command),
                              .response = response.data(),
                              .max_response_length = kResponseSize,
                              .response_length = 0,
                              .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&get_var2_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_var2_context.response_length, 2);

  EXPECT_EQ(uint16_t(-1), u8_to_u16(response.data()));

  // Get trace var ID for var kMaxTraceVars (out of bounds)
  std::array<uint8_t, 2> get_var_max_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kGetVarId),
      static_cast<uint8_t>(kMaxTraceVars)};
  processed = false;
  Context get_var_max_context = {.request = get_var_max_command.data(),
                                 .request_length =
                                     std::size(get_var_max_command),
                                 .response = response.data(),
                                 .max_response_length = kResponseSize,
                                 .response_length = 0,
                                 .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&get_var_max_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_var_max_context.response_length, 2);
  EXPECT_EQ(uint16_t(-1), u8_to_u16(response.data()));

  // Set trace period
  std::array<uint8_t, 5> set_period_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kSetPeriod)};
  u32_to_u8(2, &set_period_command[1]);
  processed = false;
  Context set_period_context = {.request = set_period_command.data(),
                                .request_length = std::size(set_period_command),
                                .response = response.data(),
                                .max_response_length = kResponseSize,
                                .response_length = 0,
                                .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&set_period_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(set_period_context.response_length, 0);

  EXPECT_EQ(trace.GetPeriod(), u8_to_u32(&set_period_command[1]));

  // Get trace period
  std::array get_period_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::kGetPeriod)};
  processed = false;
  Context get_period_context = {.request = get_period_command.data(),
                                .request_length = std::size(get_period_command),
                                .response = response.data(),
                                .max_response_length = kResponseSize,
                                .response_length = 0,
                                .processed = &processed};
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&get_period_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_period_context.response_length, 4);

  EXPECT_EQ(trace.GetPeriod(), u8_to_u32(get_period_context.response));
}

TEST(TraceHandler, Errors) {
  // define some debug variables
  DebugUInt32 var_x("x");
  DebugUInt32 var_y("y");

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  trace.SetTracedVarId<1>(var_x.GetId());
  trace.SetTracedVarId<3>(var_y.GetId());
  trace.Start();

  std::vector<std::tuple<std::vector<uint8_t>, ErrorCode>> requests = {
      {{}, ErrorCode::kMissingData},  // Missing subcommand
      {{8}, ErrorCode::kInvalidData}, // Invalid subcommand
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kDownload)},
       ErrorCode::kNoMemory},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kSetVarId), 1, 1},
       ErrorCode::kMissingData},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kSetVarId),
        kMaxTraceVars, 1, 0},
       ErrorCode::kInvalidData},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kGetVarId)},
       ErrorCode::kMissingData},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kGetVarId), 1},
       ErrorCode::kNoMemory},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kSetPeriod), 1, 1, 1},
       ErrorCode::kMissingData},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kGetPeriod)},
       ErrorCode::kNoMemory},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::kCountSamples)},
       ErrorCode::kNoMemory},
  };
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  for (auto &[request, error] : requests) {
    Context context = {.request = request.data(),
                       .request_length = static_cast<uint32_t>(request.size()),
                       .response = response.data(),
                       .max_response_length =
                           1, // To provoke the No Memory error once all other
                              // checks have passed
                       .response_length = 0,
                       .processed = &processed};
    EXPECT_EQ(error, trace_handler.Process(&context));
    EXPECT_FALSE(processed);
    EXPECT_EQ(context.response_length, 0);
  }
}

} // namespace Debug::Command
