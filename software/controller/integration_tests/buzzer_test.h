
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
#include "system_timer.h"

// test parameters
static constexpr Duration Delay{milliseconds(1000)};
static constexpr float MinVolume{TEST_PARAM_1};
static constexpr float MaxVolume{TEST_PARAM_2};
static constexpr float InitialStep{0.1f};

void RunTest() {
  hal.Init();

  float volume = MinVolume;
  float step = InitialStep;

  bool buzzer_on{false};

  while (true) {
    if (buzzer_on) {
      hal.buzzer.set(volume);
    } else {
      hal.buzzer.set(0);
    }

    SystemTimer::singleton().delay(Delay);

    buzzer_on = !buzzer_on;
    if (buzzer_on) {
      volume += step;
      if (volume >= MaxVolume) {
        // switch to ramp-down state
        volume = MaxVolume;
        step = -step;
      } else if (volume <= MinVolume) {
        // switch to ramp-up state
        volume = MinVolume;
        step = -step;
      }
    }
  }
}
