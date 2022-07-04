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

#include <array>

#include "commands.h"
#include "gtest/gtest.h"
#include "trace.h"

namespace Debug::Command {

static constexpr size_t kResponseSize{100};
// helper function that checks the buffer output
void CheckBufferOutput(std::array<uint8_t, kResponseSize> response, size_t response_length,
                       size_t sample_size, uint32_t initial_value) {
  for (int i = 0; i < response_length / sample_size; ++i) {
    uint8_t expected_value[sample_size];
    u32_to_u8(static_cast<uint32_t>(i + initial_value), &expected_value[0]);
    u32_to_u8(static_cast<uint32_t>(10 * (i + initial_value)), &expected_value[4]);
    // if you add variables, you'll need to assign expected_value[8] and so on
    for (int j = 0; j < sample_size; ++j) {
      EXPECT_EQ(response[i * sample_size + j], expected_value[j]);
    }
  }
}

TEST(TraceHandler, Flush) {
  // define some debug variables
  uint32_t i = 0;
  Debug::Variable::FnVar32 var_x(
      Debug::Variable::Type::UInt32, "x", Debug::Variable::Access::ReadOnly, "unit",
      [&](void *write_buff) { std::memcpy(write_buff, &i, 4); },
      [&](const void *read_buf) { (void)read_buf; }, "");
  Debug::Variable::FnVar32 var_y(
      Debug::Variable::Type::UInt32, "x", Debug::Variable::Access::ReadOnly, "unit",
      [&](void *write_buff) {
        auto ii = 10 * i;
        std::memcpy(write_buff, &ii, 4);
      },
      [&](const void *read_buf) { (void)read_buf; }, "");

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  // activate trace
  trace.set_traced_variable(1, var_x.id());
  trace.set_traced_variable(3, var_y.id());
  trace.start();  // Enable tracing

  trace.maybe_sample();
  EXPECT_EQ(trace.sample_count(), 1);
  EXPECT_TRUE(trace.running());

  std::array flush_command = {static_cast<uint8_t>(TraceHandler::Subcommand::Flush)};
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  Context flush_context = {.request = flush_command.data(),
                           .request_length = std::size(flush_command),
                           .response = response.data(),
                           .max_response_length = std::size(response),
                           .response_length = 0,
                           .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&flush_context));
  EXPECT_EQ(trace.sample_count(), 0);
  EXPECT_TRUE(processed);
  EXPECT_TRUE(trace.running());
  trace.maybe_sample();
  EXPECT_EQ(trace.sample_count(), 1);

  std::array stop_command = {static_cast<uint8_t>(TraceHandler::Subcommand::Stop)};
  Context stop_context = {.request = stop_command.data(),
                          .request_length = std::size(stop_command),
                          .response = response.data(),
                          .max_response_length = std::size(response),
                          .response_length = 0,
                          .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&stop_context));
  EXPECT_EQ(trace.sample_count(), 1);
  EXPECT_TRUE(processed);
  EXPECT_FALSE(trace.running());
  trace.maybe_sample();
  EXPECT_EQ(trace.sample_count(), 1);
}

