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
# ./common.sh help

# Use this script during development to have a good approximation
# of whether your PR will pass tests on CircleCI:
#
# $ ./common.sh test
#
# CircleCI runs this script (via .circleci/config.yml), but might have some
# environment differences, so the approximation is not perfect. For

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

. ./base.sh

ensure_linux

COVERAGE_ENVIRONMENT=native
COVERAGE_OUTPUT_DIR=coverage_reports

print_help() {
    cat <<EOF
Utility script for the RespiraWorks Ventilator common code testing.

The following options are available:
  install       One-time installation of build toolchain and dependencies
  generate      Generates network protocols via Nanopb and protobuf
  update        Updates platformio and required libraries
  check         Runs static checks only
  clean         Clean build directories
  test          Builds and runs all unit tests, integration tests, static checks, generates coverage
                  [--no-checks] - do not run static checks (for CI)
                  [--cov]       - generate coverage reports
  cov-cleanup   Prepare coverage reports for uploading to server
  unit          Builds and runs unit tests only (and generates coverage reports)
                  <name>  - run specific unit test, may include wildcards, i.e. '*checksum*'
                  [-o]    - open coverage report in browser when done
  help/-h       Display this help info
EOF
}

clean_all() {
  clean_dir .pio
  clean_dir ${COVERAGE_OUTPUT_DIR}
}

update_platformio() {
  pipx install --force platformio=="${PIO_VERSION}"
  pio pkg uninstall -d .
  pio pkg install -d .
}

generate_network_protocols() {
  PROTOCOLS_DIR=generated_libs/protocols
  PYTHON_LIB_PATH=../tools/protocols
  GUI_LIB_PATH=../gui/src/protocols
  NANOPB_PLUGIN=${HOME}/.local/bin/protoc-gen-nanopb

  # ensure paths exist
  mkdir -p "$PYTHON_LIB_PATH"
  mkdir -p "$GUI_LIB_PATH"
  #ensure old files are gone
  rm -f $PROTOCOLS_DIR/*.h $PROTOCOLS_DIR/*.c $GUI_LIB_PATH/*.c* $GUI_LIB_PATH/*.h* $PYTHON_LIB_PATH/*_pb2.py

  protoc \
  --plugin="$NANOPB_PLUGIN" \
  -I $PROTOCOLS_DIR \
  --nanopb_out=$PROTOCOLS_DIR \
  --cpp_out=$GUI_LIB_PATH \
  --python_out=$PYTHON_LIB_PATH \
  network_protocol.proto

  pio pkg install -d .
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

    # Native - cppcheck & clangtidy
    pio check -e native --fail-on-defect=high
}

cleanup_coverage_reports() {
  echo "Cleaning up coverage reports for common code..."

  SRC_DIR=".pio/build/$COVERAGE_ENVIRONMENT"

  # If $COVERAGE_OUTPUT_DIR, assumes it is clean, i.e. with clean_dir
  clean_dir ${COVERAGE_OUTPUT_DIR}/ugly
  clean_dir ${COVERAGE_OUTPUT_DIR}/processed
  mkdir -p "$COVERAGE_OUTPUT_DIR/ugly"
  mkdir -p "$COVERAGE_OUTPUT_DIR/processed"

  find $SRC_DIR -name '*.gcda' -exec cp -t ${COVERAGE_OUTPUT_DIR}/ugly {} +
  find $SRC_DIR -name '*.gcno' -exec cp -t ${COVERAGE_OUTPUT_DIR}/ugly {} +
  find . \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.tpp" \) \
      -not -path '*generated_libs*' \
      -not -path '*cmake*' \
      -exec gcov -pb -o ${COVERAGE_OUTPUT_DIR}/ugly -f {} \;
  mv {*.gcov,.*.gcov} "$COVERAGE_OUTPUT_DIR/processed"
  rm $COVERAGE_OUTPUT_DIR/processed/#usr*
  rm $COVERAGE_OUTPUT_DIR/processed/test*
  rm $COVERAGE_OUTPUT_DIR/processed/.pio*
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
  lcov ${QUIET} --remove "${COVERAGE_OUTPUT_DIR}/coverage.info" \
       --output-file "${COVERAGE_OUTPUT_DIR}/coverage_trimmed.info" \
       --ignore-errors unused \
       "*_test_transport.c" \
       "*/protocols/*" \
       "*output_export.c*" \
       "*test*" \
       "*.pio/libdeps/*" \
       "/usr/include*"

  # Has to be called `coverage.info` for Codecov to pick it up
  rm "${COVERAGE_OUTPUT_DIR}/coverage.info"
  mv "${COVERAGE_OUTPUT_DIR}/coverage_trimmed.info" "${COVERAGE_OUTPUT_DIR}/coverage.info"

  genhtml ${QUIET} "${COVERAGE_OUTPUT_DIR}/coverage.info" \
      --output-directory "${COVERAGE_OUTPUT_DIR}"

  echo "Coverage reports generated at '${COVERAGE_OUTPUT_DIR}/index.html'"
  echo "   You may open it in browser with 'python3 -m webbrowser ${COVERAGE_OUTPUT_DIR}/index.html'"

}

########
# HELP #
########
if [ "$1" == "help" ] || [ "$1" == "-h" ]; then
  print_help
  exit_good

###########
# INSTALL #
###########
elif [ "$1" == "install" ]; then
  ensure_not_root
  install_common_tooling
  exit_good

##########
# UPDATE #
##########
elif [ "$1" == "update" ]; then
  ensure_not_root
  update_platformio
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
  generate_network_protocols
  run_checks
  exit_good

#############
# UNIT TEST #
#############
elif [ "$1" == "unit" ]; then
  clean_all
  generate_network_protocols

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
  generate_network_protocols

  # If we need to generate coverage reports, perform a first build of a single unit test
  # because platformio clears the build dir after the first unit test it builds.
  # We arbitrarily chose binutils.
  if [ "$2" == "--cov" ] || [ "$3" == "--cov" ]; then
    pio test -e native -f test_prototraits
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

##############################
# GENERATE NETWORK PROTOCOLS #
##############################
elif [ "$1" == "generate" ]; then
  generate_network_protocols
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
