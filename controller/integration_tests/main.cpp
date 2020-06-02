#ifdef BLOWER_TEST
#include "blower_test.h"
#elif SOLENOID_TEST
#include "solenoid_test.h"
#elif STEPPER_TEST
#include "stepper_test.h"
#endif

int main() { run_test(); }
