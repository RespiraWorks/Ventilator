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

# Check if Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Linux'. You have $PLATFORM."
  exit 1
fi

if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
  echo "Please do not run tests with root privileges!"
  exit 1
fi

../../controller/controller.sh --run

../../gui/gui.sh --build --release --no-checks
