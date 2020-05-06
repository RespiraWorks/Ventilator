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

#include "hal.h"
#include "pid.h"
#include "types.h"
#include "gtest/gtest.h"

// The PWM is a 0-255 integer, which means we can accept error of 1 in output
static const float OUTPUT_TOLERANCE = 1;

// PID min/max output
static const float MAX_OUTPUT = 255;
static const float MIN_OUTPUT = 0;

// Maximum task jitter (assume 5 ms for now, will need adjuting)
static const Duration max_task_jitter = milliseconds(5);

TEST(pidTest, Proportional) {
  float Kp = 0.9;
  float Ki = 0;
  float Kd = 0;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);

  // Create PID and run once
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();

  // Ki and Kd = 0 therefore output = Kp*error, nothing more
  EXPECT_NEAR(output, (setpoint - input) * Kp, OUTPUT_TOLERANCE);
  Hal.delay(sample_time);
  myPID.Compute();
  EXPECT_NEAR(output, (setpoint - input) * Kp, OUTPUT_TOLERANCE);
}

TEST(pidTest, Integral) {
  float Kp = 0;
  float Ki = 1.75;
  float Kd = 0;
  const uint32_t MAX_ITERATIONS = 1000;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);

  // Create PID and run once
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();

  // on first call, integral = error*sample time, output = Ki*integral
  EXPECT_NEAR(output, (setpoint - input) * sample_time.seconds() * Ki,
              OUTPUT_TOLERANCE);

  Hal.delay(sample_time);
  myPID.Compute();

  // on second call, integral = error*2*sample time, output = Ki*integral
  EXPECT_NEAR(output, (setpoint - input) * 2 * sample_time.seconds() * Ki,
              OUTPUT_TOLERANCE);

  uint32_t cycles = MAX_ITERATIONS;
  // test output and integral saturation to MAX
  while (output < MAX_OUTPUT && cycles > 0) {
    Hal.delay(sample_time);
    myPID.Compute();
    cycles--;
  }
  EXPECT_EQ(output, MAX_OUTPUT);
  // run a few times to make sure actual integral is bigger
  for (int i = 0; i < 10; i++) {
    Hal.delay(sample_time);
    myPID.Compute();
  }
  // make setpoint lower than input to de-saturate output
  setpoint = input - 10;

  Hal.delay(sample_time);
  myPID.Compute();

  EXPECT_NEAR(output,
              MAX_OUTPUT + (setpoint - input) * sample_time.seconds() * Ki,
              OUTPUT_TOLERANCE);

  // test output and integral saturation to MIN
  cycles = MAX_ITERATIONS;
  while (output > MIN_OUTPUT && cycles > 0) {
    Hal.delay(sample_time);
    myPID.Compute();
    cycles--;
  }
  EXPECT_EQ(output, MIN_OUTPUT);

  // run a few times to make sure actual integral is lower
  for (int i = 0; i < 10; i++) {
    Hal.delay(sample_time);
    myPID.Compute();
  }

  // make setpoint bigger than input again to de-saturate output
  setpoint = input + 10;

  Hal.delay(sample_time);
  myPID.Compute();

  EXPECT_NEAR(output,
              MIN_OUTPUT + (setpoint - input) * sample_time.seconds() * Ki,
              OUTPUT_TOLERANCE);
}

TEST(pidTest, derivativeOnMeasure) {
  float Kp = 0;
  float Ki = 0;
  float Kd = 1.5;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();
  // Expect no derivative on first call
  EXPECT_NEAR(output, 0, OUTPUT_TOLERANCE);

  // delay and update input to create non-zero derivative
  float previous_input = input;
  // output being in [0 255], create a negative input derivative to have a
  // positive output (DifferentialTerm::ON_MEASUREMENT actually works with
  // -1*Kd) to allow using the same parameters as DifferentialTerm::ON_ERROR)
  input = input - 5;
  float derivative = (input - previous_input) / sample_time.seconds();

  Hal.delay(sample_time);
  myPID.Compute();

  EXPECT_NEAR(output, -1 * derivative * Kd, OUTPUT_TOLERANCE);

  previous_input = input;
  // no change in input, but change setpoint, which should have no effect on
  // derivative (this mode uses derivative on measurement)
  setpoint = setpoint + 5;
  derivative = (input - previous_input) / sample_time.seconds();

  Hal.delay(sample_time);
  myPID.Compute();

  EXPECT_NEAR(output, -1 * derivative * Kd, OUTPUT_TOLERANCE);
}

