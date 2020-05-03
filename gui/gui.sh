#!/bin/bash

# This script is designed for local unix usage.
# ./gui.sh --build

set -e
set -o pipefail
set -o xtrace
cd "$(dirname "$0")"

#########
# UTILS #
#########

create_clean_directory(){
  dir_name=$1
  if [ -d "$dir_name" ]; then
    echo "Removing $dir_name"
    rm -rf "$dir_name"
  elif [ -f "$dir_name" ]; then
    echo "File with this name already exists, not a directory."
    exit
  fi
  if mkdir "$dir_name"; then
    echo "Clean directory created: $dir_name"
    return 0
  else
    echo "Creating directory failed: $dir_name"
    return 1
  fi
}


#########
# SETUP #
#########

# Check if Darwin or Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Darwin" ] && [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Darwin' or 'Linux'. You passed $PLATFORM."
  exit 1
fi

########
# HELP #
########

if [ "$1" == "--help" ] ; then
cat << EOF
--help      Display this dialog
--install   Install dependencies for your platform [$PLATFORM]
--build     Build the gui to /build
--test      Run the unit QTest autotest suite
EOF
fi

###########
# INSTALL #
###########

if [ "$1" == "--install" ] ; then
  if [ "$PLATFORM" == "Darwin" ]; then
    brew install qt5
  fi

  if [ "$PLATFORM" == "Linux" ]; then
    apt-get update && \
    apt-get install -y \
    build-essential \
    qt5-default \
    qtbase5-dev-tools \
    qtdeclarative5-dev \
    qtmultimedia5-dev \
    libqt5charts5 \
    libqt5charts5-dev \
    qml-module-qtcharts \
    libqt5multimedia5-plugins \
    qtquickcontrols2-5-dev \
    qml-module-qtquick-controls \
    qml-module-qtquick-controls2 \
    qtdeclarative5-dev-tools \
    xvfb
  fi
fi

#########
# BUILD #
#########

if [ "$1" == "--build" ] ; then
  create_clean_directory build
  qmake -unset QMAKEFEATURES
  cd build && qmake .. && make -j && cd -
fi

########
# TEST #
########

if [ "$1" == "--test" ] ; then
  create_clean_directory build_autotests
  cd build_autotests && qmake ../autotests && make -j && cd -

  if [ "$PLATFORM" == "Darwin" ]; then
    ./build_autotests/autotests.app/Contents/MacOS/autotests
  fi
  if [ "$PLATFORM" == "Linux" ]; then
    Xvfb :1 &
    DISPLAY=:1 ./build_autotests/autotests
  fi
fi
