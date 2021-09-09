#!/bin/bash

# Copyright 2020-2021, RespiraWorks
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# This script is designed for local unix usage.
# ./controller.sh help

# Use this script during development to have a good approximation
# of whether your PR will pass tests on CircleCI:
#
# $ ./controller.sh test
#
# CircleCI runs this script (via .circleci/config.yml), but might have some
# environment differences, so the approximation is not perfect. For
# a more exact test run, please build the tests_Dockerfile.
#
# Feel free to add more checks here, but keep in mind that:
# - They have to pass on CircleCI
# - They have to have a very good chance of passing for other
#   developers if they run via ./controller.sh test

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
if [ -n "$VERBOSE" ]; then
  set -o xtrace
fi

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

EXIT_FAILURE=1
EXIT_SUCCESS=0

COVERAGE_ENVIRONMENT=native
COVERAGE_OUTPUT_DIR=coverage_reports

# Check if Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Linux'. You have $PLATFORM."
  exit $EXIT_FAILURE
fi

#########
# UTILS #
#########

print_help() {
    cat <<EOF
Utility script for the RespiraWorks Ventilator controller.

The following options are available:
  install   Installs platformio and configures udev rules for deployment
                [-f] - force installation even with root privileges (for CI only)
  check     Runs static checks only
  clean     Clean build directories
  debug     Run debugger CLI (Python utility) to communicate with controller remotely
  run       Builds and deploys firmware to controller
                There can be only one connected. Otherwise, use the platformio/deploy.sh script manually.
  test      Builds and runs all unit tests, integration tests, static checks, generates coverage
                [--no-checks] - do not run static checks (for CI)
                [--no-cov]    - do not generate coverage reports locally (for CI)
  unit      Builds and runs unit tests only (and generates coverage reports)
                <name>  - run specific unit test, may include wildcards, i.e. 'debug*'

  --help/-h Display this help info
EOF
}

clean_dir() {
  dir_name=$1
  if [ -d "$dir_name" ]; then
    echo "Removing $dir_name"
    rm -rf "$dir_name"
    return 0
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    return 1
  fi
}

clean_all() {
  clean_dir .pio
  clean_dir $COVERAGE_OUTPUT_DIR
}

configure_platformio() {
    pushd "$HOME"
    python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
    echo 'export PATH=$PATH:~/.platformio/penv/bin' >> ~/.profile
    popd

    curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
    sudo service udev restart
    sudo usermod -a -G dialout "$USER"
    sudo usermod -a -G plugdev "$USER"

    echo "Updated udev rules. You might have to restart your machine for changes to become effective."

    exit $EXIT_SUCCESS
}

run_checks() {
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

    pio check -e stm32 --fail-on-defect=high --skip-packages
    pio check -e native --fail-on-defect=high
}

run_integration_tests() {
  # Make sure controller integration tests build for target platform.
  # TODO(martukas) This will actually have to build an deploy on Jenkins and not here
  INTEGRATION_TEST_H=idle_test.h pio run -e integration-test
  INTEGRATION_TEST_H=buzzer_test.h TEST_PARAM_1=0.0f TEST_PARAM_2=1.0f pio run -e integration-test
  INTEGRATION_TEST_H=blower_test.h TEST_PARAM_1=0.0f TEST_PARAM_2=1.0f pio run -e integration-test
  INTEGRATION_TEST_H=stepper_test.h TEST_PARAM_1=0 TEST_PARAM_2=90.0f pio run -e integration-test
  INTEGRATION_TEST_H=pinch_valve_test.h TEST_PARAM_1=0 pio run -e integration-test
  INTEGRATION_TEST_H=psol_test.h pio run -e integration-test
  INTEGRATION_TEST_H=eeprom_test.h pio run -e integration-test
}

generate_coverage_reports() {
  SRC_DIR=".pio/build/$COVERAGE_ENVIRONMENT"

  # delete the old report as "safely" as possible
  find "$COVERAGE_OUTPUT_DIR" -name "*.html" -delete || true
  mkdir -p "$COVERAGE_OUTPUT_DIR"
  lcov --directory "$SRC_DIR" --capture \
       --output-file "$COVERAGE_OUTPUT_DIR/$COVERAGE_ENVIRONMENT.info"

  # the file "output_export.cpp" causes an lcov error,
  # but it doesn't appear to be part of our source, so we're excluding it
  lcov -r "$COVERAGE_OUTPUT_DIR/$COVERAGE_ENVIRONMENT.info" \
      --output-file "$COVERAGE_OUTPUT_DIR/${COVERAGE_ENVIRONMENT}_trimmed.info" \
      "*output_export.c*" \
      "*.pio/libdeps/*" \
      "*_test_transport.c" \
      "/usr/include*"
  genhtml "$COVERAGE_OUTPUT_DIR/${COVERAGE_ENVIRONMENT}_trimmed.info" \
      --output-directory "$COVERAGE_OUTPUT_DIR"

  echo "Coverage report generated. Open '$COVERAGE_OUTPUT_DIR/index.html' in a browser to view it."
}

launch_browser() {
  python -m webbrowser ${COVERAGE_OUTPUT_DIR}/index.html
}

########
# HELP #
########

if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
  print_help
  exit $EXIT_SUCCESS

###########
# INSTALL #
###########
elif [ "$1" == "install" ]; then
  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run install with root privileges!"
    exit $EXIT_SUCCESS
  fi

  configure_platformio

#########
# CLEAN #
#########
elif [ "$1" == "clean" ]; then
  clean_all
  exit $EXIT_SUCCESS

#########
# CHECK #
#########
elif [ "$1" == "check" ]; then
  run_checks
  exit $EXIT_SUCCESS

#############
# UNIT TEST #
#############
elif [ "$1" == "unit" ]; then
  clean_all

  if [ -n "$2" ]; then
    pio test -e native -f "$2"
  else
    pio test -e native
  fi

  generate_coverage_reports

  exit $EXIT_SUCCESS

############
# TEST ALL #
############
elif [ "$1" == "test" ]; then
  clean_all

  # Make sure controller builds for target platform
  pio run -e stm32

  # Make sure integration tests build
  run_integration_tests

  # Controller unit tests on native
  # This must be the last thing built
  clean_all
  pio test -e native

  if [ "$2" != "--no-checks" ] && [ "$3" != "--no-checks" ]; then
    run_checks
  else
    echo "Skipping static checks."
  fi

  if [ "$2" != "--no-cov" ] && [ "$3" != "--no-cov" ]; then
    generate_coverage_reports
  else
    echo "Skipping coverage reports."
  fi

  exit $EXIT_SUCCESS

#######
# RUN #
#######
elif [ "$1" == "run" ]; then

  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run the app with root privileges!"
    exit $EXIT_FAILURE
  fi

  platformio/deploy.sh stm32

  exit $EXIT_SUCCESS

#########
# DEBUG #
#########
elif [ "$1" == "debug" ]; then

  shift
  ../utils/debug/debug_cli.py "$@"

  exit $EXIT_SUCCESS

################
# ERROR & HELP #
################
else
  echo "ERROR: Bad command or insufficient parameters!"
  echo
  print_help
  exit $EXIT_FAILURE
fi
