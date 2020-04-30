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

#include "hal.h"
#include "gtest/gtest.h"

class BlowerFsmTest : public ::testing::Test {
public:
  BlowerFsmTest() = default;
  void SetUp() override { blower_fsm_init(); }
};

TEST_F(BlowerFsmTest, InitiallyOff) {
  VentParams p = VentParams_init_zero;
  EXPECT_EQ(blower_fsm_get_setpoint(p), kPa(0));
}

TEST_F(BlowerFsmTest, StaysOff) {
  VentParams p = VentParams_init_zero;
  Hal.delay(1000);
  EXPECT_EQ(blower_fsm_get_setpoint(p), kPa(0));
}

TEST_F(BlowerFsmTest, PressureControl) {
  VentParams p = VentParams_init_zero;
  p.mode = VentMode_PRESSURE_CONTROL;
  // 20 breaths/min = 3s/breath.  I:E = 2 means 2s for inspire, 1s for expire.
  p.breaths_per_min = 20;
  p.inspiratory_expiratory_ratio = 2;
  p.peep_cm_h2o = 10;
  p.pip_cm_h2o = 20;

  EXPECT_EQ(blower_fsm_get_setpoint(p).cmH2O(), 20);
  Hal.delay(1000); // t = 1000
  EXPECT_EQ(blower_fsm_get_setpoint(p).cmH2O(), 20);
  Hal.delay(999); // t = 1999
  EXPECT_EQ(blower_fsm_get_setpoint(p).cmH2O(), 20);

  Hal.delay(2); // t = 2001
  EXPECT_EQ(blower_fsm_get_setpoint(p).cmH2O(), 10);
  Hal.delay(998); // t = 2999
  EXPECT_EQ(blower_fsm_get_setpoint(p).cmH2O(), 10);

  Hal.delay(2); // t = 3001
  EXPECT_EQ(blower_fsm_get_setpoint(p).cmH2O(), 20);
}
