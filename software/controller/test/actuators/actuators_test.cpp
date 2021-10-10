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

#include "actuators.h"

#include "gtest/gtest.h"

// \TODO improve pwm and pinchvalve/stepper mocks to create tests for pwmActuator, pinch valves
// and actuators as a whole

TEST(Actuator, init) {
  Actuator actuator("default_actuator_", " of default actuator");
  // checking the associated debugvars' attributes, I know they are the last two we registered
  Debug::Variable::Base *default_cal = Debug::Variable::Registry::singleton().find(
      static_cast<uint16_t>(Debug::Variable::Registry::singleton().count() - 2));
  Debug::Variable::Float *forced =
      reinterpret_cast<Debug::Variable::Float *>(Debug::Variable::Registry::singleton().find(
          static_cast<uint16_t>(Debug::Variable::Registry::singleton().count() - 1)));

  EXPECT_STREQ(default_cal->name(), "default_actuator_cal");
  EXPECT_STREQ(default_cal->units(), "");
  EXPECT_STREQ(default_cal->help(), "Calibration table of default actuator");

  EXPECT_STREQ(forced->name(), "forced_default_actuator_setting");
  EXPECT_STREQ(forced->units(), "ratio");
  EXPECT_STREQ(forced->help(),
               "Force the setting of default actuator"
               " to a certain value in [0,1].  Specify a value outside"
               " this range to let the controller control it.");
  EXPECT_EQ(forced->get(), -1.0f);
};

TEST(Actuator, get_value) {
  std::array<float, ActuatorsCalSize> array{-1.f, -.5f, 0, 0.5f, 1.f, 2.f, 3.f, 5.f, 6.f, 8.f, 9.f};
  Interpolant<ActuatorsCalSize> test_interpolant("test", array);
  Actuator actuator("test_actuator_", " of test actuator", array);

  // Get the associated forced debugvar so I can force setting
  Debug::Variable::Float *forced =
      reinterpret_cast<Debug::Variable::Float *>(Debug::Variable::Registry::singleton().find(
          static_cast<uint16_t>(Debug::Variable::Registry::singleton().count() - 1)));

  // checking the correct use of interpolant outside of the interpolation grid
  float input = 0.1453f;
  // check that forced_ allows us to force value (ignore input value) and also uses calibration
  forced->set(input);
  EXPECT_EQ(actuator.get_value(0.5f), test_interpolant.get_value(input));

  // check that forcing outside [0,1] brings back our "normal" behaviour
  forced->set(2.0f);
  EXPECT_EQ(actuator.get_value(input), test_interpolant.get_value(input));
};
