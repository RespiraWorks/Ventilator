#include "hal.h"

void run_test() {
  Hal.init();

  int64_t delay_ms{1000};

  bool solenoid_state = false;

  while (true) {
    Hal.digitalWrite(BinaryPin::SOLENOID,
                     solenoid_state ? VoltageLevel::LOW : VoltageLevel::HIGH);

    Hal.delay(milliseconds(delay_ms));

    Hal.watchdog_handler();

    solenoid_state = !solenoid_state;
  }
}
