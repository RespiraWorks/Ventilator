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

#include "blower_fsm.h"

#include "blower_fsm_test_data.h"
#include "controller.h"
#include "hal.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include <optional>
#include <string>

namespace {

constexpr BlowerFsmInputs inputs_zero = {
    .patient_volume = ml(0),
    .net_flow = ml_per_sec(0),
};

constexpr int64_t rise_time_us = RISE_TIME.microseconds();
static_assert(rise_time_us % 1000 == 0,
              "blower fsm tests assume rise time is a whole number of ms.");
static_assert(rise_time_us % 5 == 0,
              "blower fsm tests assume we can divide rise time ms by 5.");
static_assert(rise_time_us % 2 == 0,
              "blower fsm tests assume we can divide rise time ms by 2.");

TEST(BlowerFsmTest, InitiallyOff) {
  BlowerFsm fsm;
  VentParams p = VentParams_init_zero;
  BlowerSystemState s = fsm.DesiredState(Hal.now(), p, inputs_zero);
  EXPECT_TRUE(s.pressure_setpoint == std::nullopt);
  EXPECT_EQ(s.flow_direction, FlowDirection::EXPIRATORY);
}

TEST(BlowerFsmTest, StaysOff) {
  BlowerFsm fsm;
  VentParams p = VentParams_init_zero;
  Hal.delay(milliseconds(1000));
  BlowerSystemState s = fsm.DesiredState(Hal.now(), p, inputs_zero);
  EXPECT_TRUE(s.pressure_setpoint == std::nullopt);
  EXPECT_EQ(s.flow_direction, FlowDirection::EXPIRATORY);
}

TEST(BlowerFsmTest, OffFsmDesiredPipPeep) {
  BlowerFsm fsm;
  VentParams p = VentParams_init_zero;
  p.mode = VentMode_OFF;
  p.breaths_per_min = 20;
  p.inspiratory_expiratory_ratio = 2;
  p.peep_cm_h2o = 10;
  p.pip_cm_h2o = 20;

  BlowerSystemState s = fsm.DesiredState(Hal.now(), p, inputs_zero);
  EXPECT_EQ(s.pip.cmH2O(), 0.f);
  EXPECT_EQ(s.peep.cmH2O(), 0.f);
}

TEST(BlowerFsmTest, DesiredPipPeep) {
  for (VentMode mode : {VentMode_PRESSURE_CONTROL, VentMode_PRESSURE_ASSIST}) {
    BlowerFsm fsm;
    VentParams p = VentParams_init_zero;
    p.mode = mode;
    p.breaths_per_min = 20; // 3s/breath
    p.inspiratory_expiratory_ratio = 2;
    p.pip_cm_h2o = 20;
    p.peep_cm_h2o = 10;

    BlowerSystemState s = fsm.DesiredState(Hal.now(), p, inputs_zero);
    EXPECT_EQ(s.pip.cmH2O(), 20.f);
    EXPECT_EQ(s.peep.cmH2O(), 10.f);

    Hal.delay(seconds(1));
    p.pip_cm_h2o = 25;
    p.peep_cm_h2o = 15;

    // pip/peep unchanged, because we haven't hit a breath boundary yet.
    s = fsm.DesiredState(Hal.now(), p, inputs_zero);
    EXPECT_EQ(s.pip.cmH2O(), 20.f);
    EXPECT_EQ(s.peep.cmH2O(), 10.f);

    // We are at the breath boundary; the FSM will return the last desired state
    // for the just-completed breath; is_end_of_breath flag should be true
    Hal.delay(seconds(2));
    s = fsm.DesiredState(Hal.now(), p, inputs_zero);
    EXPECT_EQ(s.pip.cmH2O(), 20.f);
    EXPECT_EQ(s.peep.cmH2O(), 10.f);
    EXPECT_EQ(s.is_end_of_breath, true);

    // the next desired state should contain updated values
    s = fsm.DesiredState(Hal.now(), p, inputs_zero);
    EXPECT_EQ(s.pip.cmH2O(), 25.f);
    EXPECT_EQ(s.peep.cmH2O(), 15.f);
    EXPECT_EQ(s.is_end_of_breath, false);
  }
}

struct BlowerFsmTest {
  Time time{microsSinceStartup(0)};
  VentParams params;
  BlowerFsmInputs inputs;
  BlowerSystemState expected_state;
};

// Checks that a sequence of calls to blower_fsm_desired_state() yield the
// expected results.
void testSequence(const std::vector<BlowerFsmTest> &seq) {
  BlowerFsm fsm;
  if (seq.empty()) {
    return;
  }

  // Reset time to test's start time.
  // TODO: Add a Hal.test_ResetClockToZero() command?
  Hal.delay(seq.front().time - Hal.now());

  VentParams last_params;
  BlowerFsmInputs last_inputs;
  for (const auto &blower_fsm_test : seq) {
    SCOPED_TRACE("time = " + blower_fsm_test.time.microsSinceStartup() / 1000);
    // Move time forward to t in steps of Controller::GetLoopPeriod().
    while (Hal.now() < blower_fsm_test.time) {
      Hal.delay(Controller::GetLoopPeriod());
      (void)fsm.DesiredState(Hal.now(), last_params, last_inputs);
    }
    EXPECT_EQ(blower_fsm_test.time.microsSinceStartup(),
              Hal.now().microsSinceStartup());

    BlowerSystemState state = fsm.DesiredState(
        Hal.now(), blower_fsm_test.params, blower_fsm_test.inputs);
    EXPECT_EQ(state.pressure_setpoint.has_value(),
              blower_fsm_test.expected_state.pressure_setpoint.has_value());
    EXPECT_FLOAT_EQ(
        state.pressure_setpoint.value_or(cmH2O(0)).cmH2O(),
        blower_fsm_test.expected_state.pressure_setpoint.value_or(cmH2O(0))
            .cmH2O());
    EXPECT_EQ(state.flow_direction,
              blower_fsm_test.expected_state.flow_direction);

    last_params = blower_fsm_test.params;
    last_inputs = blower_fsm_test.inputs;
  }
}

TEST(BlowerFsmTest, PressureControl) {
  VentParams params = VentParams_init_zero;
  params.mode = VentMode_PRESSURE_CONTROL;
  // 20 breaths/min = 3s/breath.  I:E = 2 means 2s for inspire, 1s for expire.
  params.breaths_per_min = 20;
  params.inspiratory_expiratory_ratio = 2;
  params.peep_cm_h2o = 10;
  params.pip_cm_h2o = 20;

  testSequence({
      // Pressure starts out at PEEP and rises to PIP over period RISE_TIME.
      {.time = microsSinceStartup(0),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(10),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(1 * rise_time_us / 5),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(12),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(2 * rise_time_us / 5),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(14),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(3 * rise_time_us / 5),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(16),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(4 * rise_time_us / 5),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(18),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(5 * rise_time_us / 5),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(20),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      // Plateau at PIP
      {.time = microsSinceStartup(1E6),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(20),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(199E4),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(20),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      // end of inhale ==> back to PEEP
      {.time = microsSinceStartup(201E4),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(10),
                          .flow_direction = FlowDirection::EXPIRATORY}},
      {.time = microsSinceStartup(299E4),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(10),
                          .flow_direction = FlowDirection::EXPIRATORY}},
      // Start of next breath
      {.time = microsSinceStartup(300E4),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(10),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(300E4 + rise_time_us / 2),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(15),
                          .flow_direction = FlowDirection::INSPIRATORY}},
      {.time = microsSinceStartup(300E4 + rise_time_us),
       .params = params,
       .inputs = inputs_zero,
       .expected_state = {.pressure_setpoint = cmH2O(20),
                          .flow_direction = FlowDirection::INSPIRATORY}},
  });
}

