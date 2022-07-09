
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
#include "pwm_actuator.h"
#include "system_constants.h"
#include "system_timer.h"

// test parameters
static constexpr Duration Delay{milliseconds(1000)};
static constexpr float MinVolume{TEST_PARAM_1};
static constexpr float MaxVolume{TEST_PARAM_2};
static constexpr float InitialStep{0.1f};

void RunTest() {
  hal.Init();

  // Just to shut it up, may not need this beyond v0.3
  PwmActuator blower{BlowerChannel, BlowerFreq, HalApi::CPUFrequency(), "", ""};
  blower.set(0.0f);

  float volume = MinVolume;
  float step = InitialStep;

  bool buzzer_on{false};

  PwmActuator buzzer{BuzzerChannel, BuzzerFreq,     HalApi::CPUFrequency(), "", "", "",
                     BuzzerOff,     MaxBuzzerVolume};

  while (true) {
    if (buzzer_on) {
      buzzer.set(volume);
    } else {
      buzzer.set(0);
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
