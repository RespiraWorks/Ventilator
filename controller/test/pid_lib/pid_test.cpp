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
inline constexpr float OUTPUT_TOLERANCE = 0.0f;

// PID min/max output
inline constexpr const float MAX_OUTPUT = 1.0f;
inline constexpr const float MIN_OUTPUT = 0.0f;

//// Maximum task jitter (assume 5 ms for now, will need adjuting)
// inline constexpr Duration max_task_jitter = milliseconds(5);
//
// inline constexpr Time base = millisSinceStartup(10000);
inline constexpr Duration sample_period = milliseconds(100);
// Time ticks(int num_ticks) { return base + num_ticks * sample_period; }

#define EXPECT_OUTPUT(expected, actual)                                        \
  EXPECT_NEAR(expected, actual, OUTPUT_TOLERANCE)

TEST(PidTest, Proportional) {
  const float Kp = 0.9f;
  const float setpoint = 25;
  const float input = setpoint - 10;

  PID pid;
  pid.SetKP(Kp);
  pid.SetSamplePeriod(sample_period);

  // Ki and Kd = 0 therefore output = Kp*error, nothing more
  EXPECT_EQ(pid.Compute(input, setpoint), (setpoint - input) * Kp);
}

TEST(PidTest, IntegralBasic) {
  const float Ki = 1.75f;
  const float setpoint = 25;
  const float input = setpoint - 10;

  PID pid;
  pid.SetKI(Ki);
  pid.SetSamplePeriod(sample_period);

  // on first call, integral = error*sample time, output = Ki*integral
  EXPECT_EQ(pid.Compute(input, setpoint),
            (setpoint - input) * sample_period.seconds() * Ki);

  // on second call, integral = error*2*sample time, output = Ki*integral
  EXPECT_OUTPUT(pid.Compute(input, setpoint),
                (setpoint - input) * 2 * sample_period.seconds() * Ki);
}

TEST(PidTest, IntegralSaturationMax) {
  const float Ki = 0.175f;
  const float setpoint = 25;
  const float input = setpoint - 10;

  PID pid;
  pid.SetKI(Ki);
  pid.SetSamplePeriod(sample_period);
  pid.SetLimits(MIN_OUTPUT, MAX_OUTPUT);

  float output = 0;
  for (int i = 0; i < 1000; ++i) {
    output = pid.Compute(input, setpoint);
    if (output >= MAX_OUTPUT)
      break;
  }
  EXPECT_EQ(output, MAX_OUTPUT);

  for (int i = 0; i < 10; i++) {
    pid.Compute(input, setpoint);
  }

  // make setpoint lower than input to de-saturate output
  const float low_setpoint = input - 10;
  EXPECT_LT(pid.Compute(input, low_setpoint), MAX_OUTPUT);
}

TEST(PidTest, IntegralSaturationMin) {
  const float Ki = 0.175f;
  const float setpoint = 25;
  const float input = setpoint + 10;

  // Create PID and run once
  PID pid;
  pid.SetKI(Ki);
  pid.SetSamplePeriod(sample_period);
  pid.SetLimits(MIN_OUTPUT, MAX_OUTPUT);

  float output = 0;
  for (int i = 0; i < 1000; ++i) {
    output = pid.Compute(input, setpoint);
    if (output <= MIN_OUTPUT)
      break;
  }
  EXPECT_EQ(output, MIN_OUTPUT);

  // run a few times to make sure actual integral is lower
  for (int i = 0; i < 10; i++) {
    pid.Compute(input, setpoint);
  }

  // make setpoint bigger than input again to de-saturate output
  float new_setpoint = input + 10;

  EXPECT_GT(pid.Compute(input, new_setpoint), MIN_OUTPUT);
}

// In the following tests we transition from (input1, setpoint1) to (input2,
// setpoint2). Derivative on measure and on error should handle this transition
// differently.
TEST(PidTest, Derivative) {
  const float Kd = 1.5f;
  const float setpoint1 = 25;
  const float input1 = setpoint1 - 10;
  const float setpoint2 = 17;
  const float input2 = setpoint2 - 13;

  PID pid;
  pid.SetKD(Kd);
  pid.SetSamplePeriod(sample_period);

  float e1 = setpoint1 - input1;
  EXPECT_EQ(pid.Compute(input1, setpoint1), e1 * Kd / sample_period.seconds());

  float e2 = setpoint2 - input2;
  EXPECT_EQ(pid.Compute(input2, setpoint2),
            (e2 - e1) * Kd / sample_period.seconds());
}
