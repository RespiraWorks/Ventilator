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
# ./gui.sh --help

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

. ../common/base.sh

ensure_linux

COVERAGE_INPUT_DIR=build
COVERAGE_OUTPUT_DIR=coverage_reports

print_help() {
    cat <<EOF
RespiraWorks Ventilator UI build & test utilities.

The following options are available:
  install     One-time installation of build toolchain and dependencies
  clean       Clean build directory
  check       Runs static checks only (must build first)
  build       Build the gui to /build, options:
      [--relase/--debug] - what it says (default=release)
      [-j]               - parallel build (auto select max-1 cores)
      [--no-checks]      - do not run static checks (default=on, to annoy you!)
  test        Run unit tests, options:
      [-j]               - parallel build (auto select max-1 cores)
      [-x]               - forwards to Xvfb (for CLI-only testing)
      [--no-cov]         - do not generate coverage reports
      [--upload-cov]     - upload coverage reports to codecov (for CI only)
  run          Run the application, forwards app options:
      [-x]               - forwards to Xvfb (for CLI-only testing)
      [--startup-only]   - just start up momentarily and shutdown
      [--serial-port]    - port for communicating with controller
  help/-h     Display this dialog
EOF
}

install_linux() {
  # Last tuned for Ubuntu 2021.04 Hirsute
  sudo apt install -y \
           cmake \
           qtbase5-dev \
           qtbase5-dev-tools \
           qtmultimedia5-dev \
           qtdeclarative5-dev \
           qtdeclarative5-dev-tools \
           qtquickcontrols2-5-dev \
           libqt5serialport5 \
           libqt5serialport5-dev \
           libqt5multimedia5 \
           libqt5multimedia5-plugins \
           libqt5multimediaquick5 \
           libqt5multimediawidgets5 \
           qml-module-qtcharts \
           qml-module-qtquick-controls \
           qml-module-qtquick-controls2 \
           qml-module-qtmultimedia \
           pulseaudio \
           xvfb
}

configure_conan() {
  pipx ensurepath
  pipx install --force conan=="$CONAN_VERSION"
  source "${HOME}/.profile"
  conan profile detect
}

run_cppcheck() {
  create_clean_dir  build/cppcheck
  cppcheck --enable=all --std=c++17 --inconclusive --force --inline-suppr --quiet \
           --enable=information --check-config \
           -I ../common/generated_libs/protocols \
           -I ../common/libs/units \
           -ibuild -icmake-build-stm32 -isrc/protocols \
           .

#           --project=build/compile_commands.json \
#           --xml --output-file=build/cppcheck/report.xml \
#
#  cppcheck-htmlreport --file=build/cppcheck/report.xml \
#                      --title="Ventilator GUI" \
#                      --report-dir=build/cppcheck --source-dir=.
}

run_clang_tidy() {
  j_opt=$1

  CLANG_TIDY_EXEC=""
  CLANG_TIDY_VERSION=$(echo "$(clang-tidy --version | sed -n 2p)" | awk -F[" ".] '{print $5}')
  if [ "$CLANG_TIDY_VERSION" = "6" ]; then
    CLANG_TIDY_EXEC="run-clang-tidy-6.0.py"
  else
    CLANG_TIDY_EXEC="run-clang-tidy"
  fi
  echo "running $CLANG_TIDY_EXEC"
  find . -name '*.cpp' -not -path "*build*" \
         -exec $CLANG_TIDY_EXEC -quiet $j_opt \
         -header-filter='^.*gui\/(src|app|tests)\/.*\.(hpp|cpp|h)$' \
         -p build {} \;
}

