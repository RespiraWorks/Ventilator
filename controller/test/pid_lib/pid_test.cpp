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

//@TODO: Adjust this tolerance... assumes the PWM is a 0-255 integer but that
// may not be the case
static const double output_TOLERANCE = 1;

// PID
static double setpoint;
static double input;
static double output;

// default PID gains
static float Kp;
static float Ki;
static float Kd;

// PID Sample time (in milliseconds)
static uint32_t sample_time = 100;

// Maximum task jitter (assume 5 ms for now, will need adjuting)
static const uint32_t max_task_jitter = 5;

TEST(pidTest, Proportional) {
  Kp = 0.9;
  Ki = 0;
  Kd = 0;
  setpoint = 25;
  input = 10;
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.SetMode(AUTOMATIC);
  myPID.Compute();
  // Ki and Kd = 0 therefore output = Kp*error, nothing more
  EXPECT_NEAR(output, (setpoint - input) * Kp, output_TOLERANCE);
  Hal.delay(sample_time);
  myPID.Compute();
  EXPECT_NEAR(output, (setpoint - input) * Kp, output_TOLERANCE);
}

TEST(pidTest, Integral) {
  Kp = 0;
  Ki = 1.75;
  Kd = 0;
  // reset output to 0 since PID lib uses output to initalise its integral
  output = 0;
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.SetMode(AUTOMATIC);
  myPID.Compute();
  // on first call, integral = error*sample time, output = Ki*integral
  EXPECT_NEAR(output, (setpoint - input) * sample_time / 1000.0 * Ki,
              output_TOLERANCE);
  Hal.delay(sample_time);
  myPID.Compute();
  // on second call, integral = error*2*sample time, output = Ki*integral
  EXPECT_NEAR(output, (setpoint - input) * 2 * sample_time / 1000.0 * Ki,
              output_TOLERANCE);
}

TEST(pidTest, derivative) {
  Kp = 0;
  Ki = 0;
  Kd = 2.5;
  output = 0;
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.SetMode(AUTOMATIC);
  myPID.Compute();
  // Expect no derivative on first call
  EXPECT_NEAR(output, 0, output_TOLERANCE);

  // delay and update input to create non-zero derivative
  Hal.delay(sample_time);
  double derivative = input;
  // output being in [0 255], create a negative derivative in order to have
  // a positive output
  input = 5;
  derivative = (input - derivative) / sample_time * 1000.0;

  myPID.Compute();
  EXPECT_NEAR(output, derivative * -1 * Kd, output_TOLERANCE);
}

TEST(pidTest, TaskJitter) {
  // This test uses integral to check the effect of time between calls on the
  // PID output. Introducing jitter in call frequency and checking that the
  // integral takes this jitter into account.
  Kp = 0;
  Ki = 0.5;
  Kd = 0;
  output = 0;
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.SetMode(AUTOMATIC);
  myPID.Compute();
  // First task, with only Ki set, output = error*sample_time
  double integral = (setpoint - input) * (sample_time / 1000.0);
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
  // Advance time with jitter
  uint32_t dt = sample_time + max_task_jitter;
  Hal.delay(dt);
  myPID.Compute();
  // Expect output to take jitter into account in integral
  integral += (setpoint - input) * (dt / 1000.0);
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
  // Advance time and compensate previous jitter to have total time
  // elapsed = 2*sample time
  dt = sample_time - max_task_jitter;
  Hal.delay(dt);
  integral += (setpoint - input) * (dt / 1000.0);
  myPID.Compute();
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
}

TEST(pidTest, SampleTimeChange) {
  // This test uses integral to check the effect of changing sample time
  // during execution of the PID
  Kp = 0;
  Ki = 1.1;
  Kd = 0;
  output = 0;
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.SetMode(AUTOMATIC);
  myPID.Compute();

  // First task, with only Ki set, output = error*sample_time
  double integral = (setpoint - input) * (sample_time / 1000.0);
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
  // Advance time
  uint32_t dt = sample_time;
  Hal.delay(dt);
  myPID.Compute();
  // Expect output to integrate this
  integral += (setpoint - input) * (dt / 1000.0);
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
  // Change sample time and advance
  sample_time = 50;
  dt = sample_time;
  myPID.SetSampleTime(sample_time);
  Hal.delay(dt);
  integral += (setpoint - input) * (dt / 1000.0);
  myPID.Compute();
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
}

TEST(pidTest, MissedSample) {
  // This test uses integral to check the effect of missing a sample in the
  // execution of PID
  Kp = 0;
  Ki = 0.2;
  Kd = 0;
  output = 0;
  PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
  myPID.SetSampleTime(sample_time);
  myPID.SetMode(AUTOMATIC);
  myPID.Compute();

  // First task, with only Ki set, output = error*sample_time
  double integral = (setpoint - input) * (sample_time / 1000.0);
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
  // Advance time
  uint32_t dt = 2 * sample_time;
  Hal.delay(dt);
  myPID.Compute();
  // Expect output to integrate this
  integral += (setpoint - input) * (dt / 1000.0);
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
  // Advance time a small amount to allow PID to catch up to missed sample
  dt = max_task_jitter;
  Hal.delay(dt);
  integral += (setpoint - input) * (dt / 1000.0);
  myPID.Compute();
  // Expect output to have a new update
  EXPECT_NEAR(output, integral * Ki, output_TOLERANCE);
}
