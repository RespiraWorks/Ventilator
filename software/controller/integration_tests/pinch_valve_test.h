
// Summary:
//      This test repeatedly opens and closes a pinch valve
//
// How to run:
//      TEST=TEST_PINCH_VALVE pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include "hal.h"
#include "pinch_valve.h"

// test parameters
static constexpr int kMotorIndex{TEST_PARAM_1};
static constexpr Duration kDelay{milliseconds(1000)};

void RunTest() {
  hal.Init();
  PinchValve pinch_valve(kMotorIndex);
  pinch_valve.Home();

  bool valve_open{false};
  while (true) {
    pinch_valve.SetOutput(valve_open ? 1.0f : 0.0f);
    hal.Delay(kDelay);

    hal.WatchdogHandler();

    valve_open = !valve_open;
  }
}