generate_coverage_reports() {
  echo "Generating test coverage reports..."

  QUIET="--quiet"
  if [ -n "$VERBOSE" ]; then
    QUIET=""
  fi

  clean_dir "$COVERAGE_OUTPUT_DIR"
  mkdir -p "$COVERAGE_OUTPUT_DIR"

  lcov ${QUIET} --directory "$COVERAGE_INPUT_DIR" --capture \
       --output-file "$COVERAGE_OUTPUT_DIR/coverage.info"

  lcov ${QUIET} --remove "$COVERAGE_OUTPUT_DIR/coverage.info" \
       --output-file "$COVERAGE_OUTPUT_DIR/coverage_trimmed.info" \
       "*/common/*" \
       "*/ventilator_gui_backend_autogen/*" \
       "*/tests/*" \
       "*spdlog*" \
       "*fmt*" \
       "/usr/include*"

  rm "$COVERAGE_OUTPUT_DIR/coverage.info"
  mv "$COVERAGE_OUTPUT_DIR/coverage_trimmed.info" "$COVERAGE_OUTPUT_DIR/coverage.info"

  genhtml ${QUIET} "$COVERAGE_OUTPUT_DIR/coverage.info" \
      --output-directory "$COVERAGE_OUTPUT_DIR"

  echo "Coverage reports generated at '$COVERAGE_OUTPUT_DIR/index.html'"
  echo "   You may open it in browser with 'python3 -m webbrowser ${COVERAGE_OUTPUT_DIR}/index.html'"

  #launch_browser "${COVERAGE_OUTPUT_DIR}/index.html"
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
  ../common/common.sh install
  if [ "$PLATFORM" == "Darwin" ]; then
    brew install qt5
    configure_conan
    exit_good
  elif [ "$PLATFORM" == "Linux" ]; then
    ensure_not_root
    install_common_tooling
    install_linux
    configure_conan
    exit_good
  else
    echo "Unsupported platform: ${PLATFORM}"
    exit_fail
  fi

#########
# CLEAN #
#########
elif [ "$1" == "clean" ]; then
  clean_dir build
  clean_dir "$COVERAGE_OUTPUT_DIR"
  exit_good

#########
# BUILD #
#########
elif [ "$1" == "build" ]; then

  config_type="Release"
  if [ "$2" == "--debug" ] || [ "$3" == "--debug" ] || [ "$4" == "--debug" ]; then
    config_type="Debug"
  fi

  j_opt=""
  if [ "$2" == "-j" ] || [ "$3" == "-j" ] || [ "$4" == "-j" ]; then
    # build with 1 less than total number of CPUS, minimum 1
    NUM_CPUS=$(cat /proc/cpuinfo | grep -c processor)
    let NUM_CPUS-=1
    if [ "$NUM_CPUS" -lt "1" ]; then
      NUM_CPUS=1
    fi
    echo "Will build with ${NUM_CPUS} parallel jobs"
    j_opt="-j${NUM_CPUS}"
  fi

  checks_opt="yes"
  if [ "$2" == "--no-checks" ] || [ "$3" == "--no-checks" ] || [ "$4" == "--no-checks" ]; then
    checks_opt="no"
  fi

  ensure_not_root

  create_clean_dir build

  # generate comms protocols
  ../common/common.sh update
  ../common/common.sh generate

  pushd build
  if [ "$checks_opt" == "yes" ]; then
    cmake -DCMAKE_BUILD_TYPE=${config_type} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    make everything $j_opt
  else
    cmake -DCMAKE_BUILD_TYPE=${config_type} ..
    make everything $j_opt
  fi
  popd

  if [ "$checks_opt" == "yes" ]; then
    run_clang_tidy $j_opt
    run_cppcheck
  fi

  exit_good

#########
# CHECK #
#########
elif [ "$1" == "check" ]; then
  run_clang_tidy
  run_cppcheck

########
# TEST #
########
elif [ "$1" == "test" ]; then

  j_opt=""
  if [ "$2" == "-j" ] || [ "$3" == "-j" ] || [ "$4" == "-j" ] || [ "$5" == "-j" ]; then
    # build with 1 less than total number of CPUS, minimum 1
    NUM_CPUS=$(cat /proc/cpuinfo | grep -c processor)
    let NUM_CPUS-=1
    if [ "$NUM_CPUS" -lt "1" ]; then
      NUM_CPUS=1
    fi
    echo "Will build tests with ${NUM_CPUS} parallel jobs"
    j_opt="-j${NUM_CPUS}"
  fi

  ensure_not_root

  create_clean_dir build

  # generate comms protocols
  ../common/common.sh update
  ../common/common.sh generate

  pushd build
  cmake -DCMAKE_BUILD_TYPE=Debug -DCOV=1 ..

  if [ "$PLATFORM" == "Darwin" ]; then
    make run_tests
  elif [ "$PLATFORM" == "Linux" ]; then
    if [ "$2" == "-x" ] || [ "$3" == "-x" ] || [ "$4" == "-x" ] || [ "$5" == "-x" ]; then
      xvfb-run make run_tests $j_opt
    else
      make run_tests $j_opt
    fi
  fi
  popd

  if [ "$2" != "--no-cov" ] && [ "$3" != "--no-cov" ] \
   && [ "$4" != "--no-cov" ] && [ "$5" != "--no-cov" ]; then
    generate_coverage_reports
  fi

  exit_good

#######
# RUN #
#######
elif [ "$1" == "run" ]; then
  ensure_not_root

  pushd build
  if [ "$2" == "-x" ] || [ "$3" == "-x" ] || [ "$4" == "-x" ]; then
    xvfb-run ./bin/ventilator_gui_app "${@:3}"
  else
    ./bin/ventilator_gui_app "${@:2}"
  fi
  popd

  exit_good

################
# ERROR & HELP #
################
else
  echo No valid options provided :\(
  print_help
  exit_fail
fi
