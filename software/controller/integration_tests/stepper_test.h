
// Summary:
//      This test rotates a stepper clockwise at a predefined rate and steps
//
// How to run:
//      TEST=TEST_STEPPER pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include <cmath>

#include "hal.h"
#include "pwm_actuator.h"
#include "stepper.h"
#include "system_constants.h"
#include "system_timer.h"
#include "watchdog.h"

// test parameters
static constexpr int MotorIndex{TEST_PARAM_1};
static constexpr float StepDegrees{TEST_PARAM_2};
static constexpr Duration Delay{milliseconds(1000)};

void RunTest() {
  hal.Init(CPUFrequency);

  // Just to shut it up, may not need this beyond v0.3
  PwmActuator blower{BlowerChannel, BlowerFreq, CPUFrequency, "", ""};
  blower.set(0.0f);

  StepMotor::OneTimeInit();

  // Configure stepper
  StepMotor *stepper_motor = StepMotor::GetStepper(MotorIndex);
  stepper_motor->SetAmpAll(0.1f);
  stepper_motor->SetMaxSpeed(100.0f);
  stepper_motor->SetAccel(100.0f / 0.1f);
  stepper_motor->ClearPosition();

  while (true) {
    stepper_motor->MoveRel(StepDegrees);
    SystemTimer::singleton().delay(Delay);

    Watchdog::pet();
  }
}
