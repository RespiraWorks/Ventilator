
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
static constexpr Duration kDelay{milliseconds(1000)};
static constexpr float kMinVolume{TEST_PARAM_1};
static constexpr float kMaxVolume{TEST_PARAM_2};
static constexpr float kInitialStep{0.1f};

void RunTest() {
  hal.Init();

  float volume = kMinVolume;
  float step = kInitialStep;

  bool buzzer_on{false};

  while (true) {
    if (buzzer_on) {
      hal.BuzzerOn(volume);
    } else {
      hal.BuzzerOff();
    }

    hal.Delay(kDelay);

    buzzer_on = !buzzer_on;
    if (buzzer_on) {
      volume += step;
      if (volume >= kMaxVolume) {
        // switch to ramp-down state
        volume = kMaxVolume;
        step = -step;
      } else if (volume <= kMinVolume) {
        // switch to ramp-up state
        volume = kMinVolume;
        step = -step;
      }
    }
  }
}
