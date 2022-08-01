/* Copyright 2021, RespiraWorks

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

#include "stepper.h"

#include "gtest_main.h"
#include "mock_stepper.h"

using namespace StepMotor;

// Template class that encapsulates several TestSteppers along with the necessary tools to feed
// them the commands from our stepper handler class: a stepper chain and a mock SPI.
// This is very close to the TestEnvironment class from daisy chain tests, except that it directly
// listens to the spi tx flow instead of having us handle the bytes manually.
// This allows us to keep the busy wait in StepMotor::Handler::SendCmd without entering an infinite
// loop.
// Note that because of this busy wait in StepMotor::Handler::SendCmd(), the test commands will
// always be sent one at a time during tests. This is OK since the daisy chain is fully tested
// on its own, and the motors are completely independent from one another.
// Also note that sending commands this way can only work once we have properly set the number of
// slaves in the StepperChain, because of the way the mock spi calls the interrupts.
template <size_t NumMotors>
class TestSteppers : public TxListener {
 public:
  TestSteppers(float low_stops[NumMotors], float high_stops[NumMotors], bool power_step) {
    for (size_t i = 0; i < NumMotors; i++) {
      motors[i].emplace(low_stops[i], high_stops[i], power_step);
    }

    // Set num slaves in the daisy chain to the desired number:
    // first set a probe response accordingly.
    uint8_t probe_response[5] = {0};
    for (size_t i = 0; i < 5; i++) {
      if (i < NumMotors) {
        probe_response[i] = 0;
      } else {
        probe_response[i] = 1;
      }
    }
    // Have the daisy chain parse that probe response
    EXPECT_EQ(chain.ParseProbeResponse(probe_response, sizeof(probe_response)), NumMotors);
    spi.SetTxListener(this);
    spi.EnableInterrupts();

    // Set the flow target to our command array.
    spi.TestSetSpiFlowTarget(command_, NumMotors);
  }

  void on_tx_complete() override {
    for (size_t i = 0; i < NumMotors; i++) {
      response_[i] = motors[i]->GetNextReponseByte();
      motors[i]->ReceiveByte(command_[i]);
    }
    // reset the flow target (from the beginning of our command array)
    spi.TestSetSpiFlowTarget(command_, NumMotors);
    // Queue the response - note that this will trigger RxDMAIntHandler, which will in turn start
    // sending the next command byte (if any) to the mock spi, which will trigger on_tx_complete
    // (this function) and so on, until the full spi request has been sent.
    spi.TestQueueReceiveData(response_, NumMotors);
  }
  void on_tx_error() override {}

 private:
  uint8_t response_[NumMotors] = {0};
  uint8_t command_[NumMotors] = {0};

 public:
  // we keep these public for ease of access.
  std::optional<TestStepper> motors[NumMotors] = {std::nullopt};
  SPI::MockChannel spi;
  Chain chain{"", "", &spi, microseconds(0)};
};

TEST(StepMotor, InitializeSetsPowerStepGateConfig) {
  float low_stop[1] = {-180.0f};
  float high_stop[1] = {180.0f};

  TestSteppers<1> fake_steppers(low_stop, high_stop, /*power_step=*/true);

  Handler handler(0, &fake_steppers.chain);

  handler.Initialize();

  EXPECT_TRUE(handler.power_step());

  TestStepper *motor = &fake_steppers.motors[0].value();

  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::GateConfig1)], 0x0FFD);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::GateConfig2)], 0xF7);
}

TEST(StepMotor, InitializeDoesNotChangeL6470State) {
  float low_stop[1] = {-180.0f};
  float high_stop[1] = {180.0f};

  TestSteppers<1> fake_steppers(low_stop, high_stop, /*power_step=*/false);

  Handler handler(0, &fake_steppers.chain);

  handler.Initialize();

  TestStepper *motor = &fake_steppers.motors[0].value();

  // GateConfig1 is the IC config register on L6470, and Handler::Initialize should not change it.
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::GateConfig1)], 0x2E88);
}

