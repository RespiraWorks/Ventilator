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

# Print each command as it executes
if [ -n "$VERBOSE" ]; then
  set -o xtrace
fi

EXIT_FAILURE=1
EXIT_SUCCESS=0

# Check if Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Linux'. You have $PLATFORM."
  exit $EXIT_FAILURE
fi

# Make sure it's not sudo
if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
  echo "Please do not run tests with root privileges!"
  exit $EXIT_FAILURE
fi

if [ -z "$VERBOSE" ]; then
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
fi

# This script should run from repo/software dir
cd "$(dirname "$0")"/../..

### Will not switch branch to master!
git pull

### Rebuild GUI
echo "Do you wish to build GUI as debug or release?"
select dr in "Debug" "Release" "Abort"; do
    case $dr in
        Debug ) ./gui/gui.sh clean && ./gui/gui.sh build --debug --no-checks; break;;
        Release ) ./gui/gui.sh clean && ./gui/gui.sh build --release --no-checks; break;;
        Abort ) exit $EXIT_FAILURE;;
    esac
done

### Update controller and deploy
./controller/controller.sh clean
./controller/controller.sh run

if [ -z "$VERBOSE" ]; then
  echo "Installation complete. Please check that this terminated with no errors."
  echo " "
  read -n1 -s -r -p $'Press any key to continue.\n' key
fi
