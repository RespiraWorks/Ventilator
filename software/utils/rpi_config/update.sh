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
echo "  -- if you have made any local changes, abort take care of those first"
echo "  -- neither unit tests nor static checks will be performed"
echo " "
read -n1 -s -r -p $'Press any key to continue...\n' key

# Print each command as it executes
# TODO only if verbose
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

### Will not switch branch to master!
git pull

### Update desktop shortcuts
yes | cp -rf ./software/utils/rpi_config/Desktop/* /home/respira/Desktop

### Update controller and deploy
./controller/controller.sh install_local
./controller/controller.sh clean
./controller/controller.sh run

### Rebuild GUI
./gui/gui.sh install_local
./gui/gui.sh clean
./gui/gui.sh build --release --no-checks

echo "Installation complete. Please check that this terminated with no errors."
echo " "
read -n1 -s -r -p $'Press any key to close this window.\n' key
