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

# Make sure we are not in sudo
if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
  echo "Please do not run this script with root privileges!"
  exit $EXIT_FAILURE
fi

message="RespiraWorks Ventilator update utility

  -- a 'git pull' will be performed, but branch will not be changed
  -- will rebuild the GUI
  -- will rebuild and flash controller
  -- if you have made any local changes, abort and take care of those first
  -- neither unit tests nor static checks will be performed

PLEASE NOTE THAT AT THE END OF THIS PROCESS YOU SHOULD
GET A CONFIRMATION DIALOG INDICATING SUCCESS

  Do you want to continue?"

if zenity --question --no-wrap --title="Ventilator update" \
          --text "<span size=\"large\">$message.</span>"
then
  echo "Continuing with update..."
else
  exit $EXIT_FAILURE
fi

### Set RW background - must be done in Desktop mode, thus not in boostrap.sh
pcmanfm --set-wallpaper ${HOME}/ventilator/manufacturing/images/rendering_full.jpg

### disable screensaver
#echo "@xset -dpms" >> /home/admin/.config/lxsession/LXDE-pi/autostart
#echo "@xset s off" >> /home/admin/.config/lxsession/LXDE-pi/autostart

# This script should run from repo/software dir
cd "$(dirname "$0")"/../..

### Will not switch branch to master!
git pull

### Update controller and deploy
./controller/controller.sh clean
./controller/controller.sh run

ans=$(zenity --list --radiolist --title "GUI Build" \
       --column "Select" --column "Build type" \
          TRUE "Release" FALSE "Debug")

if [[ $ans = "Release" ]]
then
  echo "Building Release"
  ./gui/gui.sh clean
  ./gui/gui.sh build --release --no-checks;
elif [[ $ans = "Debug" ]]
then
  echo "Building Debug"
  ./gui/gui.sh clean
  ./gui/gui.sh build --debug --no-checks
else
  exit $EXIT_FAILURE
fi

zenity --info --no-wrap --title="Success" --text "Ventilator update executed successfully!"
exit $EXIT_SUCCESS
