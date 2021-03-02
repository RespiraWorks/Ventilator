#!/bin/bash

# Fail if any command fails
set -e
set -o pipefail

echo "==============================================================================="
echo "=================== RespiraWorks Ventilator bootstrapper ======================"
echo "==============================================================================="
echo " "
echo "  -- clones the repository"
echo "  -- configures Raspberry Pi:"
echo "     * no screensaver or screen blanking"
echo "     * no splash on bootup"
echo "     * serial interface enabled for GUI<->controller comms"
echo "  -- udev rule added for nucleo, enabling flashing of firmware"
echo "  -- guake console accessible at all times using [F12]"
echo "  -- desktop shortcuts for ventilator GUI, updater, debug interface"
echo "  -- RW-theme background"
echo "  -- installs toolchains and dependencies for building controller and GUI"
echo "  -- will require a restart when done"
echo " "
echo " Will require a restart when complete."
read -n1 -s -r -p $'Press any key to continue...\n' key

# Print each command as it executes
set -o xtrace

# Check if Linux
PLATFORM="$(uname -s)"
if [ $PLATFORM != "Linux" ]; then
  echo "Error: This script only supports 'Linux'. You have $PLATFORM."
  exit 1
fi

sudo apt-get --yes install guake git-lfs

### this is for controller debug interface
pip3 install matplotlib pillow

### enable serial interface but not console
sudo raspi-config nonint do_serial 2

### disable screen blanking
sudo raspi-config nonint do_blanking 1

### disable splash screen
sudo raspi-config nonint do_boot_splash 1

### configure USB permissions to deploy to Nucleo
echo 'ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="666"' | sudo tee /etc/udev/rules.d/99-openocd.rules > /dev/null

### Clone repository and go in
git clone https://github.com/RespiraWorks/Ventilator.git
cd Ventilator

### no screensaver
### guake on startup
sudo /bin/cp -f software/utils/rpi_config/autostart /etc/xdg/lxsession/LXDE-pi/autostart

### Desktop shortcuts
cp software/utils/rpi_config/Github /home/pi/Desktop
cp software/utils/rpi_config/*.desktop /home/pi/Desktop

### Execute shortcuts without bitching
mkdir -p /home/pi/.config/libfm && cp -f software/utils/rpi_config/libfm.conf /home/pi/.config/libfm

### RW theme :)
pcmanfm --set-wallpaper /home/pi/Ventilator/manufacturing/images/rendering_full.jpg

sudo ./software/gui/gui.sh --install
./software/controller/controller.sh --install

echo "Installation complete. Please check that this terminated with no errors."
echo "Upon restart, please run the 'Ventilator update' app from your desktop."
echo " "
read -n1 -s -r -p $'Press any key to restart the machine\n' key

sudo shutdown -r now
