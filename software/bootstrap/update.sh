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

# This script should work no matter where you call it from.
pushd "$(dirname "$0")" > /dev/null || exit_fail

. ../common/base.sh

ensure_linux
ensure_not_root

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
  exit_fail
fi

# This script should run from repo/software dir
pushd ../.. || exit_fail

### Will not switch branch to master!
git pull

### Set RW background - must be done in Desktop mode, thus not in boostrap.sh
pcmanfm --set-wallpaper "${HOME}/ventilator/software/bootstrap/desktop_background.jpg"

### Update Desktop shortcuts
/bin/cp -rf software/bootstrap/user/Desktop/* "${HOME}/Desktop"

if zenity --question --no-wrap --title="PIO Update" \
          --text "<span size=\"large\">Update PlatformIO and libraries?</span>"
then
  ### Clean and update PIO
  echo "Updating PlatformIO and libraries..."
  ./software/common/common.sh update
  ./software/controller/controller.sh patch-ocd
fi

if zenity --question --no-wrap --title="Controller Build" \
          --text "<span size=\"large\">Build and deploy controller?</span>"
then
  echo "Building and deploying cycle controller..."
  ### Update controller and deploy
  ./software/controller/controller.sh clean
  ./software/controller/controller.sh run
fi

ans=$(zenity --list --radiolist --title "GUI Build" \
       --column "Select" --column "Build type" \
          TRUE "Release" FALSE "Debug")

if [[ $ans = "Release" ]]
then
  echo "Building Release"
  ./software/gui/gui.sh clean
  ./software/gui/gui.sh build --release --no-checks;
elif [[ $ans = "Debug" ]]
then
  echo "Building Debug"
  ./software/gui/gui.sh clean
  ./software/gui/gui.sh build --debug --no-checks
fi

zenity --info --no-wrap --title="Success" --text "Ventilator update executed successfully!"
