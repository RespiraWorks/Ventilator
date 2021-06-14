// Summary:
//      This does nothing. Good to have when testing deployment and maintaining
//      sanity by making them shut up :)
//
// How to run:
//      TEST=TEST_IDLE pio run -e integration-test -t upload
//
// Automation:
//      TBD - which python script to run?
//

#include "hal.h"

// test parameters
static constexpr Duration Delay{milliseconds(10)};

void RunTest() {
  hal.Init();

  while (true) {
    hal.Delay(Delay);
  }
}
