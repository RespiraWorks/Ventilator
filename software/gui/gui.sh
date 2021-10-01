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
# ./gui.sh --help

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
if [ -n "$VERBOSE" ]; then
  set -o xtrace
fi

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

# Check if Darwin or Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Darwin" ] && [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Darwin' or 'Linux'. You have $PLATFORM."
  exit 1
fi

EXIT_FAILURE=1
EXIT_SUCCESS=0

COVERAGE_INPUT_DIR=build/tests
COVERAGE_OUTPUT_DIR=coverage_reports

#########
# UTILS #
#########

print_help() {
    cat <<EOF
RespiraWorks Ventilator UI build & test utilities.

The following options are available:
  install       Install dependencies for your platform [$PLATFORM]
  clean         Clean build directory and de-initialize git submodules
  build         Build the gui to /build, options:
        [--relase/--debug] - what it says (default=release)
        [-f]               - force run, even with root privileges
        [-j]               - parallel build
        [--no-checks]      - do not run static checks (yes, it's to annoy you!)
  test         Run the unit QTest autotest suite, options:
       [-f]               - force run, even with root privileges
       [-x]               - forwards to Xvfb (for CLI-only testing)
       [--no-cov]         - do not generate coverage reports
  cov_upload   Upload coverage reports to Codecov server
  run          Run the application, forwards app options:
      [-f]               - force run, even with root privileges
      [--startup-only] - just start up momentarily and shutdown
      [--serial-port]  - port for communicating with controller
  cov_upload
  help/-h    Display this dialog
EOF
}

clean_dir() {
  dir_name=$1
  if [ -d "$dir_name" ]; then
    echo "Removing $dir_name"
    rm -rf "$dir_name"
    return $EXIT_SUCCESS
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    return $EXIT_FAILURE
  fi
}

create_clean_directory() {
  dir_name=$1
  clean_dir "$dir_name"
  if mkdir "$dir_name"; then
    echo "Clean directory created: $dir_name"
    return $EXIT_SUCCESS
  else
    echo "Creating directory failed: $dir_name"
    return $EXIT_FAILURE
  fi
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
       "*/tests/*" \
       "*spdlog*" \
       "*fmt*" \
       "/usr/include*"

  mv "$COVERAGE_OUTPUT_DIR/coverage.info" "$COVERAGE_OUTPUT_DIR/coverage_untrimmed.info"
  mv "$COVERAGE_OUTPUT_DIR/coverage_trimmed.info" "$COVERAGE_OUTPUT_DIR/coverage.info"

  genhtml ${QUIET} "$COVERAGE_OUTPUT_DIR/coverage.info" \
      --output-directory "$COVERAGE_OUTPUT_DIR"

  echo "Coverage reports generated at '$COVERAGE_OUTPUT_DIR/index.html'"
  echo "   You may open it in browser with 'python -m webbrowser ${COVERAGE_OUTPUT_DIR}/index.html'"

  #launch_browser
}

upload_coverage_reports() {
  echo "Uploading coverage reports to Codecov"

  curl -Os https://uploader.codecov.io/latest/linux/codecov
  chmod +x codecov
  ./codecov -F gui
  rm codecov
}

launch_browser() {
  python -m webbrowser "${COVERAGE_OUTPUT_DIR}/index.html"
}

install_linux() {
  # Last tuned for Ubuntu 2021.04 Hirsute
  apt-get update
  apt-get install -y \
          git \
          build-essential \
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
          xvfb \
          bear \
          cppcheck \
          lcov \
          clang-tidy
}

configure_conan() {
  conan profile new --detect default
  conan profile update settings.compiler.libcxx=libstdc++11 default
  conan remote add conan-center https://conan.bintray.com False -f
  conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan False -f
}


checks_pre() {
  #Should happen in the build directory
  cppcheck -ithird_party -ibuild .
}

checks_post() {
  #Should happen in the build directory

  cppcheck --project=compile_commands.json \
           -i ../../src/third_party \
           -i ../../../common/third_party \
           .

  CLANG_TIDY_VERSION=$(echo "$(clang-tidy --version | sed -n 2p)" | awk -F[" ".] '{print $5}')
  if [ "$CLANG_TIDY_VERSION" = "6" ]; then
    run-clang-tidy-6.0.py -p .
  else
    eval "run-clang-tidy-${CLANG_TIDY_VERSION}.py"
  fi
}

build_configure() {
  config_type=$1
  will_need_checks=$2

  create_clean_directory build
  git submodule update --init --recursive

  if [ "$will_need_checks" == "yes" ]; then
    checks_pre
  fi

  pushd build
  cmake -DCMAKE_BUILD_TYPE=${config_type} ..
  popd
}

build_with_bear() {
  j_opt=$1

  bear_opt=""
  if [ -n "$VERBOSE" ]; then
    bear --version
    bear --help
    bear_opt="--verbose"
  fi

  pushd build
  bear $bear_opt -- make $j_opt
  checks_post
  popd
}

build_with_make() {
  j_opt=$1

  pushd build
  make $j_opt
  popd
}


########
# HELP #
########

if [ "$1" == "help" ] || [ "$1" == "-h" ]; then
  print_help
  exit $EXIT_SUCCESS

###########
# INSTALL #
###########
elif [ "$1" == "install" ]; then
  if [ "$PLATFORM" == "Darwin" ]; then
    brew install qt5
    configure_conan
    exit $EXIT_SUCCESS
  elif [ "$PLATFORM" == "Linux" ]; then
    if [ "$EUID" -ne 0 ]; then
      echo "Please run install with root privileges!"
      exit $EXIT_FAILURE
    fi
    install_linux
    configure_conan
    exit $EXIT_SUCCESS
  else
    echo "Unsupported platform: ${PLATFORM}"
    exit $EXIT_FAILURE
  fi

#########
# CLEAN #
#########
elif [ "$1" == "clean" ]; then
  clean_dir build
  clean_dir "$COVERAGE_OUTPUT_DIR"
  git submodule deinit -f .
  exit $EXIT_SUCCESS

#########
# BUILD #
#########
elif [ "$1" == "build" ]; then

  #TODO: what if it's one of the later params?
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run build with root privileges!"
    exit $EXIT_FAILURE
  fi

  config_type="Release"
  if [ "$2" == "--debug" ] || [ "$3" == "--debug" ] || [ "$4" == "--debug" ]; then
    config_type="Debug"
  fi

  j_opt=""
  if [ "$2" == "-j" ] || [ "$3" == "-j" ] || [ "$4" == "-j" ]; then
    j_opt="-j3"
  fi

  checks_opt="no"
  if [ "$2" == "--no-checks" ] || [ "$3" == "--no-checks" ] || [ "$4" == "--no-checks" ]; then
    checks_opt="no"
  fi

  build_configure $config_type $checks_opt

  if [ "$checks_opt" == "yes" ]; then
    build_with_bear $j_opt
  else
    build_with_make $j_opt
  fi

  exit $EXIT_SUCCESS

########
# TEST #
########
elif [ "$1" == "test" ]; then

  #TODO: what if it's one of the later params?
  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run tests with root privileges!"
    exit $EXIT_FAILURE
  fi

  git submodule update --init --recursive
  create_clean_directory build

  pushd build
  cmake -DCMAKE_BUILD_TYPE=Debug -DCOV=1 ..

  if [ "$PLATFORM" == "Darwin" ]; then
    make run_tests
  elif [ "$PLATFORM" == "Linux" ]; then
    if [ "$2" == "-x" ] || [ "$3" == "-x" ] || [ "$4" == "-x" ]; then
      Xvfb :1 &
      DISPLAY=:1 make run_tests -j3
    else
      make run_tests -j3
    fi
  fi
  popd

  if [ "$2" != "--no-cov" ] && [ "$3" != "--no-cov" ] && [ "$4" != "--no-cov" ]; then
    generate_coverage_reports
  fi

  exit $EXIT_SUCCESS

###################
# UPLOAD COVERAGE #
###################
elif [ "$1" == "cov_upload" ]; then
  upload_coverage_reports
  exit $EXIT_SUCCESS

#######
# RUN #
#######
elif [ "$1" == "run" ]; then

  if [ "$EUID" -eq 0 ] && [ -z "$FORCED_ROOT" ]; then
    echo "Please do not run the app with root privileges!"
    exit $EXIT_FAILURE
  fi

  pushd build

  if [ "$2" == "-x" ]; then
    Xvfb :1 &
    DISPLAY=:1 ./bin/ventilator_gui_app "${@:3}"
    exit $EXIT_SUCCESS
  else
    ./bin/ventilator_gui_app "${@:2}"
    exit $EXIT_SUCCESS
  fi
  popd

################
# ERROR & HELP #
################
else
  echo No valid options provided :\(
  print_help
  exit $EXIT_FAILURE
fi
