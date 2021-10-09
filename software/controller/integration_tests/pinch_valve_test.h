
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
#include "system_timer.h"
#include "watchdog.h"

// test parameters
static constexpr int MotorIndex{TEST_PARAM_1};
static constexpr Duration Delay{milliseconds(1000)};
static NVParams::Handler nv_params;
static I2Ceeprom eeprom = I2Ceeprom(0x50, 64, 32768, &i2c1);
static PinchValve pinch_valve("pinch_valve_", " for pinch valve", MotorIndex);

void RunTest() {
  hal.Init();
  nv_params.Init(&eeprom);
  pinch_valve.LinkCalibration(&nv_params, offsetof(NVParams::Structure, blower_pinch_cal));
  pinch_valve.Home();

  bool valve_open{false};
  while (true) {
    pinch_valve.SetOutput(valve_open ? 1.0f : 0.0f);
    SystemTimer::singleton().delay(Delay);

    Watchdog::pet();

    valve_open = !valve_open;
  }
}
