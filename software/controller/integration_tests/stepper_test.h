
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
#include "spi_stm32.h"
#include "stepper.h"
#include "system_constants.h"
#include "system_timer.h"
#include "watchdog.h"

// test parameters
static constexpr size_t MotorIndex{TEST_PARAM_1};
static constexpr float StepDegrees{TEST_PARAM_2};
static constexpr Duration Delay{milliseconds(1000)};

static SPI::STM32Channel spi(SPI::Base::SPI1, DMA::Base::DMA2);
static StepMotor::Chain stepper_chain("stepper", "for stepper daisy chain", &spi,
                                      /*min_cs_high_time=*/microseconds(1));
static StepMotor::Handler stepper(MotorIndex, &stepper_chain);

void RunTest() {
  hal.Init(CPUFrequency);
  SystemTimer::singleton().initialize(PeripheralID::Timer6, InterruptVector::Timer6, CPUFrequency);

  spi.Initialize(/*clock_port=*/GPIO::Port::A, 5, /*miso_port=*/GPIO::Port::A, 6,
                 /*mosi_port=*/GPIO::Port::A, 7, /*chip_select_port=*/GPIO::Port::B, 6,
                 /*reset_port=*/GPIO::Port::A, 9, /*word_size=*/8, SPI::Bitrate::CpuFreqBySixteen);

  stepper_chain.ProbeSlaves(/*null_command=*/static_cast<uint8_t>(StepMotor::OpCode::Nop),
                            /*reset_command=*/static_cast<uint8_t>(StepMotor::OpCode::ResetDevice));

  hal.bind_channels(nullptr, nullptr, nullptr, &spi);

  Interrupts::singleton().EnableInterrupts();

  // Just to shut it up, may not need this beyond v0.3
  PwmActuator blower{BlowerChannel, BlowerFreq, CPUFrequency, "", ""};
  blower.set(0.0f);

  stepper.Initialize();

  // Configure stepper
  stepper.SetAmpAll(0.1f);
  stepper.SetMaxSpeed(100.0f);
  stepper.SetAccel(100.0f / 0.1f);
  stepper.ClearPosition();

  while (true) {
    stepper.MoveRel(StepDegrees);
    SystemTimer::singleton().delay(Delay);

    Watchdog::pet();
  }
}
