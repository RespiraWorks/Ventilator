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
static constexpr Duration kDelay{milliseconds(10)};
static constexpr float kFanMin{TEST_PARAM_1};
static constexpr float kFanMax{TEST_PARAM_2};
static constexpr float kInitialStep{0.002f};

void RunTest() {
  hal.Init();

  float fan_power = kFanMin;
  float step = kInitialStep;

  while (true) {
    hal.AnalogWrite(PwmPin::Blower, fan_power);
    hal.Delay(kDelay);

    hal.WatchdogHandler();

    fan_power += step;
    if (fan_power >= kFanMax) {
      // switch to ramp-down state
      fan_power = kFanMax;
      step = -step;
    } else if (fan_power <= kFanMin) {
      // switch to ramp-up state
      fan_power = kFanMin;
      step = -step;
    }
  }
}