TEST(TraceHandler, Read) {
  static constexpr size_t kExtraSamples{5};

  // define some debug variables
  uint32_t i = 0;
  Debug::Variable::FnVar32 var_x(
      Debug::Variable::Type::UInt32, "x", Debug::Variable::Access::ReadOnly, "unit",
      [&](void *write_buff) { std::memcpy(write_buff, &i, 4); },
      [&](const void *read_buf) { (void)read_buf; }, "");
  Debug::Variable::FnVar32 var_y(
      Debug::Variable::Type::UInt32, "y", Debug::Variable::Access::ReadOnly, "unit",
      [&](void *write_buff) {
        auto ii = 10 * i;
        std::memcpy(write_buff, &ii, 4);
      },
      [&](const void *read_buf) { (void)read_buf; }, "");

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  // read with no vars ==> nothing to report
  std::array read_command = {static_cast<uint8_t>(TraceHandler::Subcommand::Download)};
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  Context read_context = {.request = read_command.data(),
                          .request_length = std::size(read_command),
                          .response = response.data(),
                          .max_response_length = kResponseSize,
                          .response_length = 0,
                          .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&read_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(read_context.response_length, 0);

  trace.set_traced_variable(1, var_x.id());
  trace.set_traced_variable(3, var_y.id());

  // start the trace (using debug command)
  std::array start_command = {static_cast<uint8_t>(TraceHandler::Subcommand::Start)};
  processed = false;
  Context start_context = {.request = start_command.data(),
                           .request_length = std::size(start_command),
                           .response = response.data(),
                           .max_response_length = kResponseSize,
                           .response_length = 0,
                           .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&start_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(start_context.response_length, 0);
  EXPECT_EQ(trace.running(), 1);

  // trace with no samples (tracing active) ==> nothing to report
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&read_context));
  EXPECT_EQ(read_context.response_length, 0);

  // fill the buffer to a bigger size than the max response length
  // note we are sampling 2 variables that are 4 bytes each
  uint32_t num_vars = trace.active_variable_count();
  size_t sample_size = num_vars * sizeof(uint32_t);
  for (i = 0; i < kResponseSize / sample_size + kExtraSamples; ++i) {
    trace.maybe_sample();
  }

  // Take advantage of having a full buffer to test CountSamples command
  std::array num_samples_command = {static_cast<uint8_t>(TraceHandler::Subcommand::CountSamples)};
  processed = false;
  Context num_samples_context = {.request = num_samples_command.data(),
                                 .request_length = std::size(num_samples_command),
                                 .response = response.data(),
                                 .max_response_length = kResponseSize,
                                 .response_length = 0,
                                 .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&num_samples_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(num_samples_context.response_length, 4);
  EXPECT_EQ(trace.sample_count(), u8_to_u32(response.data()));

  // issue a new read command
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&read_context));
  // we are only returning entire samples
  EXPECT_EQ(read_context.response_length, kResponseSize - kResponseSize % sample_size);
  size_t read_samples = read_context.response_length / sample_size;
  // check response
  CheckBufferOutput(response, read_context.response_length, sample_size, 0);

  // reset response pointer and response_length before issuing a new read
  read_context.response = response.data();
  read_context.response_length = 0;
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&read_context));
  EXPECT_EQ(read_context.response_length, kExtraSamples * sample_size);
  // check response
  CheckBufferOutput(response, read_context.response_length, sample_size,
                    static_cast<uint32_t>(read_samples));

  // expect trace to still be running but with no samples in buffer
  EXPECT_TRUE(trace.running());
  EXPECT_EQ(trace.sample_count(), 0);
}

