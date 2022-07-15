
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
#include "pwm_actuator.h"
#include "system_constants.h"
#include "system_timer.h"
#include "watchdog.h"

// test parameters
static constexpr int MotorIndex{TEST_PARAM_1};
static constexpr Duration Delay{milliseconds(1000)};
static PinchValve pinch_valve("pinch_valve_", " for pinch valve", MotorIndex);

void RunTest() {
  hal.Init();

  // Just to shut it up, may not need this beyond v0.3
  PwmActuator blower{BlowerChannel, BlowerFreq, HalApi::CPUFrequency(), "", ""};
  blower.set(0.0f);

  StepMotor::OneTimeInit();
  pinch_valve.Home();

  bool valve_open{false};
  while (true) {
    pinch_valve.SetOutput(valve_open ? 1.0f : 0.0f);
    SystemTimer::singleton().delay(Delay);

    Watchdog::pet();

    valve_open = !valve_open;
  }
}