struct FlowTraceResults {
  // Time relative to start of breath when flow direction switched from
  // INSPIRATORY to EXPIRATORY.
  std::optional<Duration> expire_start_time;

  // Time relative to start of breath when FSM indicated it was done.
  std::optional<Duration> finish_time;
};

// Runs a breath using a fresh FSM of type FsmTy, using the given array of
// flows (length n, time period between entries trace_interval).
//
// At every time named in setpoint_checks, check that the setpoint pressure is
// as specified.
template <typename FsmTy>
FlowTraceResults
RunFlowTrace(const VolumetricFlow *trace, size_t n, const VentParams &params,
             Duration trace_interval,
             std::vector<std::tuple</*time ms*/ uint64_t,
                                    /*setpoint pressure, cmH2O*/ float>>
                 setpoint_checks) {
  FsmTy fsm(Hal.now(), params);

  Time start = Hal.now();
  FlowTraceResults results;
  auto check_it = setpoint_checks.begin();

  for (size_t i = 0; i < n; i++) {
    auto ms = (Hal.now() - start).microseconds() / 1000;
    SCOPED_TRACE("time = " + std::to_string(ms));

    VolumetricFlow f = trace[i];

    // Our traces don't contain volume measurements, but this is OK for now.
    BlowerSystemState desired_state =
        fsm.DesiredState(Hal.now(), {.patient_volume = ml(0), .net_flow = f});
    FlowDirection dir = desired_state.flow_direction;
    if (dir == FlowDirection::EXPIRATORY && !results.expire_start_time) {
      results.expire_start_time = Hal.now() - start;
    }

    if (results.expire_start_time == std::nullopt) {
      EXPECT_EQ(FlowDirection::INSPIRATORY, dir);
    } else {
      EXPECT_EQ(FlowDirection::EXPIRATORY, dir);
    }

    if (check_it != setpoint_checks.end()) {
      const auto &[check_ms, check_cmh2o] = *check_it;
      if (check_ms == ms) {
        auto sp = desired_state.pressure_setpoint;
        EXPECT_TRUE(sp.has_value());
        if (sp.has_value()) {
          EXPECT_EQ(sp->cmH2O(), check_cmh2o);
        }
      }
      ++check_it;
    }

    if (desired_state.is_end_of_breath) {
      results.finish_time = Hal.now() - start;
      break;
    }

    Hal.delay(trace_interval);
  }

  EXPECT_TRUE(check_it == setpoint_checks.end())
      << "didn't see every expected pressure checkpoint";

  return results;
}

