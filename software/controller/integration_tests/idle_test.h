// Summary:
//      This does nothing. Good to have when testing deployment and maintaining
//      sanity by making them shut up :)
//
// How to run:
//      TEST=TEST_IDLE pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include "hal.h"
#include "pwm_actuator.h"
#include "system_constants.h"
#include "system_timer.h"

// test parameters
static constexpr Duration Delay{milliseconds(10)};

void RunTest() {
  hal.Init();

  // Just to shut it up, may not need this beyond v0.3
  PwmActuator blower{BlowerChannel, BlowerFreq, HalApi::CPUFrequency(), "", ""};
  blower.set(0.0f);

  while (true) {
    SystemTimer::singleton().delay(Delay);
  }
}