TEST(TraceHandler, SettersAndGetters) {
  // define debug variables
  Debug::Variable::UInt32 var_x("x", Debug::Variable::Access::ReadOnly, 0, "unit");
  Debug::Variable::UInt32 var_y("y", Debug::Variable::Access::ReadOnly, 0, "unit");

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  // Set trace var ID for var 1
  std::array<uint8_t, 4> set_var1_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::SetVarId), 1, 0, 0};
  u16_to_u8(var_y.id(), &set_var1_command[2]);
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  Context set_var1_context = {.request = set_var1_command.data(),
                              .request_length = std::size(set_var1_command),
                              .response = response.data(),
                              .max_response_length = kResponseSize,
                              .response_length = 0,
                              .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&set_var1_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(set_var1_context.response_length, 0);
  // check that the trace now points to desired var
  EXPECT_EQ(u8_to_u16(&set_var1_command[2]), trace.traced_variable(1));

  // Get trace var ID for var 1
  std::array<uint8_t, 2> get_var1_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::GetVarId), 1};
  processed = false;
  Context get_var1_context = {.request = get_var1_command.data(),
                              .request_length = std::size(get_var1_command),
                              .response = response.data(),
                              .max_response_length = kResponseSize,
                              .response_length = 0,
                              .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&get_var1_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_var1_context.response_length, 2);

  EXPECT_EQ(trace.traced_variable(1), u8_to_u16(response.data()));

  // Get trace var ID for var 2 (un-associated)
  std::array<uint8_t, 2> get_var2_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::GetVarId), 2};
  processed = false;
  Context get_var2_context = {.request = get_var2_command.data(),
                              .request_length = std::size(get_var2_command),
                              .response = response.data(),
                              .max_response_length = kResponseSize,
                              .response_length = 0,
                              .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&get_var2_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_var2_context.response_length, 2);

  EXPECT_EQ(Variable::InvalidID, u8_to_u16(response.data()));

  // Get trace var ID for var Trace::MaxVars (out of bounds)
  std::array<uint8_t, 2> get_var_max_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::GetVarId),
      static_cast<uint8_t>(Trace::MaxVars)};
  processed = false;
  Context get_var_max_context = {.request = get_var_max_command.data(),
                                 .request_length = std::size(get_var_max_command),
                                 .response = response.data(),
                                 .max_response_length = kResponseSize,
                                 .response_length = 0,
                                 .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&get_var_max_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_var_max_context.response_length, 2);
  EXPECT_EQ(Variable::InvalidID, u8_to_u16(response.data()));

  // Set trace period
  std::array<uint8_t, 5> set_period_command = {
      static_cast<uint8_t>(TraceHandler::Subcommand::SetPeriod)};
  u32_to_u8(2, &set_period_command[1]);
  processed = false;
  Context set_period_context = {.request = set_period_command.data(),
                                .request_length = std::size(set_period_command),
                                .response = response.data(),
                                .max_response_length = kResponseSize,
                                .response_length = 0,
                                .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&set_period_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(set_period_context.response_length, 0);

  EXPECT_EQ(trace.period(), u8_to_u32(&set_period_command[1]));

  // Get trace period
  std::array get_period_command = {static_cast<uint8_t>(TraceHandler::Subcommand::GetPeriod)};
  processed = false;
  Context get_period_context = {.request = get_period_command.data(),
                                .request_length = std::size(get_period_command),
                                .response = response.data(),
                                .max_response_length = kResponseSize,
                                .response_length = 0,
                                .processed = &processed};
  EXPECT_EQ(ErrorCode::None, trace_handler.Process(&get_period_context));
  EXPECT_TRUE(processed);
  EXPECT_EQ(get_period_context.response_length, 4);

  EXPECT_EQ(trace.period(), u8_to_u32(get_period_context.response));
}

TEST(TraceHandler, Errors) {
  // define some debug variables
  Debug::Variable::UInt32 var_x("x", Debug::Variable::Access::ReadOnly, 0, "unit");
  Debug::Variable::UInt32 var_y("y", Debug::Variable::Access::ReadOnly, 0, "unit");

  // define trace and trace handler
  Trace trace;
  TraceHandler trace_handler = TraceHandler(&trace);

  trace.set_traced_variable(1, var_x.id());
  trace.set_traced_variable(3, var_y.id());
  trace.start();

  std::vector<std::tuple<std::vector<uint8_t>, ErrorCode>> requests = {
      {std::vector<uint8_t>{}, ErrorCode::MissingData},  // Missing subcommand
      {{9}, ErrorCode::InvalidData},                     // Invalid subcommand
      {{static_cast<uint8_t>(TraceHandler::Subcommand::Download)}, ErrorCode::NoMemory},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::SetVarId), 1, 1}, ErrorCode::MissingData},
      //      {{static_cast<uint8_t>(TraceHandler::Subcommand::SetVarId), Trace::MaxVars, 1, 0},
      //       ErrorCode::InvalidData}, // no longer the expected behavior?
      {{static_cast<uint8_t>(TraceHandler::Subcommand::GetVarId)}, ErrorCode::MissingData},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::GetVarId), 1}, ErrorCode::NoMemory},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::SetPeriod), 1, 1, 1},
       ErrorCode::MissingData},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::GetPeriod)}, ErrorCode::NoMemory},
      {{static_cast<uint8_t>(TraceHandler::Subcommand::CountSamples)}, ErrorCode::NoMemory},
  };
  std::array<uint8_t, kResponseSize> response;
  bool processed{false};
  for (size_t i = 0; i < requests.size(); ++i) {
    auto &[request, error] = requests[i];
    Context context = {.request = request.data(),
                       .request_length = static_cast<uint32_t>(request.size()),
                       .response = response.data(),
                       .max_response_length = 1,  // To provoke the No Memory error once all other
                                                  // checks have passed
                       .response_length = 0,
                       .processed = &processed};
    EXPECT_EQ(error, trace_handler.Process(&context)) << " failed on test #" << i;
    EXPECT_FALSE(processed);
    EXPECT_EQ(context.response_length, 0);
  }
}

}  // namespace Debug::Command
