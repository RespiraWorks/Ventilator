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

/*
 * module contributors: asmodai27
 *
 * The purpose of this module is to provide unit testing for the pid
 * controller module.  Module is to be run on an x86 host and is not to be run
 * on an Arduino platform.
 */

#include "pid.h"
#include "types.h"
#include "gtest/gtest.h"

// The PWM is a 0-255 integer, which means we can accept error of 1 in output
inline constexpr float OUTPUT_TOLERANCE = 1;

// PID min/max output
inline constexpr const float MAX_OUTPUT = 255;
inline constexpr const float MIN_OUTPUT = 0;

// Maximum task jitter (assume 5 ms for now, will need adjuting)
inline constexpr Duration max_task_jitter = milliseconds(5);

inline constexpr Time base = microsSinceStartup(10'000'000);
inline constexpr Duration sample_period = milliseconds(100);
Time ticks(int num_ticks) { return base + num_ticks * sample_period; }

#define EXPECT_OUTPUT(expected, actual)                                        \
  EXPECT_NEAR(expected, actual, OUTPUT_TOLERANCE)

TEST(PidTest, Proportional) {
  const float Kp = 0.9f;
  const float setpoint = 25;
  const float input = setpoint - 10;

  // Create PID and run once
  PID pid(Kp, /*ki=*/0, /*kd=*/0, ProportionalTerm::ON_ERROR,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  int t = 0;

  // Ki and Kd = 0 therefore output = Kp*error, nothing more
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, setpoint),
                (setpoint - input) * Kp);
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, setpoint),
                (setpoint - input) * Kp);
}

TEST(PidTest, IntegralBasic) {
  const float Ki = 1.75f;
  const float setpoint = 25;
  const float input = setpoint - 10;

  PID pid(/*kp=*/0, Ki, /*kd=*/0, ProportionalTerm::ON_ERROR,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  int t = 0;

  // on first call, integral = error * 0, output = 0.
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, setpoint), 0);

  // on second call, integral = error * sample_time, output = Ki*integral
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, setpoint),
                (setpoint - input) * sample_period.seconds() * Ki);

  // on third call, integral = error * 2 * sample_time, output = Ki*integral
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, setpoint),
                (setpoint - input) * 2 * sample_period.seconds() * Ki);
}

TEST(PidTest, IntegralSaturationMax) {
  const float Ki = 1.75f;
  const float setpoint = 25;
  const float input = setpoint - 10;

  PID pid(/*kp=*/0, Ki, /*kd=*/0, ProportionalTerm::ON_ERROR,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  int t = 0;

  float output = 0;
  for (int i = 0; i < 1000; ++i) {
    output = pid.Compute(ticks(t++), input, setpoint);
    if (output >= MAX_OUTPUT)
      break;
  }
  EXPECT_EQ(output, MAX_OUTPUT);

  for (int i = 0; i < 10; i++) {
    pid.Compute(ticks(t++), input, setpoint);
  }

  // make setpoint lower than input to de-saturate output
  const float low_setpoint = input - 10;
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, low_setpoint),
                MAX_OUTPUT +
                    (low_setpoint - input) * sample_period.seconds() * Ki);
}

TEST(PidTest, IntegralSaturationMin) {
  const float Ki = 1.75f;
  const float setpoint = 25;
  const float input = setpoint + 10;

  // Create PID and run once
  PID pid(/*kp=*/0, Ki, /*kd=*/0, ProportionalTerm::ON_ERROR,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  int t = 0;

  float output = 0;
  for (int i = 0; i < 1000; ++i) {
    output = pid.Compute(ticks(t++), input, setpoint);
    if (output <= MIN_OUTPUT)
      break;
  }
  EXPECT_EQ(output, MIN_OUTPUT);

  // run a few times to make sure actual integral is lower
  for (int i = 0; i < 10; i++) {
    pid.Compute(ticks(t++), input, setpoint);
  }

  // make setpoint bigger than input again to de-saturate output
  float new_setpoint = input + 10;

  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, new_setpoint),
                MIN_OUTPUT +
                    (new_setpoint - input) * sample_period.seconds() * Ki);
}

// In the following tests we transition from (input1, setpoint1) to (input2,
// setpoint2). Derivative on measure and on error should handle this transition
// differently.
TEST(PidTest, DerivativeOnMeasure) {
  const float Kd = 1.5f;
  const float setpoint1 = 25;
  const float input1 = setpoint1 - 10;
  const float setpoint2 = 17;
  const float input2 = setpoint2 - 13;
  PID pid(/*kp=*/0, /*ki=*/0, Kd, ProportionalTerm::ON_MEASUREMENT,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  int t = 0;

  // Expect no derivative on first call
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input1, setpoint1), 0);

  // Note that DifferentialTerm::ON_MEASUREMENT actually works with
  // -1*Kd to allow using the same parameters as DifferentialTerm::ON_ERROR
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input2, setpoint2),
                -1 * Kd * (input2 - input1) / sample_period.seconds());
}

TEST(PidTest, DerivativeOnError) {
  const float Kd = 1.5f;
  const float setpoint1 = 25;
  const float input1 = setpoint1 - 10;
  const float setpoint2 = 17;
  const float input2 = setpoint2 - 13;
  PID pid(/*kp=*/0, /*ki=*/0, Kd, ProportionalTerm::ON_MEASUREMENT,
          DifferentialTerm::ON_ERROR, MIN_OUTPUT, MAX_OUTPUT);
  int t = 0;

  // Expect no derivative on first call
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input1, setpoint1), 0);
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input2, setpoint2),
                Kd * ((setpoint2 - input2) - (setpoint1 - input1)) /
                    sample_period.seconds());
}

