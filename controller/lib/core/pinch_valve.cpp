/* Copyright 2020, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "pinch_valve.h"
#include "stepper.h"

// These constants define various properties of the pinch
// value and how we control it.  As the mechanical design
// of the value continues to evolve these constants may
// need to change.  Once things stabilize however, we should
// be able to come up with a good set of values here that
// won't need further modification.

// Amplitude of the power level used for homing
// 0.1 is 10% of maximum.  There's no need for high
// power when homing, we're intentionally driving the
// motor against a hard stop.
constexpr float home_amp = 0.1f;

// Velocity (deg/sec), acceleration (deg/sec/sec)
// and distance (deg) to move during the home
constexpr float home_vel = 60.0f;
constexpr float home_accel = home_vel / 0.1f;
constexpr float home_dist = 90.0f;

// Amount we can move away from the homing end stop before
// we start to touch the tube (deg)
constexpr float home_offset = 20.0f;

// This is the distance (deg) from the zero position until
// the tube is completely shut.
constexpr float max_move = 50.0f;

// Amplitude of power level for normal operation.
// Don't go crazy here, you can easily overheat the
// motor driver chips and stepper motors.
constexpr float move_amp = 0.2f;

// Speed/accel for normal moves.  We want the motor
// to be quick because the PID loop output will be
// small move and ideally we want it to finish before
// the next loop cycle
constexpr float move_vel = 2000.0f;
constexpr float move_acc = move_vel / 0.05f;

PinchValve::PinchValve(int motor_index) {

  // Find the stepper motor associated with this
  // pinch valve.
  //
  // NOTE - If this returns NULL then it means the
  // constant in stepper.h is wrong.
  // If mtr ends up being NULL I can never home or
  // move
  mtr_ = StepMotor::GetStepper(motor_index);
}

// Initialize the pinch value position at startup
void PinchValve::Home() {

  homed_ = false;
  if (!mtr_)
    return;

  // I'll reset the stepper driver first,
  // that puts it in a known state so I don't have
  // to worry about setting everything
  StepMtrErr err;
  err = mtr_->Reset();
  if (err != StepMtrErr::OK)
    return;

  // Limit motor power during homing
  err = mtr_->SetAmpAll(home_amp);
  if (err != StepMtrErr::OK)
    return;

  // Make a relative move in the positive direction
  // that should be away from the tubing.
  // My move is far enough that I should definitely
  // hit the hard stop
  err = mtr_->SetMaxSpeed(home_vel);
  if (err != StepMtrErr::OK)
    return;

  err = mtr_->SetAccel(home_accel);
  if (err != StepMtrErr::OK)
    return;

  err = mtr_->MoveRel(home_dist);
  if (err != StepMtrErr::OK)
    return;

  // Wait for this move to finish.  When it does,
  // I should be up against the hard stop.
  err = WaitForMove();
  if (err != StepMtrErr::OK)
    return;

  // Switch to noram power setting
  err = mtr_->SetAmpAll(move_amp);
  if (err != StepMtrErr::OK)
    return;

  // Make a relative move away from the hard stop.
  // This should cause us to end up with the bearings
  // just touching the tube, but not squeezing it.
  err = mtr_->MoveRel(-home_offset);
  if (err != StepMtrErr::OK)
    return;

  err = WaitForMove();
  if (err != StepMtrErr::OK)
    return;

  // Set this position to zero.
  err = mtr_->ClearPosition();
  if (err != StepMtrErr::OK)
    return;

  // Switch to normal move speed/accel
  err = mtr_->SetMaxSpeed(move_vel);
  if (err != StepMtrErr::OK)
    return;

  err = mtr_->SetAccel(move_acc);
  if (err != StepMtrErr::OK)
    return;

  homed_ = true;
}

void PinchValve::SetOutput(float value) {
  if (!homed_)
    return;

  if (value < 0.0f)
    value = 0.0f;
  if (value > 1.0f)
    value = 1.0f;

  // Convert the value to an absolute position in deg
  // The motor's zero position is at the home offset
  // which corresponds to fully open (i.e. 100% flow)
  // The valve is closed at a position of -max_move;

  float pos = (value - 1.0f) * max_move;
  mtr_->GotoPos(pos);
}

// Wait for the move to finish.  Only used when homing
StepMtrErr PinchValve::WaitForMove() {
  while (true) {
    StepperStatus status;
    StepMtrErr err = mtr_->GetStatus(&status);
    if (err != StepMtrErr::OK)
      return err;

    if (status.move_status == StepMoveStatus::STOPPED)
      return StepMtrErr::OK;
  }
}
