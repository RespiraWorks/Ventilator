// Summary:
//      This test read, writes and reads data again to the EEPROM
//
// How to run:
//      deploy_test.sh <device_id> eeprom <min> <max> <step>
//
// Automation:
//      TBD - which python script to run?
//

#include "debug.h"
#include "eeprom.h"
#include "hal.h"
#include "vars.h"

static DebugUInt32 dbg_addr_before("eeprom_before",
                                   "address of eeprom contents at startup", 0);
static DebugUInt32 dbg_write_data("write_data", "address of write data", 0);
static DebugUInt32 dbg_addr_after("eeprom_after",
                                  "address of eeprom contents after write", 0);
static DebugUInt32 dbg_test_result("result", "result of the test", 0);

// test parameters
static constexpr uint16_t address{TEST_PARAM_1};
static constexpr uint8_t data{TEST_PARAM_2};
static constexpr uint16_t length{TEST_PARAM_3};

void run_test() {
  Hal.init();
  uint8_t eeprom_before[length];
  uint8_t write_data[length];
  uint8_t eeprom_after[length];
  bool processed[4];

  for (int i = 0; i < length; ++i) {
    write_data[i] = data;
  }

  // Read
  eeprom.ReadBytes(address, length, &eeprom_before, &processed[0]);

  eeprom.WriteBytes(address, length, &write_data, &processed[1]);

  eeprom.ReadBytes(address, length, &eeprom_after, &processed[2]);

  Hal.BuzzerOn(0.1f);
  dbg_addr_before.Set(reinterpret_cast<uint32_t>(&eeprom_before));
  dbg_write_data.Set(reinterpret_cast<uint32_t>(&write_data));
  dbg_addr_after.Set(reinterpret_cast<uint32_t>(&eeprom_after));

  while (!processed[2]) {
    asm volatile("nop");
  };

  bool failed = false;
  for (int i = 0; i < length; i++) {
    if (eeprom_after[i] != write_data[i]) {
      failed = true;
    }
  }

  dbg_test_result.Set(!failed);

  if (!failed) {
    Hal.BuzzerOff();
  }

  // put the memory back in its initial state
  eeprom.WriteBytes(address, length, &eeprom_before, &processed[3]);

  // loop here so we are sure to hear the buzzer is still On after this
  while (true) {

    debug.Poll();
  };
}
