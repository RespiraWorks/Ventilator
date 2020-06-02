#include "hal.h"
#include "stepper.h"
#include <cmath>

StepMotor *mtr_{nullptr};

void run_test() {
  float step_degrees{-30.0f};
  int64_t delay_ms{1000};

  Hal.init();
  mtr_ = StepMotor::GetStepper(0);
  mtr_->SetAmpAll(0.1f);
  mtr_->SetMaxSpeed(100.0f);
  mtr_->SetAccel(100.0f / 0.1f);
  mtr_->ClearPosition();

  while (true) {
    mtr_->MoveRel(step_degrees);
    Hal.delay(milliseconds(delay_ms));

    Hal.watchdog_handler();
  }
}
