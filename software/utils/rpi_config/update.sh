#!/bin/bash

# Fail if any command fails
set -e
set -o pipefail

echo "==============================================================================="
echo "================== RespiraWorks Ventilator update utility ====================="
echo "==============================================================================="
echo " "
echo "  -- this will rebuild and flash controller firmware"
echo "  -- this will rebuild the GUI"
echo "  -- a 'git pull' will be performed, but branch will not be changed"
echo "  -- if you have made any local changes, take care"
echo "  -- neither unit tests nor static checks will be performed"
echo " "
read -n1 -s -r -p $'Press any key to continue...\n' key

# Print each command as it executes
set -o xtrace

# This script should run from repo/software dir
cd "$(dirname "$0")"/../..

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

git pull

./controller/controller.sh --clean
./controller/controller.sh --run

./gui/gui.sh --build --release --no-checks

echo "Installation complete. Please check that this terminated with no errors."
echo " "
read -n1 -s -r -p $'Press any key to close this window.\n' key
