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

#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>

#include "hal.h"

// These constants define various properties of the pinch
// value and how we control it.  As the mechanical design
// of the value continues to evolve these constants may
// need to change.  Once things stabilize however, we should
// be able to come up with a good set of values here that
// won't need further modification.

// MoveDir should be set to either 1 or -1 depending on the
// desired direction of motion of the pinch valve.
static constexpr float MoveDir = -1.0f;

// Amplitude of the power level used for homing.
// 0.1 is 10% of maximum.  There's no need for high
// power when homing, we're intentionally driving the
// motor against a hard stop.
static constexpr float HomeAmp = 0.1f;

// Velocity (deg/sec), acceleration (deg/sec/sec)
// and distance (deg) to move during the home
static constexpr float HomeVel = 60.0f;
static constexpr float HomeAccel = HomeVel / 0.1f;
static constexpr float HomeDist = 90.0f * MoveDir;

// Amount we can move away from the homing end stop before
// we start to touch the tube (deg)
static constexpr float HomeOffset = 30.0f * MoveDir;

// This is the distance (deg) from the zero position until
// the tube is completely shut.
static constexpr float MaxMove = 50.0f * MoveDir;

// Amplitude of power level for normal operation.
// Don't go crazy here, you can easily overheat the
// motor driver chips and stepper motors.
static constexpr float MoveAmp = 0.25f;

// Speed/accel for normal moves.  We want the motor
// to be quick because the PID loop output will be
// small move and ideally we want it to finish before
// the next loop cycle
static constexpr float MoveVel = 2000.0f;
static constexpr float MoveAccel = MoveVel / 0.05f;

// This table is used to roughly linearize the pinch valve
// output.  It was built by adjusting the pinch valve and
// monitoring the flow through the venturi tube.
// The entries should give pinch valve settings for a list
// of equally spaced flow rates.  The first entry should be
// the setting for 0 flow rate (normally 0) and the last entry
// should be the setting for 100% flow rate.  The minimum
// length of the table is 2 entries.
static constexpr float FlowTable[] = {0.0000f, 0.0410f, 0.0689f, 0.0987f, 0.1275f, 0.1590f,
                                      0.1932f, 0.2359f, 0.2940f, 0.3988f, 1.0000f};

Debug::Variable::FloatArray<11> PinchValve::calibration = Debug::Variable::FloatArray<11>(
    "pinch_valve_cal", Debug::Variable::Access::ReadWrite, "", "Pinch valve linearization table");

PinchValve::PinchValve(int motor_index) : motor_index_(motor_index) {
  calibration.data[0] = 0.0000f;
  calibration.data[1] = 0.0410f;
  calibration.data[2] = 0.0689f;
  calibration.data[3] = 0.0987f;
  calibration.data[4] = 0.1275f;
  calibration.data[5] = 0.1590f;
  calibration.data[6] = 0.1932f;
  calibration.data[7] = 0.2359f;
  calibration.data[8] = 0.2940f;
  calibration.data[9] = 0.3988f;
  calibration.data[10] = 1.0000f;
}

// Disable the pinch valve
void PinchValve::Disable() {
  StepMotor *mtr = StepMotor::GetStepper(motor_index_);
  if (!mtr) return;

  home_state_ = PinchValveHomeState::Disabled;
  mtr->HardDisable();
}