// Test a pressure-assist trace which has inspiratory effort.
TEST(BlowerFsmTest, PressureAssistFlowTrace1) {
  VentParams p = VentParams_init_zero;
  p.mode = VentMode_PRESSURE_ASSIST;
  p.breaths_per_min = 12;
  p.inspiratory_expiratory_ratio = 0.25;
  p.peep_cm_h2o = 10;
  p.pip_cm_h2o = 20;

  FlowTraceResults results = RunFlowTrace<PressureAssistFsm>(
      FLOW_TRACE_INSPIRATORY_EFFORT, std::size(FLOW_TRACE_INSPIRATORY_EFFORT),
      p, milliseconds(10),
      {
          {0, 10},    // start at PEEP
          {500, 20},  // rise to PIP
          {1000, 20}, // end inspire at PIP
          {1100, 10}, // fall to PEEP
          {1500, 10},
      });

  ASSERT_TRUE(results.expire_start_time.has_value());
  ASSERT_TRUE(results.finish_time.has_value());
  EXPECT_EQ(results.expire_start_time->milliseconds(), 1000.f);

  // Finished means either we detected inspiratory effort or we timed out
  // (started next breath).  In this trace, inspiratory effort begins at
  // around 1800ms and ends at around 2040ms.
  EXPECT_GE(results.finish_time->milliseconds(), 1800.f);
  EXPECT_LE(results.finish_time->milliseconds(), 2000.f);
}

// Test a pressure-assist trace which doesn't have inspiratory effort.
TEST(BlowerFsmTest, PressureAssistFlowTrace2) {
  VentParams p = VentParams_init_zero;
  p.mode = VentMode_PRESSURE_ASSIST;
  p.breaths_per_min = 12;
  p.inspiratory_expiratory_ratio = 0.25;
  p.peep_cm_h2o = 10;
  p.pip_cm_h2o = 20;

  FlowTraceResults results = RunFlowTrace<PressureAssistFsm>(
      FLOW_TRACE_NO_INSPIRATORY_EFFORT,
      std::size(FLOW_TRACE_NO_INSPIRATORY_EFFORT), p, milliseconds(10),
      {
          {0, 10},    // start at PEEP
          {500, 20},  // rise to PIP
          {1000, 20}, // end inspire at PIP
          {1100, 10}, // fall to PEEP
          {1500, 10},
          {5000, 10},
      });

  ASSERT_TRUE(results.expire_start_time.has_value());
  ASSERT_TRUE(results.finish_time.has_value());
  EXPECT_EQ(results.expire_start_time->milliseconds(), 1000.f);

  // No inspiratory effort, so this trace should end right at 5s, according to
  // the minimum respiratory rate in the VentParams.
  EXPECT_GE(results.finish_time->milliseconds(), 5000.f);
}

} // anonymous namespace
