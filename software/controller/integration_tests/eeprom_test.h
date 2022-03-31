// Summary:
//      This test reads the initial state of the EEPROM, writes length bytes
//      (all equal to data) at the provided address in the EEPROM, only to read
//      them again and compare what was written to what was read.
//      It then restores the EEPROM to its initial state after it ran, unless
//      the test failed because of a hardware fault.
//
//      The PCB buzzer starts during the EEPROM access and stops after about 1s
//      if the test succeeds. This means it will keep buzzing if the test fails.
//
// How to run:
//      deploy_test.sh <device_id> eeprom <address> <data> <length>
//      The sum of address and length cannot exceed 32768 because of EEPROM
//      size.
//      length cannot exceed I2C write buffer size - 6 bytes (i.e. 4090).
//      data has to be in the [0 ; 255] range (it is written as bytes).
//
//      example: deploy_test.sh eeprom p7 eeprom 0 42 1000
//          Reads the first 1000 bytes of the EEPROM, replaces them with 42 on
//          the EEPROM, then reads them again, and checks that the second read
//          is equal to what was written.
//          It then writes the first 1000 bytes it initially read back to the
//          EEPROM to put it back in its initial state.
//
//      In case the buzzer doesn't work, or you are working on a remote machine
//      with no way to hear sound, you can check the result manually using the
//      debug interface:
//      $ utils/controller_debug.py
//      ] get result
//      Will return 1 if the result is successful, and 0 otherwise.
//
//      In case of failure, the debug interface provides a way to manually check
//      all steps of the process by reading the actual data that is contained in
//      the eeprom_before, write_data and eeprom_after tables, through a
//      combination of get and peek commands:
//      ] get eeprom_after
//      123456788
//      ] peek 123456788 8
//      0x075BCD14 0x04030201 0x08070605
//      In this example, 123456788 (equal to 0x075BCD14) is the address of the
//      eeprom_after table in the STM32 internal memory, 0x04030201 0x08070605
//      are the contents of this table (which should have been all copies of the
//      same "data" byte, but here contain their address + 1 instead).
//
// Automation:
//      TBD - which python script to run?
//

#include "commands.h"
#include "hal.h"
#include "interface.h"
#include "system_timer.h"
#include "vars.h"

static Debug::Variable::UInt32 dbg_addr_before("eeprom_before", Debug::Variable::Access::ReadOnly,
                                               0, "", "address of eeprom contents at startup");
static Debug::Variable::UInt32 dbg_write_data("write_data", Debug::Variable::Access::ReadOnly, 0,
                                              "", "address of write data");
static Debug::Variable::UInt32 dbg_addr_after("eeprom_after", Debug::Variable::Access::ReadOnly, 0,
                                              "", "address of eeprom contents after write");
static Debug::Variable::UInt32 dbg_test_result("result", Debug::Variable::Access::ReadOnly, 0, "",
                                               "result of the test");

// test parameters
static constexpr uint16_t Address{TEST_PARAM_1};
static constexpr uint8_t Data{TEST_PARAM_2};
static constexpr uint16_t Length{TEST_PARAM_3};

// declaration of EEPROM
static I2Ceeprom eeprom = I2Ceeprom(0x50, 64, 32768, &i2c1);

// Global variables for the debug interface
static Debug::Trace trace;
// Create a handler for each of the known commands that the Debug Handler can
// link to.  This is a bit tedious but I can't find a simpler way.
static Debug::Command::ModeHandler mode_command;
static Debug::Command::PeekHandler peek_command;
static Debug::Command::PokeHandler poke_command;
static Debug::Command::VarHandler var_command;
static Debug::Command::TraceHandler trace_command(&trace);
static Debug::Command::EepromHandler eeprom_command(&eeprom);

static Debug::Interface debug(&trace, 12, Debug::Command::Code::Mode, &mode_command,
                              Debug::Command::Code::Peek, &peek_command, Debug::Command::Code::Poke,
                              &poke_command, Debug::Command::Code::Variable, &var_command,
                              Debug::Command::Code::Trace, &trace_command,
                              Debug::Command::Code::EepromAccess, &eeprom_command);

void RunTest() {
  hal.Init();
  uint8_t eeprom_before[Length];
  uint8_t write_data[Length];
  uint8_t eeprom_after[Length];
  bool second_read_finished{false};

  for (int i = 0; i < Length; ++i) {
    write_data[i] = Data;
  }

  hal.buzzer.set(0.1f);
  eeprom.ReadBytes(Address, Length, &eeprom_before, nullptr);

  eeprom.WriteBytes(Address, Length, &write_data, nullptr);

  eeprom.ReadBytes(Address, Length, &eeprom_after, &second_read_finished);

  dbg_addr_before.set(reinterpret_cast<uint32_t>(&eeprom_before));
  dbg_write_data.set(reinterpret_cast<uint32_t>(&write_data));
  dbg_addr_after.set(reinterpret_cast<uint32_t>(&eeprom_after));

  // stall execution to give time for the actual sending of those requests
  // (through DMA and/or hardware interrupts), with a 500 ms timeout: our I2C
  // bus is 400 kb/s, each of these operations should take less than 100 ms,
  // unless they are too big for our design anyway.
  while (!second_read_finished && SystemTimer::singleton().now() < microsSinceStartup(500000)) {
  };

  bool failed = false;
  for (int i = 0; i < Length; i++) {
    if (eeprom_after[i] != write_data[i]) {
      failed = true;
      break;
    }
  }

  dbg_test_result.set(!failed);

  // Put the memory back in its initial state (note that if the initial read
  // failed, or the write method is broken, this will also fail and the eeprom
  // will end up in an undefined state).
  eeprom.WriteBytes(Address, Length, &eeprom_before, nullptr);

  // loop here to allow the above write to be processed, the buzzer to be
  // stopped after some time, and to process debug interface commands.
  while (true) {
    // stop the buzzer after 1 second if the test is a success
    if (!failed && SystemTimer::singleton().now() > microsSinceStartup(1000000)) {
      hal.buzzer.set(0.0f);
    }

    debug.Poll();
  };
}
