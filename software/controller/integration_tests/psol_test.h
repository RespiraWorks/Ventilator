// Summary:
//      This test cycles the proportional solenoid between two positions
//
// How to run:
//      deploy_test.sh <device_id> psol <min> <max> <step>
//
// Automation:
//      TBD - which python script to run?
//

#include "hal.h"

// test parameters
static constexpr Duration kDelay{milliseconds(10)};
static constexpr float kPSolMin{TEST_PARAM_1};
static constexpr float kPSolMax{TEST_PARAM_2};
static constexpr float kInitialStep{TEST_PARAM_3};

void RunTest() {
  hal.Init();

  float psol_position = kPSolMin;
  float step = kInitialStep;

  while (true) {
    hal.PSolValue(psol_position);
    hal.Delay(kDelay);

    hal.WatchdogHandler();

    psol_position += step;
    if (psol_position >= kPSolMax) {
      // switch to ramp-down state
      psol_position = kPSolMax;
      step = -step;
    } else if (psol_position <= kPSolMin) {
      // switch to ramp-up state
      psol_position = kPSolMin;
      step = -step;
    }
  }
}
