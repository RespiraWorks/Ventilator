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

# Check if Linux
PLATFORM="$(uname -s)"
if [ "$PLATFORM" != "Linux" ]; then
  echo "Error: This script only supports 'Linux'. You have $PLATFORM."
  exit $EXIT_FAILURE
fi

# Make sure we are not in sudo
if [ "$EUID" -eq 0 ] && [ "$2" != "-f" ]; then
  echo "Please do not run this script with root privileges!"
  exit $EXIT_FAILURE
fi

if [ -z "$VERBOSE" ]; then
  echo "==============================================================================="
  echo "=================== RespiraWorks Ventilator bootstrapper ======================"
  echo "==============================================================================="
  echo " "
  echo " MAKE SURE YOU HAVE FOLLOWED ONLINE INSTRUCTIONS TO SET UP YOUR RASPBERRY PI"
  echo " "
  echo "  -- clones the repository"
  echo "  -- desktop shortcuts for ventilator GUI, updater, debug interface"
  echo "  -- installs toolchains and dependencies for building controller and GUI"
  echo "  -- configures device rules for stm32, enabling flashing of firmware"
  echo "  -- will require a restart when done"
  echo " "
  echo " PLEASE DO NOT RUN THIS ON YOUR DEVELOPMENT PC."
  echo " THIS WILL MESS WITH YOUR SYSTEM CONFIGURATION."
  echo " THIS IS FOR RASPBERRY-PI ONLY!"
  echo " "
  read -n1 -s -r -p $'Press any key to continue...\n' _
fi

### Install git-lfs and update the system
sudo apt-get update
sudo apt-get --yes install git-lfs matchbox-keyboard
sudo apt-get --yes upgrade
sudo apt-get --yes autoremove
sudo apt-get autoclean
git config --global pull.ff only

### RasPi configuration
sudo raspi-config nonint do_serial      2  # enable serial interface but not console
sudo raspi-config nonint do_blanking    1  # disable screen blanking
sudo raspi-config nonint do_boot_splash 1  # disable splash screen

### Clone repository without LFS and go in
cd "${HOME}"
GIT_LFS_SKIP_SMUDGE=1 git clone https://github.com/RespiraWorks/Ventilator.git ventilator
cd ventilator

###############################################################################
### TODO: Uncomment this section only when tweaking this script in a branch ###
###############################################################################
#git checkout issue_1277_deploy_with_graphics
###############################################################################
### TODO: Comment out above before merging to master!!!                     ###
###############################################################################

### Bring in software-related graphics only from LFS
git lfs pull -I "software/**"

### Desktop shortcuts
/bin/cp -rf software/bootstrap/user/Desktop/* "${HOME}/Desktop"

### On-screen keyboard, in case its needed
sudo cp -f software/bootstrap/keyboard.xml /usr/share/matchbox-keyboard/

### Execute desktop shortcuts without bitching
mkdir -p "${HOME}/.config/libfm" && cp -f software/bootstrap/user/.config/libfm.conf "${HOME}/.config/libfm"

# Install dependencies and do initial configuration for build toolchains
./software/gui/gui.sh install
./software/common/common.sh install
./software/controller/controller.sh configure

if [ -z "$VERBOSE" ]; then
  echo "Installation complete. Please check that this terminated with no errors."
  echo "Upon restart, please run the 'Update' app from your desktop to complete deployment."
  echo " "
  read -n1 -s -r -p $'Press any key to restart the machine\n' _

  sudo shutdown -r now
fi