TEST(pidTest, derivativeOnError) {
  float Kp = 0;
  float Ki = 0;
  float Kd = 2.5;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_ERROR, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();
  // Expect no derivative on first call
  EXPECT_NEAR(output, 0, OUTPUT_TOLERANCE);

  // update input to create non-zero derivative
  float previous_input = input;
  float previous_setpoint = setpoint;
  // output being in [0 255], create a negative input derivative in order to
  // have a positive error derivative, and therefore a positive output
  input = input - 5;
  float derivative =
      ((setpoint - input) - (previous_setpoint - previous_input)) /
      sample_time.seconds();

  Hal.delay(sample_time);
  myPID.Compute();

  EXPECT_NEAR(output, derivative * Kd, OUTPUT_TOLERANCE);

  previous_setpoint = setpoint;
  previous_input = input;
  // output being in [0 255], create a positive setpoint derivative in order to
  // have a positive error derivative, and therefore a positive output
  setpoint = setpoint + 5;
  derivative = ((setpoint - input) - (previous_setpoint - previous_input)) /
               sample_time.seconds();
  Hal.delay(sample_time);
  myPID.Compute();
  EXPECT_NEAR(output, derivative * Kd, OUTPUT_TOLERANCE);
}

TEST(pidTest, CallFasterThanSample) {
  // This test calls PID::Compute faster than sample time to check this has no
  // effect on output
  float Kp = 5.5;
  float Ki = 1.1;
  float Kd = 1.5;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();

  float last_output = output;
  // check that call with no time change has no effect
  myPID.Compute();
  EXPECT_EQ(output, last_output);

  Time last_time = Hal.now();

  // chose dt to not be a divisor of SampleTime, for better coverage
  Duration dt = milliseconds(6);
  // Run PID a few times with fast and check output doesn't change unless a
  // sample time has passed
  for (int i = 0; i < 100; i++) {
    Hal.delay(dt);
    myPID.Compute();
    if (Hal.now() >= last_time + sample_time) {
      last_output = output;
      last_time = last_time + sample_time;
    } else {
      EXPECT_EQ(output, last_output);
    }
  }
}

TEST(pidTest, TaskJitter) {
  // This test uses integral to check the effect of time between calls on the
  // PID output. Introducing jitter in call frequency and checking that the
  // integral takes this jitter into account.
  float Kp = 0;
  float Ki = 0.5;
  float Kd = 0;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();
  // First task, with only Ki set, output = error*sample_time
  float integral = (setpoint - input) * sample_time.seconds();
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
  // Advance time with jitter
  Duration dt = sample_time + max_task_jitter;
  Hal.delay(dt);
  myPID.Compute();
  // Expect output to take jitter into account in integral
  integral += (setpoint - input) * (dt.seconds());
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
  // Advance time and compensate previous jitter to have total time
  // elapsed = 2*sample time
  dt = sample_time - max_task_jitter;
  integral += (setpoint - input) * (dt.seconds());
  Hal.delay(dt);
  myPID.Compute();
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
}

TEST(pidTest, SampleTimeChange) {
  // This test uses integral to check the effect of changing sample time
  // during execution of the PID
  float Kp = 0;
  float Ki = 1.1;
  float Kd = 0;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();

  // First task, with only Ki set, output = error*sample_time
  float integral = (setpoint - input) * (sample_time.seconds());
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
  // Advance time
  Hal.delay(sample_time);
  myPID.Compute();
  // Expect output to integrate this
  integral += (setpoint - input) * (sample_time.seconds());
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
  // Change sample time and advance
  sample_time = milliseconds(50);
  myPID.SetSampleTime(sample_time);
  integral += (setpoint - input) * (sample_time.seconds());
  Hal.delay(sample_time);
  myPID.Compute();
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
}

TEST(pidTest, MissedSample) {
  // This test uses integral to check the effect of missing a sample in the
  // execution of PID
  float Kp = 0;
  float Ki = 0.2;
  float Kd = 0;
  float output = 0;
  float setpoint = 25;
  float input = setpoint - 10;
  Duration sample_time = milliseconds(100);
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, ProportionalTerm::ON_ERROR,
            DifferentialTerm::ON_MEASUREMENT, ControlDirection::DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.Compute();

  // First task, with only Ki set, output = error*sample_time
  float integral = (setpoint - input) * (sample_time.seconds());
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
  // Advance time
  Duration dt = sample_time + sample_time;
  Hal.delay(dt);
  myPID.Compute();
  // Expect output to integrate this
  integral += (setpoint - input) * (dt.seconds());
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
  // check that a new call without change in time has no effect, even if we
  // missed a sample
  float last_output = output;
  myPID.Compute();
  EXPECT_EQ(output, last_output);
  // Advance time a small amount to allow PID to catch up to missed sample
  dt = max_task_jitter;
  Hal.delay(dt);
  integral += (setpoint - input) * (dt.seconds());
  myPID.Compute();
  // Expect output to have a new update
  EXPECT_NEAR(output, integral * Ki, OUTPUT_TOLERANCE);
}
