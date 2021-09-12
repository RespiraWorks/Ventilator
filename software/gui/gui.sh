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
RespiraWorks VentilatorUI build utilities.
The following options are provided:
  --help      Display this dialog
  --install   Install dependencies for your platform [$PLATFORM]
  --clean     Clean build directory and de-initialize git submodules
  --build     Build the gui to /build, options:
      [--relase/--debug] - what it says
      [-j]               - parallel build
      [--no-checks]      - do not run static checks (yes, it's to annoy you!)
  --test      Run the unit QTest autotest suite, options:
      [-x]               - forwards to Xvfb (for CLI-only testing)
  --run       Run the application, forwards app options:
      [--startup-only] - just start up momentarily and shutdown
      [--serial-port]  - port for communicating with controller
  -f          Forces run, bypassing root privilege check. For CI only. Please don't do this!
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
  create_clean_directory "$COVERAGE_OUTPUT_DIR"

  # the file "output_export.cpp" causes an lcov error,
  # but it doesn't appear to be part of our source, so we're excluding it
  lcov ${QUIET} --directory "$COVERAGE_INPUT_DIR" --capture \
       --output-file "$COVERAGE_OUTPUT_DIR/coverage.info" \
       --exclude "*/common/*" \
       --exclude "*/tests/*" \
       --exclude "*spdlog*" \
       --exclude "/usr/include*"

  genhtml ${QUIET} "$COVERAGE_OUTPUT_DIR/coverage.info" \
      --output-directory "$COVERAGE_OUTPUT_DIR"

  echo "Coverage reports generated at '$COVERAGE_OUTPUT_DIR/index.html'"
  echo "   You may open it in browser with 'python -m webbrowser ${COVERAGE_OUTPUT_DIR}/index.html'"

  #launch_browser
}

upload_coverage_reports() {
  echo "Generating test coverage reports for GUI..."

  # If $COVERAGE_OUTPUT_DIR, assumes it is clean, i.e. with clean_dir
  clean_dir ${COVERAGE_OUTPUT_DIR}/ugly
  clean_dir ${COVERAGE_OUTPUT_DIR}/processed
  mkdir -p "$COVERAGE_OUTPUT_DIR/ugly"
  mkdir -p "$COVERAGE_OUTPUT_DIR/processed"

  find $COVERAGE_INPUT_DIR -name '*.gcda' -exec cp -t ${COVERAGE_OUTPUT_DIR}/ugly {} +
  find $COVERAGE_INPUT_DIR -name '*.gcno' -exec cp -t ${COVERAGE_OUTPUT_DIR}/ugly {} +
  find . \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) \
      -not -path '*third_party*' \
      -exec gcov -pb -o coverage_reports/ugly -f {} \;
  mv *.gcov "$COVERAGE_OUTPUT_DIR/processed"

