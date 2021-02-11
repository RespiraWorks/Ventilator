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

# Check if Darwin or Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Darwin' or 'Linux'. You have $PLATFORM."
  exit 1
fi

if [ "$EUID" -ne 0 ]; then
  echo "Please run install with root privileges!"
  exit 1
fi

apt-get update
apt-get uprade -y

apt-get install -y guake

### enable serial interface but not console
raspi-config nonint do_serial 2

### configure USB permissions to deploy to Nucleo
echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="666"' > /etc/udev/rules.d/99-openocd.rules

### No screensaver, but guake
/bin/cp -f autostart /etc/xdg/lxsession/LXDE-pi/autostart

### RW theme :)
pcmanfm --set-wallpaper /home/pi/Ventilator/manufacturing/images/rendering_full.jpg

### Desktop shortcuts
cp ./Github "$HOME"/Desktop
cp ./*.desktop "$HOME"/Desktop

### Execute desktop shortcuts without bitching
sed 's/quick_exec=0/quick_exec=1/' "$HOME"/.config/libfm/libfm.conf

../../controller/controller.sh --install

../../gui/gui.sh --install

shutdown -r now
