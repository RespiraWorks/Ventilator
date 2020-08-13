#!/bin/bash

# This script is designed for local unix usage.
# ./gui.sh --help

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
set -o xtrace

# This script should work no matter where you call it from.
cd "$(dirname "$0")"

# Check if Darwin or Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Darwin" ] && [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Darwin' or 'Linux'. You passed $PLATFORM."
  exit 1
fi


#########
# UTILS #
#########

print_help() {
    cat <<EOF
RespiraWorks VentilatorUI build utilities.
The following options are provided:
  --help      Display this dialog
  --install   Install dependencies for your platform [$PLATFORM]
  --clean     Clean build directory and deinitialize git submodules
  --build     Build the gui to /build, options:
      [--relase/--debug] - what it says
      [-j]               - parallel build
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
    return 0
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    return 1
  fi
}

create_clean_directory() {
  dir_name=$1
  clean_dir "$dir_name"
  if mkdir "$dir_name"; then
    echo "Clean directory created: $dir_name"
    return 0
  else
    echo "Creating directory failed: $dir_name"
    return 1
  fi
}

########
# HELP #
########

if [ "$1" == "--help" ]; then
  print_help
fi

###########
# INSTALL #
###########

if [ "$1" == "--install" ]; then
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
        xvfb
  fi
  exit 0
fi

#########
# CLEAN #
#########

if [ "$1" == "--clean" ]; then
  clean_dir build
  qmake -unset QMAKEFEATURES
  git submodule deinit .
  exit 0
fi

#########
# BUILD #
#########

if [ "$1" == "--build" ]; then

  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run build with root privileges!"
    exit 1
  fi

  create_clean_directory build
  qmake -unset QMAKEFEATURES
  git submodule update --init --recursive

  config_opt="CONFIG+=release"
  if [ "$2" == "--debug" ] || [ "$3" == "--debug" ]; then
    config_opt="CONFIG+=debug"
  fi

  j_opt=""
  if [ "$2" == "-j" ] || [ "$3" == "-j" ]; then
    j_opt="-j"
  fi

  pushd build && qmake $config_opt .. && make $j_opt && popd

  exit 0
fi


########
# TEST #
########

if [ "$1" == "--test" ]; then

  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run tests with root privileges!"
    exit 1
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
  exit 0
fi


#######
# RUN #
#######

if [ "$1" == "--run" ]; then

  if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
    echo "Please do not run the app with root privileges!"
    exit 1
  fi

  pushd build/app

  if [ "$PLATFORM" == "Darwin" ]; then
    ./ProjectVentilatorGUI.app/Contents/MacOS/ProjectVentilatorGUI "${@:2}"
    exit 0
  fi
  if [ "$PLATFORM" == "Linux" ]; then
    ./ProjectVentilatorGUI "${@:2}"
    exit 0
  fi
  popd
fi

echo No valid options provided :\(
print_help
exit 1
