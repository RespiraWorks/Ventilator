#!/bin/bash

# This script is designed for local unix usage.
# ./gui.sh --help

# Fail if any command fails
set -e
set -o pipefail

# Print each command as it executes
set -o xtrace

# Check if Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Linux'. You have $PLATFORM."
  exit 1
fi

if [ "$EUID" -ne 0 ]; then
  echo "Please run install with root privileges!"
  exit 1
fi

apt-get install -y git

cd "$HOME"

GIT_LFS_SKIP_SMUDGE=1 git clone https://github.com/RespiraWorks/Ventilator.git

Ventilator/software/utils/run_me_first.sh
