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
static constexpr int64_t delay_ms{10};
static constexpr float psol_min{TEST_PARAM_1};
static constexpr float psol_max{TEST_PARAM_2};
static constexpr float initial_step{TEST_PARAM_3};

void run_test() {
  Hal.init();

  float psol_position = psol_min;
  float step = initial_step;

  while (true) {
    Hal.PSOL_Value(psol_position);
    Hal.delay(milliseconds(delay_ms));

    Hal.watchdog_handler();

    psol_position += step;
    if (psol_position >= psol_max) {
      // switch to ramp-down state
      psol_position = psol_max;
      step = -step;
    } else if (psol_position <= psol_min) {
      // switch to ramp-up state
      psol_position = psol_min;
      step = -step;
    }
  }
}
