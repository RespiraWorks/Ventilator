
// Summary:
//      This test rotates a stepper clockwise at a predefined rate and steps
//
// How to run:
//      TEST=TEST_STEPPER pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include "hal.h"
#include "stepper.h"
#include <cmath>

// test parameters
static constexpr int kMotorIndex{TEST_PARAM_1};
static constexpr float kStepDegrees{TEST_PARAM_2};
static constexpr Duration kDelay{milliseconds(1000)};

void RunTest() {
  hal.Init();

  // Configure stepper
  StepMotor *stepper_motor = StepMotor::GetStepper(kMotorIndex);
  stepper_motor->SetAmpAll(0.1f);
  stepper_motor->SetMaxSpeed(100.0f);
  stepper_motor->SetAccel(100.0f / 0.1f);
  stepper_motor->ClearPosition();

  while (true) {
    stepper_motor->MoveRel(kStepDegrees);
    hal.Delay(kDelay);

    hal.WatchdogHandler();
  }
}
