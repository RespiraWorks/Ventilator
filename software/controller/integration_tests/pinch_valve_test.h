
// Summary:
//      This test repeatedly opens and closes a pinch valve
//
// How to run:
//      TEST=TEST_PINCH_VALVE pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include "hal.h"
#include "pinch_valve.h"
#include "pwm_actuator.h"
#include "spi_stm32.h"
#include "system_constants.h"
#include "system_timer.h"
#include "watchdog.h"

// test parameters
static constexpr size_t MotorIndex{TEST_PARAM_1};
static constexpr Duration Delay{milliseconds(1000)};
static SPI::STM32Channel spi(SPI::Base::SPI1, DMA::Base::DMA2);
static StepMotor::Chain stepper_chain("stepper", "for stepper daisy chain", &spi,
                                      /*min_cs_high_time=*/microseconds(1));
static PinchValve pinch_valve("pinch_valve_", " for pinch valve", MotorIndex, &stepper_chain);

void RunTest() {
  hal.Init(CPUFrequency);

  // Set SPI listeners before its proper initialization to allow Initialize to enable the proper
  // interrupts
  spi.SetListeners(/*rxl=*/&stepper_chain, /*txl=*/nullptr);

  spi.Initialize(/*clock_port=*/GPIO::Port::A, 5, /*miso_port=*/GPIO::Port::A, 6,
                 /*mosi_port=*/GPIO::Port::A, 7, /*chip_select_port=*/GPIO::Port::B, 6,
                 /*reset_port=*/GPIO::Port::A, 9, /*word_size=*/8, SPI::Bitrate::CpuFreqBySixteen);

  stepper_chain.ProbeSlaves(/*null_command=*/static_cast<uint8_t>(StepMotor::OpCode::Nop),
                            /*reset_command=*/static_cast<uint8_t>(StepMotor::OpCode::ResetDevice));

  hal.bind_channels(nullptr, nullptr, nullptr, &spi);

  // Just to shut it up, may not need this beyond v0.3
  PwmActuator blower{BlowerChannel, BlowerFreq, CPUFrequency, "", ""};
  blower.set(0.0f);

  pinch_valve.Initialize();
  pinch_valve.Home();

  bool valve_open{false};
  while (true) {
    pinch_valve.SetOutput(valve_open ? 1.0f : 0.0f);
    SystemTimer::singleton().delay(Delay);

    Watchdog::pet();

    valve_open = !valve_open;
  }
}
