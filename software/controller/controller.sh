#!/bin/bash

# Copyright 2020-2022, RespiraWorks
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

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

. ../common/base.sh

ensure_linux

COVERAGE_ENVIRONMENT=native
COVERAGE_OUTPUT_DIR=coverage_reports

print_help() {
    cat <<EOF
Utility script for the RespiraWorks Ventilator controller.

The following options are available:
  configure   One-time configuring of udev rules for deployment to controller
  patch-ocd   One-time patching of OCD script for ST-Link for multi-device deployment environments
  devices     List all devices available for deploying to
  check       Runs static checks only
  clean       Clean build directories
  debug       Run debugger CLI (Python utility) to communicate with controller remotely
  run         Builds and deploys firmware to controller
              There can be only one connected. Otherwise, use the platformio/deploy.sh script manually.
  test        Builds and runs all unit tests, integration tests, static checks, generates coverage
                [--no-checks] - do not run static checks (for CI)
                [--cov]       - generate coverage reports
  unit        Builds and runs unit tests only (and generates coverage reports)
                <name>  - run specific unit test, may include wildcards, i.e. '*debug*'
                [-o]    - open coverage report in browser when done
  integrate   Run integration tests
                all [delay_time] - run all integration tests, pause for [delay_time] in between
                <test_name> [parameters...] - run specific integration test with parameters
  cov-cleanup Prepare coverage reports for uploading to server
  help/-h     Display this help info

Additionally, the following environment variables may be evaluated:
  SN      - serial number or alias of device to deploy firmware or integration tests, as per manifest
  VERBOSE - print all shell commands as they are being executed, pass verbose flags to some scripts
EOF
}

clean_all() {
  clean_dir .pio
  clean_dir ${COVERAGE_OUTPUT_DIR}
}

configure_platformio() {
  echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="666"' | sudo tee /etc/udev/rules.d/99-openocd.rules > /dev/null
  curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
  sudo service udev restart
  sudo usermod -a -G dialout "$USER"
  sudo usermod -a -G plugdev "$USER"

  echo "Updated udev rules. You might have to restart your machine for changes to become effective."
}

patch_ocd_stlink() {
  cp -fr platformio/stlink.cfg "${HOME}/.platformio/packages/tool-openocd/scripts/interface"
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

    # STM32 - cppcheck & clangtidy
    pio check -e stm32 --fail-on-defect=high --skip-packages

    # Native - cppcheck & clangtidy
    pio check -e native --fail-on-defect=high
}