TEST(StepMotor, SetAmpScalesGivenValue) {
  float low_stop[1] = {-180.0f};
  float high_stop[1] = {180.0f};

  TestSteppers<1> fake_steppers(low_stop, high_stop, /*power_step=*/false);

  Handler handler(0, &fake_steppers.chain);

  handler.Initialize();

  EXPECT_EQ(handler.SetAmpHold(1.1f), ErrorCode::BadValue);
  EXPECT_EQ(handler.SetAmpRun(-0.1f), ErrorCode::BadValue);

  EXPECT_EQ(handler.SetAmpAll(0.5f), ErrorCode::Ok);

  TestStepper *motor = &fake_steppers.motors[0].value();
  // 0.5 means half of the 0-255 full scale, which is 127.
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueAccelerate)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueDecelerate)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueHold)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueRun)], 127);

  EXPECT_EQ(handler.SetAmpHold(0.25f), ErrorCode::Ok);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueAccelerate)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueDecelerate)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueHold)], 63);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueRun)], 127);

  EXPECT_EQ(handler.SetAmpRun(0.125f), ErrorCode::Ok);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueAccelerate)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueDecelerate)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueHold)], 63);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueRun)], 31);

  EXPECT_EQ(handler.SetAmpAccel(0.75f), ErrorCode::Ok);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueAccelerate)], 191);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueDecelerate)], 127);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueHold)], 63);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueRun)], 31);

  EXPECT_EQ(handler.SetAmpDecel(1.0f), ErrorCode::Ok);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueAccelerate)], 191);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueDecelerate)], 255);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueHold)], 63);
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::KValueRun)], 31);
}

TEST(StepMotor, Setters) {
  float low_stop[1] = {-180.0f};
  float high_stop[1] = {180.0f};

  TestSteppers<1> fake_steppers(low_stop, high_stop, /*power_step=*/false);

  Handler handler(0, &fake_steppers.chain);

  handler.Initialize();

  TestStepper *motor = &fake_steppers.motors[0].value();

  EXPECT_EQ(handler.SetMaxSpeed(200.0f), ErrorCode::Ok);
  // Datasheet gives max speed resolution of 15.25 steps/sec, which equals 27.45 degrees/s.
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::MaxSpeed)],
            static_cast<uint32_t>(200.0f / 27.45f));

  EXPECT_EQ(handler.SetAccel(360.0f), ErrorCode::Ok);
  // Datasheet gives accel resolution of 14.55 step/s², which equals 26.19 degrees/s²:
  EXPECT_EQ(motor->params_[static_cast<size_t>(Param::Acceleration)],
            static_cast<uint32_t>(360.0f / 26.19f));

  // I don't test the other setters for now because we don't actually use them.
  /// \TODO (low priority) test other setters
}

TEST(StepMotor, MotorMovements) {
  float low_stop[1] = {-180.0f};
  float high_stop[1] = {180.0f};

  TestSteppers<1> fake_steppers(low_stop, high_stop, /*power_step=*/false);

  Handler handler(0, &fake_steppers.chain);

  handler.Initialize();

  TestStepper *motor = &fake_steppers.motors[0].value();

  // define a tolerance of 0.02°
  constexpr float PositionTolerance{0.02f};

  EXPECT_EQ(motor->Position(), 0.0f);
  EXPECT_EQ(handler.GotoPos(30.0f), ErrorCode::Ok);
  EXPECT_NEAR(motor->Position(), 30.0f, PositionTolerance);
  EXPECT_EQ(handler.HardStop(), ErrorCode::Ok);
  EXPECT_EQ(handler.MoveRel(10.0f), ErrorCode::Ok);
  EXPECT_NEAR(motor->Position(), 40.0f, PositionTolerance);
  EXPECT_EQ(handler.HardStop(), ErrorCode::Ok);
  EXPECT_EQ(handler.MoveRel(-80.0f), ErrorCode::Ok);
  EXPECT_NEAR(motor->Position(), -40.0f, PositionTolerance);
  EXPECT_EQ(handler.HardStop(), ErrorCode::Ok);
  EXPECT_EQ(handler.GotoPos(-30.0f), ErrorCode::Ok);
  EXPECT_NEAR(motor->Position(), -30.0f, PositionTolerance);
  // we are out of 0 position test that sending ClearPosition
  // resets current position to 0
  EXPECT_EQ(handler.ClearPosition(), ErrorCode::Ok);
  EXPECT_EQ(motor->Position(), 0.0f);
}

// I don't test the other commands yet because we don't actually use them.
/// \TODO (low priority) design tests for other commands
