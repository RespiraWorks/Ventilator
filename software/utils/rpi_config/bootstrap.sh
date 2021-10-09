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

if [ -z "$VERBOSE" ]; then
  echo "==============================================================================="
  echo "=================== RespiraWorks Ventilator bootstrapper ======================"
  echo "==============================================================================="
  echo " "
  echo "  -- clones the repository"
  echo "  -- configures Raspberry Pi:"
  echo "     * no screensaver or screen blanking"
  echo "     * GUI<->controller serial connection enabled but no terminal"
  echo "     * serial interface enabled for GUI<->controller communications"
  echo "  -- udev rule added for nucleo, enabling flashing of firmware"
  echo "  -- guake console accessible at all times using [F12]"
  echo "  -- desktop shortcuts for ventilator GUI, updater, debug interface"
  echo "  -- RW-theme background"
  echo "  -- installs toolchains and dependencies for building controller and GUI"
  echo "  -- will require a restart when done"
  echo " "
  echo " PLEASE DO NOT RUN THIS ON YOUR DEVELOPMENT PC."
  echo " THIS WILL MESS WITH YOUR BOOTLOADER."
  echo " THIS IS FOR RASPBERRY-PI ONLY!"
  read -n1 -s -r -p $'Press any key to continue...\n' key
fi

### No screensaver or screen lock
gsettings set org.mate.power-manager sleep-display-ac 0
gsettings set org.mate.screensaver lock-enabled 'false'
gsettings set org.mate.screensaver idle-activation-enabled 'false'
gsettings set org.mate.screensaver lock-delay 3600
gsettings set org.mate.screensaver cycle-delay 240

mate-panel --reset --layout netbook
sudo add-apt-repository --yes ppa:lah7/ubuntu-mate-colours
sudo apt-get update
sudo apt-get --yes install ubuntu-mate-colours-blue guake git-lfs
# \TODO: apply Ambiant-MATE-Dark-Blue theme?
sudo apt-get --yes remove libreoffice*

### Update the system
sudo apt-get update
sudo apt-get --yes upgrade
sudo apt-get --yes autoremove
sudo apt-get autoclean

# Enable UART, but disable serial terminal
echo "enable_uart=1" | sudo tee -a /boot/firmware/usercfg.txt
sudo sed -i -e 's/console=tty1 //g' /boot/firmware/cmdline.txt

### TODO: Disable boot splash or make custom splash with RW logo?
#https://ubuntu-mate.community/t/disable-splash-screen-on-bootup/3375

### Clone repository and go in
cd ${HOME}
git clone https://github.com/RespiraWorks/Ventilator.git ventilator
cd ventilator

###############################################################################
### TODO: Uncomment this section only when tweaking this script in a branch ###
###############################################################################
git checkout issue_1180_cmake_build_on_rpi
git pull
###############################################################################
### TODO: Comment out above before merging to master!!!                     ###
###############################################################################

### Dekstop shortcuts, guake on startup and guake settings

sudo cp ./software/utils/icons/* /usr/share/pixmaps

cp -rf ./software/utils/rpi_config/user/. ${HOME}
dconf load /apps/guake/ < ./software/utils/rpi_config/dconf-guake-dump.txt

### RW theme :)
gsettings set org.mate.background picture-filename ${HOME}/ventilator/manufacturing/images/rendering_full.jpg

# Install dependencies and do initial configuration for build toolchains
sudo ./software/gui/gui.sh install
./software/gui/gui.sh install_local
sudo ./software/controller/controller.sh install
./software/controller/controller.sh install_local
sudo ./software/controller/controller.sh configure

sudo gpasswd --add ${USER} dialout

### configure USB permissions to deploy to Nucleo
echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="666"' | sudo tee /etc/udev/rules.d/99-openocd.rules > /dev/null

if [ -z "$VERBOSE" ]; then
  echo "Installation complete. Please check that this terminated with no errors."
  echo "Upon restart, please run the 'Ventilator update' app from your desktop."
  echo " "
  read -n1 -s -r -p $'Press any key to restart the machine\n' key

  sudo shutdown -r now
fi
