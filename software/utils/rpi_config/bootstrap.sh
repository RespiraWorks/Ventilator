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

if [ -z "$VERBOSE" ]; then
  echo "==============================================================================="
  echo "=================== RespiraWorks Ventilator bootstrapper ======================"
  echo "==============================================================================="
  echo " "
  echo "  -- clones the repository"
  echo "  -- configures Raspberry Pi:"
  echo "     * no screensaver or screen blanking"
  echo "     * no splash on bootup"
  echo "     * serial interface enabled for GUI<->controller communications"
  echo "  -- udev rule added for nucleo, enabling flashing of firmware"
  echo "  -- guake console accessible at all times using [F12]"
  echo "  -- desktop shortcuts for ventilator GUI, updater, debug interface"
  echo "  -- RW-theme background"
  echo "  -- installs toolchains and dependencies for building controller and GUI"
  echo "  -- will require a restart when done"
  echo " "
  echo " Will require a restart when complete."
  read -n1 -s -r -p $'Press any key to continue...\n' key
fi

### No screensaver or screen lock
gsettings set org.gnome.desktop.lockdown disable-lock-screen 'true'
gsettings set org.gnome.desktop.screensaver lock-enabled 'false'
gsettings set org.gnome.desktop.screensaver lock-delay 3600
gsettings set org.gnome.desktop.screensaver idle-activation-enabled 'false'
gsettings set org.gnome.desktop.session idle-delay 0

# TODO: Ubuntu MATE desktop and color config
mate-panel --reset --layout netbook
sudo apt install ubuntu-mate-colours-blue
#mate-appearance-properties --install=Ambiant-MATE-Dark-Blue

### Update the system TODO: uncomment
#sudo apt-get update
#sudo apt-get --yes upgrade

### Install guake terminal and git with lfs
sudo apt-get --yes install guake git-lfs

#echo "deb http://archive.raspberrypi.org/debian/ buster main" | sudo tee -a /etc/apt/sources.list
#sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 7FA3303E
#sudo apt-get update
#sudo apt-get --yes install raspi-config

# Ubuntu MATE does not have serial console enabled by default,
# so we do not need to disable it. TODO: test this
# If you are using regular RasPI, you may need to do this:
#sudo raspi-config nonint do_serial 2

### disable screen blanking TODO this don't work
###sudo raspi-config nonint do_blanking 1

### disable splash screen TODO this don't work
#sudo raspi-config nonint do_boot_splash 1

### configure USB permissions to deploy to Nucleo
echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="666"' | sudo tee /etc/udev/rules.d/99-openocd.rules > /dev/null

### Clone repository and go in
git clone https://github.com/RespiraWorks/Ventilator.git ventilator
cd ventilator

###############################################################################
### TODO: Uncomment this section only when tweaking this script in a branch ###
###############################################################################
git checkout issue_1180_cmake_build_on_rpi
git pull
###############################################################################

### guake on startup and settings
mkdir -p /home/respira/.config/autostart
cp ./software/utils/rpi_config/user_config/autostart/* /home/respira/.config/autostart
chmod +x /home/respira/.config/autostart/*
dconf load /apps/guake/ < ./software/utils/rpi_config/user_config/dconf-guake-dump.txt

### Desktop shortcuts
cp ./software/utils/rpi_config/Desktop/* /home/respira/Desktop
chmod +x /home/respira/Desktop/*

### Execute shortcuts without bitching TODO: is this still needed?
#mkdir -p /home/respira/.config/libfm && cp -f software/utils/rpi_config/libfm.conf /home/respira/.config/libfm

### RW theme :)
gsettings set org.mate.background picture-filename /home/respira/ventilator/manufacturing/images/rendering_full.jpg

# Install dependencies and do initial configuration for build toolchains
sudo ./software/gui/gui.sh install
./software/gui/gui.sh install_local
sudo ./software/controller/controller.sh install
./software/controller/controller.sh install_local
sudo ./software/controller/controller.sh configure

if [ -z "$VERBOSE" ]; then
  echo "Installation complete. Please check that this terminated with no errors."
  echo "Upon restart, please run the 'Ventilator update' app from your desktop."
  echo " "
  read -n1 -s -r -p $'Press any key to restart the machine\n' key

  sudo shutdown -r now
fi
