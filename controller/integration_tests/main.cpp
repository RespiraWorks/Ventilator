// Using these compile-time defines to avoid multiple directories and mains

#ifdef TEST_BLOWER
#include "blower_test.h"
#elif TEST_SOLENOID
#include "solenoid_test.h"
#elif TEST_STEPPER
#include "stepper_test.h"
#elif TEST_PINCH_VALVE
#include "pinch_valve_test.h"
#elif TEST_BUZZER
#include "buzzer_test.h"
#elif TEST_IDLE
#include "idle_test.h"
#endif

int main() { run_test(); }
