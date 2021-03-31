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
// check response helper function
void CheckResponse(std::array<uint8_t, kResponseSize> response,
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

  std::array flush_command = {static_cast<uint8_t>(Subcommand::kFlushTrace)};
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
      VarType::UINT32, "x", "", "", [&] { return i * 10; },
      [&](uint32_t value) { (void)value; });

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  // read with no vars ==> nothing to report
  std::array read_command = {static_cast<uint8_t>(Subcommand::kDownloadTrace)};
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

  trace.Start();
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
  // issue a new read command
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&read_context));
  // we are only returning entire samples
  EXPECT_EQ(read_context.response_length,
            kResponseSize - kResponseSize % sample_size);
  size_t read_samples = read_context.response_length / sample_size;
  // check response
  CheckResponse(response, read_context.response_length, sample_size, 0);

  // reset response pointer and response_length before issuing a new read
  read_context.response = response.data();
  read_context.response_length = 0;
  EXPECT_EQ(ErrorCode::kNone, trace_handler.Process(&read_context));
  EXPECT_EQ(read_context.response_length, kExtraSamples * sample_size);
  // check response
  CheckResponse(response, read_context.response_length, sample_size,
                static_cast<uint32_t>(read_samples));

  // expect trace to still be running but with no samples in buffer
  EXPECT_TRUE(trace.GetStatus());
  EXPECT_EQ(trace.GetNumSamples(), 0);
}

TEST(TraceHandler, Errors) {
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

  trace.SetTracedVarId<1>(var_x.GetId());
  trace.SetTracedVarId<3>(var_y.GetId());
  trace.Start();

  // note we are sampling 2 variables that are 4 bytes each
  uint32_t num_vars = trace.GetNumActiveVars();
  size_t sample_size = num_vars * sizeof(uint32_t);

  std::vector<std::tuple<std::vector<uint8_t>, ErrorCode>> requests = {
      {{}, ErrorCode::kMissingData},  // Missing subcommand
      {{3}, ErrorCode::kInvalidData}, // Invalid subcommand
      {{1}, ErrorCode::kNoMemory},
  };
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  for (auto &[request, error] : requests) {
    Context context = {.request = request.data(),
                       .request_length = static_cast<uint32_t>(request.size()),
                       .response = response.data(),
                       .max_response_length =
                           static_cast<uint32_t>(sample_size - 1),
                       // To provoke the No Memory error once all other
                       // checks have passed
                       .response_length = 0,
                       .processed = &processed};
    EXPECT_EQ(error, trace_handler.Process(&context));
    EXPECT_FALSE(processed);
    EXPECT_EQ(context.response_length, 0);
  }
}

} // namespace Debug::Command