// This runs through the homing procedure.  The pinch valves need
// to be homed before they can be used, and this needs to be done
// any time the valve is first enabled
//
void PinchValve::Home() {
  StepMotor *mtr = StepMotor::GetStepper(motor_index_);
  if (!mtr) return;

  StepMtrErr err;

  switch (home_state_) {
    case PinchValveHomeState::Disabled:
      home_state_ = PinchValveHomeState::LowerAmp;
      // fall through

    // Limit motor power during homing.
    case PinchValveHomeState::LowerAmp:
      err = mtr->SetAmpAll(HomeAmp);
      if (err == StepMtrErr::Ok) home_state_ = PinchValveHomeState::SetHomeSpeed;
      break;

    // Set the move speed/accel to be used during homing
    case PinchValveHomeState::SetHomeSpeed:
      err = mtr->SetMaxSpeed(HomeVel);
      if (err == StepMtrErr::Ok) err = mtr->SetAccel(HomeAccel);
      if (err == StepMtrErr::Ok) home_state_ = PinchValveHomeState::MoveToStop;
      break;

    // Start a relative move into the hard stop
    case PinchValveHomeState::MoveToStop:
      move_start_time_ = hal.Now();
      err = mtr->MoveRel(HomeDist);
      if (err == StepMtrErr::Ok) home_state_ = PinchValveHomeState::WaitMoveStop;
      break;

    // Wait for the move to hard stop to end
    case PinchValveHomeState::WaitMoveStop: {
      Duration dt = hal.Now() - move_start_time_;
      if (dt.seconds() >= 3.0f) home_state_ = PinchValveHomeState::SetNormalAmp;
      break;
    }

    // Switch to normal power setting
    case PinchValveHomeState::SetNormalAmp:
      err = mtr->SetAmpAll(MoveAmp);
      if (err == StepMtrErr::Ok) home_state_ = PinchValveHomeState::MoveOffset;
      break;

    // Make a relative move away from the hard stop.
    // This should cause us to end up with the bearings
    // just touching the tube, but not squeezing it.
    case PinchValveHomeState::MoveOffset:
      move_start_time_ = hal.Now();
      err = mtr->MoveRel(-HomeOffset);
      if (err == StepMtrErr::Ok) home_state_ = PinchValveHomeState::WaitMoveOffset;
      break;

    case PinchValveHomeState::WaitMoveOffset: {
      Duration dt = hal.Now() - move_start_time_;
      if (dt.seconds() >= 2.0f) home_state_ = PinchValveHomeState::ZeroPos;
      break;
    }

    // Set the current position to zero
    case PinchValveHomeState::ZeroPos:
      err = mtr->ClearPosition();
      if (err == StepMtrErr::Ok) home_state_ = PinchValveHomeState::SetNormalSpeed;
      break;

    // Switch to normal move speed/accel
    case PinchValveHomeState::SetNormalSpeed:

      err = mtr->SetMaxSpeed(MoveVel);
      if (err == StepMtrErr::Ok) err = mtr->SetAccel(MoveAccel);
      if (err == StepMtrErr::Ok) home_state_ = PinchValveHomeState::Homed;

    case PinchValveHomeState::Homed:
      break;
  }
}

void PinchValve::SetOutput(float value) {
  if (home_state_ != PinchValveHomeState::Homed) {
    Home();
    return;
  }

  StepMotor *mtr = StepMotor::GetStepper(motor_index_);
  if (!mtr) return;

  value = std::clamp(value, 0.0f, 1.0f);

  // Number of intervals defined by the table.
  float tbl_len = std::size(FlowTable) - 1;

  // Convert the input value based on a table
  // used to linearize the pinch valve output
  int n = static_cast<int>(value * tbl_len);
  float f = value * tbl_len - static_cast<float>(n);

  if (n == static_cast<int>(tbl_len))
    value = FlowTable[n];
  else
    value = FlowTable[n] + f * (FlowTable[n + 1] - FlowTable[n]);

  // Convert the value to an absolute position in deg
  // The motor's zero position is at the home offset
  // which corresponds to fully open (i.e. 100% flow)
  // The valve is closed at a position of -MaxMove;
  float pos = (value - 1.0f) * MaxMove;

  // Once you put a move in motion you can't change the destination position
  // until the move ends. That means that if the servo loop commands a
  // relatively large move of the valve in one cycle, then any new commands
  // will be ignored until that move finishes. This can cause the pinch valve
  // to oscillate if you turn the gains up too high.
  //
  // To fix this, we hard-stop the valve before sending a new position. This
  // doesn't do anything if the valve was already stopped, but if it was in the
  // middle of a big move it tells it to abandon that old move and start a new
  // move to the new commanded position.
  //
  // This seems to really smooth out the pinch valve motion and allow for
  // higher gains.
  if (fabsf(pos - last_command_) > FLT_EPSILON) mtr->HardStop();

  last_command_ = pos;
  mtr->GotoPos(pos);
}
