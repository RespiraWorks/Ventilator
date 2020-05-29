#include "hal.h"

int main() {
  Hal.init();

  float fan_min{0.0};
  float fan_max{0.5};
  float step{0.001f};

  float fan_power = fan_min;

  while (true) {
    Hal.analogWrite(PwmPin::BLOWER, fan_power);
    Hal.delay(milliseconds(10));

    Hal.watchdog_handler();

    fan_power += step;
    if (fan_power >= fan_max) {
      fan_power = fan_max;
      step = -step;
    } else if (fan_power <= fan_min) {
      fan_power = fan_min;
      step = -step;
    }
  }
}
