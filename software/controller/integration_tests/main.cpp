// Manually including some headers here in order for ldf to find them:
// see https://community.platformio.org/t/ldf-not-following-environment-variables-in-6-1-0rc1/28616
/// \TODO: remove when (if) the above issue ever gets fixed
#include "actuator_base.h"
#include "hal.h"
#include "interface.h"
#include "sensors.h"

// Using these compile-time defines to avoid multiple directories and mains

#include INTEGRATION_TEST_H

int main() { RunTest(); }