# weird that all three of these had to be removed above for anything to come out
#      -not -path '*moc*' \
#      -not -path '*qrc*' \
#      -not -path '*test*' \

  rm $COVERAGE_OUTPUT_DIR/processed/#usr*
  rm $COVERAGE_OUTPUT_DIR/processed/*third_party*
  rm $COVERAGE_OUTPUT_DIR/processed/*common*
  rm $COVERAGE_OUTPUT_DIR/processed/moc*
  rm $COVERAGE_OUTPUT_DIR/processed/*tests*

  curl -Os https://uploader.codecov.io/latest/linux/codecov
  chmod +x codecov
  ./codecov -F gui
  rm codecov
}

launch_browser() {
  python -m webbrowser "${COVERAGE_OUTPUT_DIR}/index.html"
}

########
# HELP #
########

if [ "$1" == "--help" ]; then
  print_help
  exit $EXIT_SUCCESS

###########
# INSTALL #
###########
elif [ "$1" == "--install" ]; then
  if [ "$PLATFORM" == "Darwin" ]; then
    brew install qt5
  fi

  if [ "$PLATFORM" == "Linux" ]; then
    if [ "$EUID" -ne 0 ]; then
      echo "Please run install with root privileges!"
      exit 1
    fi

    apt-get update &&
      apt-get install -y \
        build-essential \
        git \
        qt5-default \
        qtbase5-dev-tools \
        qtdeclarative5-dev \
        qtmultimedia5-dev \
        libqt5multimediawidgets5 \
        libqt5multimedia5 \
        libqt5multimedia5-plugins \
        libqt5multimediaquick5 \
        pulseaudio \
        qml-module-qtcharts \
        qtquickcontrols2-5-dev \
        qml-module-qtquick-controls \
        qml-module-qtquick-controls2 \
        qml-module-qtmultimedia \
        libqt5serialport5-dev \
        libqt5serialport5 \
        qtdeclarative5-dev-tools \
        xvfb \
	      bear \
	      cppcheck \
	      lcov \
	      clang-tidy
  fi
  exit $EXIT_SUCCESS

#########
# CLEAN #
#########
elif [ "$1" == "--clean" ]; then
  clean_dir build
  clean_dir "$COVERAGE_OUTPUT_DIR"
  qmake -unset QMAKEFEATURES
  git submodule deinit .
  exit $EXIT_SUCCESS

#########
# BUILD #
#########
elif [ "$1" == "--build" ]; then

  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run build with root privileges!"
    exit $EXIT_FAILURE
  fi

  create_clean_directory build
  qmake -unset QMAKEFEATURES
  git submodule update --init --recursive

  config_opt="CONFIG+=release"
  if [ "$2" == "--debug" ] || [ "$3" == "--debug" ] || [ "$4" == "--debug" ]; then
    config_opt="CONFIG+=debug"
  fi

  j_opt=""
  if [ "$2" == "-j" ] || [ "$3" == "-j" ] || [ "$4" == "-j" ]; then
    j_opt="-j"
  fi

  ## Does this have to happen up here or can it go with the other checks after build?
  if [ "$2" != "--no-checks" ] && [ "$3" != "--no-checks" ] && [ "$4" != "--no-checks" ]; then
    cppcheck -ithird_party -ibuild .
  fi

  pushd build

  qmake $config_opt ..
  bear make $j_opt

  if [ "$2" != "--no-checks" ] && [ "$3" != "--no-checks" ] && [ "$4" != "--no-checks" ]; then
    cppcheck --project=compile_commands.json -i ../../src/third_party -i ../../../common/third_party .

    CLANG_TIDY_VERSION=$(echo "$(clang-tidy --version | sed -n 2p)" | awk -F[" ".] '{print $5}')
    if [ "$CLANG_TIDY_VERSION" = "6" ]; then
      run-clang-tidy-6.0.py -p .
    else
      eval "run-clang-tidy-${CLANG_TIDY_VERSION}.py"
    fi
  fi

  popd
  exit $EXIT_SUCCESS

########
# TEST #
########
elif [ "$1" == "--test" ]; then

  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run tests with root privileges!"
    exit $EXIT_FAILURE
  fi

  pushd build

  if [ "$PLATFORM" == "Darwin" ]; then
    make check
  fi
  if [ "$PLATFORM" == "Linux" ]; then
    if [ "$2" == "-x" ] || [ "$3" == "-x" ]; then
      Xvfb :1 &
      DISPLAY=:1 make check
    else
      make check
    fi
  fi
  popd

  #generate_coverage_reports

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
elif [ "$1" == "--run" ]; then

  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run the app with root privileges!"
    exit $EXIT_FAILURE
  fi

  pushd build/app

  if [ "$PLATFORM" == "Darwin" ]; then
    ./ProjectVentilatorGUI.app/Contents/MacOS/ProjectVentilatorGUI "${@:2}"
    exit $EXIT_SUCCESS
  fi
  if [ "$PLATFORM" == "Linux" ]; then
    ./ProjectVentilatorGUI "${@:2}"
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
