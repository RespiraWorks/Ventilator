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
  Debug::Variable::FnVar var_x(
      Debug::Variable::Type::UInt32, "x", Debug::Variable::Access::ReadOnly,
      "units", [&] { return i; }, [&](uint32_t value) { (void)value; }, "");
  Debug::Variable::FnVar var_y(
      Debug::Variable::Type::UInt32, "y", Debug::Variable::Access::ReadOnly,
      "units", [&] { return 10 * i; }, [&](uint32_t value) { (void)value; },
      "");

  Trace trace;
  trace.SetPeriod(3);  // Trace every 3 cycles.
  trace.Start();       // Enable tracing
  trace.SetTracedVarId<1>(var_x.GetId());
  trace.SetTracedVarId<3>(var_y.GetId());

  EXPECT_EQ(2, trace.GetNumActiveVars());
  EXPECT_EQ(-1, trace.GetTracedVarId<0>());
  EXPECT_EQ(var_x.GetId(), trace.GetTracedVarId<1>());
  EXPECT_EQ(-1, trace.GetTracedVarId<2>());
  EXPECT_EQ(var_y.GetId(), trace.GetTracedVarId<3>());

  int expected_num_samples = 0;
  for (; i < 9; ++i) {
    trace.MaybeSample();  // Should sample at i = 0, 3, 6
    if (i % 3 == 0) ++expected_num_samples;
    EXPECT_EQ(expected_num_samples, trace.GetNumSamples());
  }

  std::array<uint32_t, 4> record = {0};
  size_t count;

  // Extract a few records
  for (int j = 0; j < 9; j += 3, --expected_num_samples) {
    EXPECT_EQ(expected_num_samples, trace.GetNumSamples());
    EXPECT_TRUE(trace.GetNextTraceRecord(&record, &count));
    EXPECT_EQ(2, count);
    EXPECT_THAT(record, testing::ElementsAre(j, 10 * j, 0, 0));
  }

  // Simulate the high-priority thread running a few times while the trace is
  // being collected.
  for (; i < 15; ++i) {
    trace.MaybeSample();  // Should sample at i = 9, 12
    if (i % 3 == 0) ++expected_num_samples;
    EXPECT_EQ(expected_num_samples, trace.GetNumSamples());
  }

  // Extract the remaining records
  for (int j = 9; j < 15; j += 3, --expected_num_samples) {
    EXPECT_EQ(expected_num_samples, trace.GetNumSamples());
    EXPECT_TRUE(trace.GetNextTraceRecord(&record, &count));
    EXPECT_EQ(2, count);
    EXPECT_THAT(record, testing::ElementsAre(j, 10 * j, 0, 0));
  }

  EXPECT_EQ(0, trace.GetNumSamples());
  EXPECT_FALSE(trace.GetNextTraceRecord(&record, &count));
}

TEST(Trace, TracesEveryCycleByDefault) {
  Trace trace;
  uint32_t x = 42;
  Debug::Variable::Primitive32 var_x("x", Debug::Variable::Access::ReadOnly, &x,
                                     "units");
  trace.SetTracedVarId<0>(var_x.GetId());
  trace.Start();
  // Do not set period

  trace.MaybeSample();
  trace.MaybeSample();
  trace.MaybeSample();
  EXPECT_EQ(3, trace.GetNumSamples());
}

TEST(Trace, BufferFull) {
  uint32_t x = 42;
  Debug::Variable::Primitive32 var_x("x", Debug::Variable::Access::ReadOnly, &x,
                                     "units");
  Trace trace;
  trace.SetTracedVarId<0>(var_x.GetId());
  trace.Start();

  // Buffer capacity from trace.h header file. Update if necessary.
  int expected_capacity = 0x4000;
  for (int i = 0; i < expected_capacity; ++i) {
    EXPECT_TRUE(trace.GetStatus());
    trace.MaybeSample();
    EXPECT_EQ(i + 1, trace.GetNumSamples());
  }

  trace.MaybeSample();
  EXPECT_FALSE(trace.GetStatus());
  EXPECT_EQ(expected_capacity, trace.GetNumSamples());

  // Flags should be 0 by now, so this has no effect.
  trace.MaybeSample();
  trace.MaybeSample();
  trace.MaybeSample();
  EXPECT_FALSE(trace.GetStatus());
  EXPECT_EQ(expected_capacity, trace.GetNumSamples());

  // Re-enable tracing: that should flush the trace.
  trace.Start();
  trace.MaybeSample();
  trace.MaybeSample();
  trace.MaybeSample();
  EXPECT_EQ(3, trace.GetNumSamples());
}

TEST(Trace, FlushOnSetVar) {
  uint32_t x = 42, y = 37;
  Debug::Variable::Primitive32 var_x("x", Debug::Variable::Access::ReadOnly, &x,
                                     "units");
  Debug::Variable::Primitive32 var_y("y", Debug::Variable::Access::ReadOnly, &y,
                                     "units");
  Trace trace;
  trace.SetTracedVarId<0>(var_x.GetId());
  trace.Start();

  trace.MaybeSample();
  trace.MaybeSample();
  trace.MaybeSample();
  EXPECT_EQ(3, trace.GetNumSamples());

  // Adding a new variable should reset the trace because otherwise
  // the interpretation of the circular buffer becomes ambiguous.
  trace.SetTracedVarId<2>(var_y.GetId());
  EXPECT_EQ(0, trace.GetNumSamples());

  trace.MaybeSample();
  trace.MaybeSample();
  EXPECT_EQ(2, trace.GetNumSamples());
}
