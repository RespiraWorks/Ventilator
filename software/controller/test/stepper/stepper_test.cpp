/* Copyright 2021, RespiraWorks

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

#include "stepper.h"
#include "gtest/gtest.h"

// Not really tests, just silencing code coverage warning for native build
TEST(Stepper, TestStubs) {
  StepMotor step_motor;
  EXPECT_EQ(StepMtrErr::OK, step_motor.HardDisable());
  EXPECT_EQ(StepMtrErr::OK, step_motor.SetAmpAll(0.0));
  EXPECT_EQ(StepMtrErr::OK, step_motor.SetMaxSpeed(0.0));
  EXPECT_EQ(StepMtrErr::OK, step_motor.SetAccel(0.0));
  EXPECT_EQ(StepMtrErr::OK, step_motor.MoveRel(0.0));
  EXPECT_EQ(StepMtrErr::OK, step_motor.ClearPosition());
  EXPECT_EQ(StepMtrErr::OK, step_motor.GotoPos(0.0));
  EXPECT_EQ(StepMtrErr::OK, step_motor.HardStop());
}
