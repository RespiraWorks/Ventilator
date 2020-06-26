
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
static constexpr float step_degrees{-30.0f};
static constexpr int64_t delay_ms{1000};

void run_test() {
  Hal.init();

  // Configure stepper
  StepMotor *stepper_motor = StepMotor::GetStepper(1);
  stepper_motor->SetAmpAll(0.1f);
  stepper_motor->SetMaxSpeed(100.0f);
  stepper_motor->SetAccel(100.0f / 0.1f);
  stepper_motor->ClearPosition();

  while (true) {
    stepper_motor->MoveRel(step_degrees);
    Hal.delay(milliseconds(delay_ms));

    Hal.watchdog_handler();
  }
}