cleanup_coverage_reports() {
  echo "Cleaning up coverage reports for controller code..."

  SRC_DIR=".pio/build/$COVERAGE_ENVIRONMENT"

  # If $COVERAGE_OUTPUT_DIR, assumes it is clean, i.e. with clean_dir
  clean_dir ${COVERAGE_OUTPUT_DIR}/ugly
  clean_dir ${COVERAGE_OUTPUT_DIR}/processed
  mkdir -p "$COVERAGE_OUTPUT_DIR/ugly"
  mkdir -p "$COVERAGE_OUTPUT_DIR/processed"

  find $SRC_DIR -name '*.gcda' -exec cp -t ${COVERAGE_OUTPUT_DIR}/ugly {} +
  find $SRC_DIR -name '*.gcno' -exec cp -t ${COVERAGE_OUTPUT_DIR}/ugly {} +
  find . \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.tpp" \) \
      -not -path '*common*' \
      -not -path '*integration_tests*' \
      -not -path '*cmake*' \
      -exec gcov -pb -o ${COVERAGE_OUTPUT_DIR}/ugly -f {} \;
  mv {*.gcov,.*.gcov} "$COVERAGE_OUTPUT_DIR/processed"
  rm $COVERAGE_OUTPUT_DIR/processed/#usr*
  rm $COVERAGE_OUTPUT_DIR/processed/test*
  rm $COVERAGE_OUTPUT_DIR/processed/.pio*
  rm $COVERAGE_OUTPUT_DIR/processed/*common*
}

generate_coverage_reports() {
  echo "Generating test coverage reports..."

  SRC_DIR=".pio/build/${COVERAGE_ENVIRONMENT}"

  QUIET="--quiet"
  if [ -n "$VERBOSE" ]; then
    QUIET=""
  fi

  clean_dir "$COVERAGE_OUTPUT_DIR"
  mkdir -p "$COVERAGE_OUTPUT_DIR"

  # cannot use --exclude as v1.13 on CI doesn't support that param
  lcov ${QUIET} --directory "$SRC_DIR" --capture \
       --output-file "${COVERAGE_OUTPUT_DIR}/coverage.info" \
       --ignore-errors mismatch

  # the file "output_export.cpp" causes an lcov error,
  # but it doesn't appear to be part of our source, so we're excluding it
  lcov ${QUIET} --remove "${COVERAGE_OUTPUT_DIR}/coverage.info"  \
       --output-file "${COVERAGE_OUTPUT_DIR}/coverage_trimmed.info" \
       --ignore-errors unused \
       "*_test_transport.c" \
       "*/protocols/*" \
       "*output_export.c*" \
       "*common*" \
       "*test*" \
       "*.pio/libdeps/*" \
       "/usr/include*"

  # Has to be called `coverage.info` for Codecov to pick it up
  rm "${COVERAGE_OUTPUT_DIR}/coverage.info"
  mv "${COVERAGE_OUTPUT_DIR}/coverage_trimmed.info" "${COVERAGE_OUTPUT_DIR}/coverage.info"

  genhtml ${QUIET} "${COVERAGE_OUTPUT_DIR}/coverage.info" \
      --output-directory "${COVERAGE_OUTPUT_DIR}"

  echo "Coverage reports generated at '$COVERAGE_OUTPUT_DIR/index.html'"
  echo "   You may open it in browser with 'python -m webbrowser ${COVERAGE_OUTPUT_DIR}/index.html'"

}

function run_debug() {
  ../tools/debug.sh "$@"
}

# prints info from device manifest, if SN is defined in environment
print_device_info() {
  if [ -n "$SN" ]
  then
    echo "SN has been defined in environment, will be deploying to the following device:"
    run_debug -c "device find $SN"
  fi
}

# get_hla_serial <alias>
# prints ST-Link serial number by defined alias
get_hla_serial() {
  device_alias="$1"
  run_debug -c "device find $device_alias h"
}

# build <target_name>
# prints command for building the specified target
build() {
  env_name="$1"

  echo "pio run -e ${env_name}"
}

# deploy <target_name>
# prints command for deploying the specified target
# if SN=alias is defined, prepends ST-Link serial number to deploy to specific device
deploy() {
  env_name="$1"

  if [ -n "$SN" ]
  then
    echo "CUSTOM_HLA_SERIAL=$(get_hla_serial ${SN}) $(build ${env_name}) -t upload"
  else
    echo "$(build ${env_name}) -t upload"
  fi
}

# deploy <test_name> [param1] ... [param5]
# prints environment variable definitions required for generating specific integration test
integration_test() {
  test_name="$1"
  test_param_1="$2"
  test_param_2="$3"
  test_param_3="$4"
  test_param_4="$5"
  test_param_5="$6"

  test_name="${test_name}_test.h"

  echo "INTEGRATION_TEST_H=$test_name TEST_PARAM_1=$test_param_1 TEST_PARAM_2=$test_param_2 \
  TEST_PARAM_3=$test_param_3 TEST_PARAM_4=$test_param_4 TEST_PARAM_5=$test_param_5 "
}

# build_integration_test <test_name> [param1] ... [param5]
# prints command for building specific integration test with desired parameters
build_integration_test() {
  echo "$(integration_test "$@") $(build "integration-test")"
}

# deploy_integration_test <test_name> [param1] ... [param5]
# prints command for deploying specific integration test with desired parameters
# if SN=alias is defined, this will be honored by call to deploy()
deploy_integration_test() {
  echo "$(integration_test "$@") $(deploy "integration-test")"
}

build_all_integration_tests() {
  # Make sure controller integration tests build for target platform.
  eval "$(build_integration_test idle)"
  eval "$(build_integration_test buzzer 0.0f 1.0f)"
  eval "$(build_integration_test blower 0.0f 1.0f)"
  eval "$(build_integration_test stepper 0 90.0f)"
  eval "$(build_integration_test pinch_valve 0)"
  eval "$(build_integration_test psol)"
  eval "$(build_integration_test eeprom 0 85 10)"
}

run_all_integration_tests() {
  wait_time="$1"

  # \todo This is a work in progress, honing in on deploying this in CI

  # This script runs a few of the integration tests in order, with slight pauses in between
  # Ends with putting controller in idle loop.

  eval "$(deploy_integration_test blower 0.0f 1.0f)"
  echo "<<<<< Verify that blower power is ramping up and down >>>>>"
  sleep "$wait_time"

  eval "$(deploy_integration_test buzzer 0.0f 1.0f)"
  echo "<<<<< Verify that buzzer is cycling at a range of volumes >>>>>"
  sleep "$wait_time"

  eval "$(deploy_integration_test pinch_valve 0)"
  echo "<<<<< Verify that pinch valve 0 is cycling >>>>>"
  sleep "$wait_time"

  eval "$(deploy_integration_test pinch_valve 1)"
  echo "<<<<< Verify that pinch valve 1 is cycling >>>>>"
  sleep "$wait_time"

  eval "$(deploy_integration_test eeprom 0 85 10)"
  echo "<<<<< Buzzer should have briefly gone on and off to indicate successful EEPROM test >>>>>"
  sleep "$wait_time"

  eval "$(deploy_integration_test idle)"
}


########
# HELP #
########
if [ "$1" == "help" ] || [ "$1" == "-h" ]; then
  print_help
  exit_good

#############
# CONFIGURE #
#############
elif [ "$1" == "configure" ]; then
  ensure_not_root
  configure_platformio
  exit_good

#########
# CLEAN #
#########
elif [ "$1" == "clean" ]; then
  clean_all
  exit_good

#########
# CHECK #
#########
elif [ "$1" == "check" ]; then
  run_checks
  exit_good

#############
# UNIT TEST #
#############
elif [ "$1" == "unit" ]; then
  clean_all

  # generate comms protocols
  ../common/common.sh generate

  if [ -n "$2" ]; then
    pio test -e native -f "$2"
  else
    pio test -e native
  fi

  generate_coverage_reports

  if [ "$2" == "-o" ] || [ "$3" == "-o" ]; then
    launch_browser "${COVERAGE_OUTPUT_DIR}/index.html"
  fi

  exit_good

############
# TEST ALL #
############
elif [ "$1" == "test" ]; then
  clean_all

  # generate comms protocols
  ../common/common.sh generate

  # Make sure controller builds for target platform
  eval "$(build stm32)"

  # Make sure integration tests build
  build_all_integration_tests

  # Controller unit tests on native
  # This must be the last thing built
  clean_all

  # if we need to generate coverage reports, perform a first build of a single unit test
  # because platformio clears the build dir after the first unit test it builds.
  # We arbitrarily chose pid_lib.
  if [ "$2" == "--cov" ] || [ "$3" == "--cov" ]; then
    pio test -e native -f test_pid_lib
  fi

  pio test -e native

  if [ "$2" == "--cov" ] || [ "$3" == "--cov" ]; then
    generate_coverage_reports
  fi

  if [ "$2" != "--no-checks" ] && [ "$3" != "--no-checks" ]; then
    run_checks
  else
    echo "Skipping static checks."
  fi

  exit_good

####################
# CLEANUP COVERAGE #
####################
elif [ "$1" == "cov-cleanup" ]; then
  cleanup_coverage_reports
  exit_good

#######
# RUN #
#######
elif [ "$1" == "run" ]; then

  ensure_not_root

  # generate comms protocols
  ../common/common.sh generate

  print_device_info
  eval "$(deploy stm32)"

  exit_good

#########
# DEBUG #
#########
elif [ "$1" == "debug" ]; then
  shift
  if [ -n "$SN" ]
  then
    run_debug -d "$SN" "$@"
  else
    run_debug "$@"
  fi
  exit_good

#############
# PATCH OCD #
#############
elif [ "$1" == "patch-ocd" ]; then
  patch_ocd_stlink
  exit_good

#############
# INTEGRATE #
#############
elif [ "$1" == "integrate" ]; then
  print_device_info
  if [ "$2" == "all" ]
  then
   if [ -z "$3" ]; then
     echo "No delay time provided"
     exit_fail
   fi
   run_all_integration_tests "$3"
  else
    deploy_integration_test "${@:2}"
  fi
  exit_good

###########
# DEVICES #
###########
elif [ "$1" == "devices" ]; then
  run_debug -c "device list"
  exit_good

################
# ERROR & HELP #
################
else
  echo "ERROR: Bad command or insufficient parameters!"
  echo
  print_help
  exit_fail
fi
