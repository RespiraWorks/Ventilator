#!/bin/bash

# Use this script during development to have a good approximation
# of whether your PR will pass tests on CircleCI:
#
# $ ./test.sh
#
# CircleCI runs this script (via .circleci/config.yml), but might have some
# environment differences, so the approximation is not perfect. For
# a more exact test run, please build the tests_Dockerfile.
#
# Feel free to add more checks here, but keep in mind that:
# - They have to pass on CircleCI
# - They have to have a very good chance of passing for other
#   developers if they run via ./test.sh

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
set -o xtrace

# This script should work no matter where you call it from.
cd "$(dirname "$0")"


#########
# UTILS #
#########

print_help() {
    cat <<EOF
RespiraWorks controller build utilities. Will build and run most tests. Will also build embedded controller code.
The following options are provided:
  --help      Display this dialog
  --no-checks Do not run clangtidy and cppcheck (yes, it's to annoy you!)
EOF
}

########
# HELP #
########

if [ "$1" == "--help" ]; then
  print_help
  exit 0
fi

# Controller unit tests on native.
pio test -e native

# Make sure controller integration tests build for target platform.
# TODO(martukas) This will actually have to be build and tested on Jenkins and not here
INTEGRATION_TEST_H=idle_test.h pio run -e integration-test
INTEGRATION_TEST_H=buzzer_test.h TEST_PARAM_1=0.0f TEST_PARAM_2=1.0f pio run -e integration-test
INTEGRATION_TEST_H=blower_test.h TEST_PARAM_1=0.0f TEST_PARAM_2=1.0f pio run -e integration-test
INTEGRATION_TEST_H=stepper_test.h TEST_PARAM_1=0 TEST_PARAM_2=90.0f pio run -e integration-test
INTEGRATION_TEST_H=pinch_valve_test.h TEST_PARAM_1=0 pio run -e integration-test
INTEGRATION_TEST_H=psol_test.h pio run -e integration-test

# Make sure controller builds for target platform.
pio run -e stm32

if [ "$1" != "--no-checks" ]; then
  # Code style / bug-prone pattern checks (eg. clang-tidy)
  # WARNING: This might sometimes give different results for different people,
  # and different results on CI:
  # See https://community.platformio.org/t/no-version-of-tool-clangtidy-works-on-all-os/13219
  # Feel free to edit .clang_tidy to blacklist problematic checks.

  # Since this is not for CI but just for devs to see, let's maximize the feedback
  # Checks should run on both environments successively regardless if the first one fails
  # It will likely fail for now...
  set +e
  set +o pipefail

  # STM32 - clangtidy [TODO(a-vinod) cppcheck]
  # Native - cppcheck & clangtidy

  pio check -e stm32 --fail-on-defect=high
  pio check -e native --fail-on-defect=high
fi
