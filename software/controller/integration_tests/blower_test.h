// Summary:
//      This test ramps the blower up and down repeatedly
//
// How to run:
//      TEST=TEST_BLOWER pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include "actuator_base.h"
#include "hal.h"
#include "system_timer.h"
#include "watchdog.h"

// test parameters
static constexpr Duration Delay{milliseconds(10)};
static constexpr float FanMin{TEST_PARAM_1};
static constexpr float FanMax{TEST_PARAM_2};
static constexpr float InitialStep{0.002f};

void RunTest() {
  hal.Init();
  PwmActuator blower{PwmPin::Blower, 20000, "blower_", " of the blower"};
  blower.initialize_pwm(HalApi::GetCpuFreq());

  float fan_power = FanMin;
  float step = InitialStep;

  while (true) {
    blower.set(fan_power);
    SystemTimer::singleton().delay(Delay);

    Watchdog::pet();

    fan_power += step;
    if (fan_power >= FanMax) {
      // switch to ramp-down state
      fan_power = FanMax;
      step = -step;
    } else if (fan_power <= FanMin) {
      // switch to ramp-up state
      fan_power = FanMin;
      step = -step;
    }
  }
}
