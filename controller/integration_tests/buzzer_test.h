
// Summary:
//      This test plays buzzer at various volumes
//
// How to run:
//      TEST=TEST_BUZZER pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//
#include "hal.h"

// test parameters
static constexpr int64_t delay_ms{1000};
static constexpr float volume_min{TEST_PARAM_1};
static constexpr float volume_max{TEST_PARAM_2};
static constexpr float initial_step{0.1f};

void run_test() {
  Hal.init();

  float volume = volume_min;
  float step = initial_step;

  bool buzzer_on{false};

  while (true) {
    if (buzzer_on) {
      Hal.BuzzerOn(volume);
    } else {
      Hal.BuzzerOff();
    }

    Hal.delay(milliseconds(delay_ms));

    buzzer_on = !buzzer_on;
    if (buzzer_on) {
      volume += step;
      if (volume >= volume_max) {
        // switch to ramp-down state
        volume = volume_max;
        step = -step;
      } else if (volume <= volume_min) {
        // switch to ramp-up state
        volume = volume_min;
        step = -step;
      }
    }
  }
}