TEST(PidTest, TaskJitter) {
  // This test uses integral to check the effect of time between calls on the
  // PID output. Introducing jitter in call frequency and checking that the
  // integral takes this jitter into account.
  const float Ki = 0.5f;
  const float setpoint = 25;
  const float input = setpoint - 10;
  PID pid(/*kp=*/0, Ki, /*kd=*/0, ProportionalTerm::ON_ERROR,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  Time now = base;

  float integral = (setpoint - input) * sample_period.seconds();
  EXPECT_OUTPUT(pid.Compute(now, input, setpoint), integral * Ki);

  // Advance time with jitter
  Duration dt_high = sample_period + max_task_jitter;
  now += dt_high;
  // Expect output to take jitter into account in integral
  integral += (setpoint - input) * dt_high.seconds();
  EXPECT_OUTPUT(pid.Compute(now, input, setpoint), integral * Ki);

  // Advance time and compensate previous jitter to have total time
  // elapsed = 2*sample time
  Duration dt_low = sample_period - max_task_jitter;
  integral += (setpoint - input) * dt_low.seconds();
  now += dt_low;
  EXPECT_OUTPUT(pid.Compute(now, input, setpoint), integral * Ki);
}

TEST(PidTest, MissedSample) {
  // This test uses integral to check the effect of missing a sample in the
  // execution of PID
  const float Ki = 0.2f;
  const float setpoint = 25;
  const float input = setpoint - 10;
  PID pid(/*kp=*/0, Ki, /*kd=*/0, ProportionalTerm::ON_ERROR,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  Time now = base;

  float integral = (setpoint - input) * (sample_period.seconds());
  EXPECT_OUTPUT(pid.Compute(now, input, setpoint), integral * Ki);

  // Advance time
  now += 2 * sample_period;
  float output = pid.Compute(now, input, setpoint);
  // Expect output to integrate this
  integral += (setpoint - input) * 2 * sample_period.seconds();
  EXPECT_OUTPUT(output, integral * Ki);

  // check that a new call without change in time has no effect, even if we
  // missed a sample
  EXPECT_EQ(pid.Compute(now, input, setpoint), output);

  // Advance time a small amount to allow PID to catch up to missed sample
  Duration dt = max_task_jitter;
  now += dt;
  integral += (setpoint - input) * dt.seconds();
  // Expect output to have a new update
  EXPECT_OUTPUT(pid.Compute(now, input, setpoint), integral * Ki);
}

TEST(PidTest, Observe) {
  float Ki = 1;
  float setpoint = 25;
  float input = setpoint - 10;
  PID pid(/*kp=*/0, Ki, /*kd=*/0, ProportionalTerm::ON_ERROR,
          DifferentialTerm::ON_MEASUREMENT, MIN_OUTPUT, MAX_OUTPUT);
  int t = 0;

  // Run PID a few times
  float output;
  for (int i = 0; i < 10; i++) {
    output = pid.Compute(ticks(t++), input, setpoint);
  }
  // Make sure we're not saturated in either direction so the following tests
  // are not vacuous.
  EXPECT_GT(output, MIN_OUTPUT);
  EXPECT_LT(output, MAX_OUTPUT);

  // Do a few cycles where the PID is not actually in control
  float last_output;
  for (int i = 0; i < 10; i++) {
    last_output = static_cast<float>(128 + i);
    pid.Observe(ticks(t++), input, setpoint, /*actual_output=*/last_output);
  }

  float integral = (setpoint - input) * sample_period.seconds();
  EXPECT_OUTPUT(pid.Compute(ticks(t++), input, setpoint),
                128 + 10 + integral * Ki);
}

TEST(PidTest, Reset) {

  PID pid(0, 0, 0, ProportionalTerm::ON_ERROR, DifferentialTerm::ON_ERROR,
          MIN_OUTPUT, MAX_OUTPUT);

  // Run one cycle to set last error
  int t = 0;
  float kp = 1;
  float setpoint = 25;
  float error = 10;
  pid.SetKP(kp);
  float output = pid.Compute(ticks(t++), setpoint - error, setpoint);
  EXPECT_EQ(output, kp * error);

  // Normally the kd term would act on the change in error.
  // If I reset the PID then the change in error will be zero, so
  // if the reset works the output will be zero
  pid.SetKP(0);
  pid.SetKD(1);
  error = 20;
  pid.Reset();
  output = pid.Compute(ticks(t++), setpoint - error, setpoint);
  EXPECT_EQ(output, 0.0f);

  // Build up an integral sum
  pid.SetKD(0);
  pid.SetKI(1);
  for (int i = 0; i < 10; i++)
    output = pid.Compute(ticks(t++), setpoint - error, setpoint);

  // With all gains set to zero, running again will not change the output
  // it will still be the output_sum_ value
  pid.SetKI(0);
  EXPECT_EQ(output, pid.Compute(ticks(t++), setpoint - error, setpoint));

  // Resetting the PID will reset output_sum to zero
  pid.Reset();
  EXPECT_EQ(0.0f, pid.Compute(ticks(t++), setpoint - error, setpoint));
}
