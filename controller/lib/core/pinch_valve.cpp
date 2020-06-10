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
#include "hal.h"
#include "stepper.h"
#include <algorithm>
#include <array>

// These constants define various properties of the pinch
// value and how we control it.  As the mechanical design
// of the value continues to evolve these constants may
// need to change.  Once things stabilize however, we should
// be able to come up with a good set of values here that
// won't need further modification.

// move_dir should be set to either 1 or -1 depending on the
// desired direction of motion of the pinch valve.
static constexpr float move_dir = -1.0f;

// Amplitude of the power level used for homing.
// 0.1 is 10% of maximum.  There's no need for high
// power when homing, we're intentionally driving the
// motor against a hard stop.
static constexpr float home_amp = 0.1f;

// Velocity (deg/sec), acceleration (deg/sec/sec)
// and distance (deg) to move during the home
static constexpr float home_vel = 60.0f;
static constexpr float home_accel = home_vel / 0.1f;
static constexpr float home_dist = 90.0f * move_dir;

// Amount we can move away from the homing end stop before
// we start to touch the tube (deg)
static constexpr float home_offset = 30.0f * move_dir;

// This is the distance (deg) from the zero position until
// the tube is completely shut.
static constexpr float max_move = 45.0f * move_dir;

// Amplitude of power level for normal operation.
// Don't go crazy here, you can easily overheat the
// motor driver chips and stepper motors.
static constexpr float move_amp = 0.25f;

// Speed/accel for normal moves.  We want the motor
// to be quick because the PID loop output will be
// small move and ideally we want it to finish before
// the next loop cycle
static constexpr float move_vel = 2000.0f;
static constexpr float move_acc = move_vel / 0.05f;

// This table is used to roughly linearize the pinch valve
// output.  It was built by adjusting the pinch valve and
// monitoring the flow through the venturi tube.
// The entries should give pinch valve settings for a list
// of equally spaced flow rates.  The first entry should be
// the setting for 0 flow rate (normally 0) and the last entry
// should be the setting for 100% flow rate.  The minimum
// length of the table is 2 entries.
static constexpr float flow_table[] = {0.0f,   0.125f, 0.161f, 0.197f,
                                       0.232f, 0.271f, 0.310f, 0.358f,
                                       0.425f, 0.542f, 0.9f};

PinchValve::PinchValve(int motor_index) { motor_index_ = motor_index; }

// Disable the pinch valve
void PinchValve::Disable() {

  StepMotor *mtr = StepMotor::GetStepper(motor_index_);
  if (!mtr)
    return;

  home_state_ = PinchValveHomeState::DISABLED;
  mtr->HardDisable();
}

// This runs through the homing procedure.  The pinch valves need
// to be homed before they can be used, and this needs to be done
// any time the valve is first enabled
//
void PinchValve::Home() {

  StepMotor *mtr = StepMotor::GetStepper(motor_index_);
  if (!mtr)
    return;

  StepMtrErr err;

  switch (home_state_) {

  case PinchValveHomeState::DISABLED:
    home_state_ = PinchValveHomeState::LOWER_AMP;
    // fall through

  // Limit motor power during homing.
  case PinchValveHomeState::LOWER_AMP:
    err = mtr->SetAmpAll(home_amp);
    if (err == StepMtrErr::OK)
      home_state_ = PinchValveHomeState::SET_HOME_SPEED;
    break;

  // Set the move speed/accel to be used during homing
  case PinchValveHomeState::SET_HOME_SPEED:
    err = mtr->SetMaxSpeed(home_vel);
    if (err == StepMtrErr::OK)
      err = mtr->SetAccel(home_accel);
    if (err == StepMtrErr::OK)
      home_state_ = PinchValveHomeState::MOVE_TO_STOP;
    break;

  // Start a relative move into the hard stop
  case PinchValveHomeState::MOVE_TO_STOP:
    move_start_time_ = Hal.now();
    err = mtr->MoveRel(home_dist);
    if (err == StepMtrErr::OK)
      home_state_ = PinchValveHomeState::WAIT_MOVE_STOP;
    break;

  // Wait for the move to hard stop to end
  case PinchValveHomeState::WAIT_MOVE_STOP: {
    Duration dt = Hal.now() - move_start_time_;
    if (dt.seconds() >= 3.0f)
      home_state_ = PinchValveHomeState::SET_NORMAL_AMP;
    break;
  }

  // Switch to normal power setting
  case PinchValveHomeState::SET_NORMAL_AMP:
    err = mtr->SetAmpAll(move_amp);
    if (err == StepMtrErr::OK)
      home_state_ = PinchValveHomeState::MOVE_OFFSET;
    break;

  // Make a relative move away from the hard stop.
  // This should cause us to end up with the bearings
  // just touching the tube, but not squeezing it.
  case PinchValveHomeState::MOVE_OFFSET:
    move_start_time_ = Hal.now();
    err = mtr->MoveRel(-home_offset);
    if (err == StepMtrErr::OK)
      home_state_ = PinchValveHomeState::WAIT_MOVE_OFFSET;
    break;

  case PinchValveHomeState::WAIT_MOVE_OFFSET: {
    Duration dt = Hal.now() - move_start_time_;
    if (dt.seconds() >= 2.0f)
      home_state_ = PinchValveHomeState::ZERO_POS;
    break;
  }

  // Set the current position to zero
  case PinchValveHomeState::ZERO_POS:
    err = mtr->ClearPosition();
    if (err == StepMtrErr::OK)
      home_state_ = PinchValveHomeState::SET_NORMAL_SPEED;
    break;

  // Switch to normal move speed/accel
  case PinchValveHomeState::SET_NORMAL_SPEED:

    err = mtr->SetMaxSpeed(move_vel);
    if (err == StepMtrErr::OK)
      err = mtr->SetAccel(move_acc);
    if (err == StepMtrErr::OK)
      home_state_ = PinchValveHomeState::HOMED;

  case PinchValveHomeState::HOMED:
    break;
  }
}

void PinchValve::SetOutput(float value) {

  if (home_state_ != PinchValveHomeState::HOMED) {
    Home();
    return;
  }

  StepMotor *mtr = StepMotor::GetStepper(motor_index_);
  if (!mtr)
    return;

  value = std::clamp(value, 0.0f, 1.0f);

  // Number of intervals defined by the table.
  float tbl_len = std::size(flow_table) - 1;

  // Convert the input value based on a table
  // used to linearize the pinch valve output
  int n = static_cast<int>(value * tbl_len);
  float f = value * tbl_len - static_cast<float>(n);

  if (n == static_cast<int>(tbl_len))
    value = flow_table[n];
  else
    value = flow_table[n] + f * (flow_table[n + 1] - flow_table[n]);

  // Convert the value to an absolute position in deg
  // The motor's zero position is at the home offset
  // which corresponds to fully open (i.e. 100% flow)
  // The valve is closed at a position of -max_move;

  float pos = (value - 1.0f) * max_move;
  mtr->GotoPos(pos);
}
