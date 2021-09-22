
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
#include "stepper.h"
#include "system_timer.h"

// test parameters
static constexpr int MotorIndex{TEST_PARAM_1};
static constexpr float StepDegrees{TEST_PARAM_2};
static constexpr Duration Delay{milliseconds(1000)};

void RunTest() {
  hal.Init();

  // Configure stepper
  StepMotor *stepper_motor = StepMotor::GetStepper(MotorIndex);
  stepper_motor->SetAmpAll(0.1f);
  stepper_motor->SetMaxSpeed(100.0f);
  stepper_motor->SetAccel(100.0f / 0.1f);
  stepper_motor->ClearPosition();

  while (true) {
    stepper_motor->MoveRel(StepDegrees);
    SystemTimer::singleton().Delay(Delay);

    hal.WatchdogHandler();
  }
}
