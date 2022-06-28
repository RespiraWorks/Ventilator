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

#include "trace.h"

#include <stdint.h>

#include <iostream>
#include <limits>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

using namespace Debug;

TEST(Trace, MaybeSampleTwoVars) {
  uint32_t i = 0;
  Variable::FnVar32 var_x(
      Variable::Type::UInt32, "x", Variable::Access::ReadOnly, "units",
      [&](void* write_buff) { std::memcpy(write_buff, &i, 4); },
      [&](const void* read_buf) { (void)read_buf; }, "");
  Variable::FnVar32 var_y(
      Variable::Type::UInt32, "y", Variable::Access::ReadOnly, "units",
      [&](void* write_buff) {
        auto ii = 10 * i;
        std::memcpy(write_buff, &ii, 4);
      },
      [&](const void* read_buf) { (void)read_buf; }, "");

  Trace trace;
  trace.set_period(3);  // Trace every 3 cycles.
  trace.start();        // Enable tracing
  trace.set_traced_variable(1, var_x.id());
  trace.set_traced_variable(3, var_y.id());

  EXPECT_EQ(2, trace.active_variable_count());
  EXPECT_EQ(Variable::InvalidID, trace.traced_variable(0));
  EXPECT_EQ(var_x.id(), trace.traced_variable(1));
  EXPECT_EQ(Variable::InvalidID, trace.traced_variable(2));
  EXPECT_EQ(var_y.id(), trace.traced_variable(3));

  int expected_num_samples = 0;
  for (; i < 9; ++i) {
    trace.maybe_sample();  // Should sample at i = 0, 3, 6
    if (i % 3 == 0) {
      ++expected_num_samples;
    }
    EXPECT_EQ(expected_num_samples, trace.sample_count());
  }

  std::array<uint32_t, 4> record = {0};
  size_t count;

  // Extract a few records
  for (int j = 0; j < 9; j += 3, --expected_num_samples) {
    EXPECT_EQ(expected_num_samples, trace.sample_count());
    EXPECT_TRUE(trace.get_next_record(&record, &count));
    EXPECT_EQ(2, count);
    EXPECT_THAT(record, testing::ElementsAre(j, 10 * j, 0, 0));
  }

  // Simulate the high-priority thread running a few times while the trace is
  // being collected.
  for (; i < 15; ++i) {
    trace.maybe_sample();  // Should sample at i = 9, 12
    if (i % 3 == 0) {
      ++expected_num_samples;
    }
    EXPECT_EQ(expected_num_samples, trace.sample_count());
  }

  // Extract the remaining records
  for (int j = 9; j < 15; j += 3, --expected_num_samples) {
    EXPECT_EQ(expected_num_samples, trace.sample_count());
    EXPECT_TRUE(trace.get_next_record(&record, &count));
    EXPECT_EQ(2, count);
    EXPECT_THAT(record, testing::ElementsAre(j, 10 * j, 0, 0));
  }

  EXPECT_EQ(0, trace.sample_count());
  EXPECT_FALSE(trace.get_next_record(&record, &count));
}

TEST(Trace, TracesEveryCycleByDefault) {
  Trace trace;
  uint32_t x = 42;
  Variable::Primitive32 var_x("x", Variable::Access::ReadOnly, &x, "units");
  trace.set_traced_variable(0, var_x.id());
  trace.start();
  // Do not set period

  trace.maybe_sample();
  trace.maybe_sample();
  trace.maybe_sample();
  EXPECT_EQ(3, trace.sample_count());
}

TEST(Trace, BufferFull) {
  uint32_t x = 42;
  Variable::Primitive32 var_x("x", Variable::Access::ReadOnly, &x, "units");
  Trace trace;
  trace.set_traced_variable(0, var_x.id());
  trace.start();

  // Buffer capacity from trace.h header file. Update if necessary.
  int expected_capacity = 0x4000;
  for (int i = 0; i < expected_capacity; ++i) {
    EXPECT_TRUE(trace.running());
    trace.maybe_sample();
    EXPECT_EQ(i + 1, trace.sample_count());
  }

  trace.maybe_sample();
  EXPECT_FALSE(trace.running());
  EXPECT_EQ(expected_capacity, trace.sample_count());

  // Flags should be 0 by now, so this has no effect.
  trace.maybe_sample();
  trace.maybe_sample();
  trace.maybe_sample();
  EXPECT_FALSE(trace.running());
  EXPECT_EQ(expected_capacity, trace.sample_count());

  // Re-enable tracing: that should flush the trace.
  trace.start();
  trace.maybe_sample();
  trace.maybe_sample();
  trace.maybe_sample();
  EXPECT_EQ(3, trace.sample_count());
}

TEST(Trace, FlushOnSetVar) {
  uint32_t x = 42, y = 37;
  Variable::Primitive32 var_x("x", Variable::Access::ReadOnly, &x, "units");
  Variable::Primitive32 var_y("y", Variable::Access::ReadOnly, &y, "units");
  Trace trace;
  trace.set_traced_variable(0, var_x.id());
  trace.start();

  trace.maybe_sample();
  trace.maybe_sample();
  trace.maybe_sample();
  EXPECT_EQ(3, trace.sample_count());

  // Adding a new variable should reset the trace because otherwise
  // the interpretation of the circular buffer becomes ambiguous.
  trace.set_traced_variable(2, var_y.id());
  EXPECT_EQ(0, trace.sample_count());

  trace.maybe_sample();
  trace.maybe_sample();
  EXPECT_EQ(2, trace.sample_count());
}

TEST(Trace, SetVarAssumptions) {
  Trace trace;
  uint32_t x = 42;
  Variable::Primitive32 var_x("x", Variable::Access::ReadOnly, &x, "units");

  EXPECT_TRUE(trace.set_traced_variable(0, Variable::InvalidID));
  EXPECT_EQ(trace.traced_variable(0), Variable::InvalidID);

  EXPECT_TRUE(trace.set_traced_variable(0, var_x.id()));
  EXPECT_EQ(trace.traced_variable(0), var_x.id());

  EXPECT_FALSE(trace.set_traced_variable(0, 666));
  EXPECT_NE(trace.traced_variable(0), 666);

  Variable::FloatArray<3> fa3("fa3", Variable::Access::ReadWrite, "units");
  EXPECT_FALSE(trace.set_traced_variable(0, fa3.id()));
  EXPECT_NE(trace.traced_variable(0), fa3.id());
}
