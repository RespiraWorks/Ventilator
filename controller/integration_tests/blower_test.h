// Summary:
//      This test ramps the blower up and down repeatedly
//
// How to run:
//      TEST=TEST_BLOWER pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include "hal.h"

// test parameters
static constexpr int64_t delay_ms{10};
static constexpr float fan_min{TEST_PARAM_1};
static constexpr float fan_max{TEST_PARAM_2};
static constexpr float initial_step{0.002f};

void run_test() {
  Hal.init();

  float fan_power = fan_min;
  float step = initial_step;

  while (true) {
    Hal.analogWrite(PwmPin::BLOWER, fan_power);
    Hal.delay(milliseconds(delay_ms));

    Hal.watchdog_handler();

    fan_power += step;
    if (fan_power >= fan_max) {
      // switch to ramp-down state
      fan_power = fan_max;
      step = -step;
    } else if (fan_power <= fan_min) {
      // switch to ramp-up state
      fan_power = fan_min;
      step = -step;
    }
  }
}
