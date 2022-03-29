# Deployment on integrated prototypes

The following scripts and configurations are here to help alleviate the frustration of having to perform the many arcane incantations to get a prototype running with software.

**DISCLAIMER: You should not use this on any living beings - as a ventilator, or in any other way. These are utilities for bench testing with mechanical lung simulators only. It is also assumed that you are familiar with the software architecture and generally know what you are doing.**

The following assumes that you have a working hardware prototype with all necessary electronics, correctly wired for power and communication. See the [manufacturing](../../../manufacturing) section for details.

## Initial setup

The first few configuration steps must be done manually. These steps can be done on any Raspberry Pi (not necessarily integrated with the entire hardware setup), and later transfer the correctly configured SD card to a full prototype.

* Start with a clean SD card of at least 32GB. Building the software can be a resource-greedy process.
* Flash the SD card using the [Raspberry Pi Imager](https://www.raspberrypi.com/software/) making sure to configure it as follows:
  * **IMPORTANT:** use the `64-bit Desktop / January 28, 2022` release of the Raspberry Pi OS. Other versions have not been tested to work.
  * hostname (recommended): `rw-ventilator` - could make it something else unique if you have multiple prototypes on your network
  * username (mandatory): `admin` - system should boot to this user by default, otherwise desktop shortcuts may not work
  * password (recommended): `respira` - will need this for `sudo` tasks, so don't forget it
  * enable *ssh* and set up credentials to access the machine remotely, especially if you cannot attach a keyboard to your prototype
  * configure `WiFi` settings if needed
  * set time zone and disable `first run wizard`
* If configured as above, you should be able to *ssh* into the RasPi from another machine by running `ssh admin@rw-ventilator` (or whatever host name you gave it).
* Otherwise, you can find out*RPi*'s IP address with `ifconfig` and then you can reach it as `ssh admin@IP_ADDRESS`.
* If you had logged into another instance with the same hostname, you may need to clear the old credentials with `ssh-keygen -R rw-ventilator.local`.
* Regardless of the approach taken above, you should now be able to run `sudo raspi-config` and apply the following settings:
  * Boot into Desktop with automatic login

## Automatic setup

With the above configured, the next step should be pretty automatic. You can still use a stand-alone RasPi without all the hardware.

Use `ssh` or open up a terminal on the RasPi and just copy and paste this script into a terminal:

<!-- \TODO: change command to point to master before merging!!! -->
```shell
bash <(wget -qO- https://raw.githubusercontent.com/RespiraWorks/Ventilator/issue_1180_cmake_build_on_rpi/software/utils/rpi_config/bootstrap.sh)
```

Follow the onscreen directions. When the initial installation is complete, the system will reboot.

This is now a good time to:
* transfer the SD card to the target machine
* make sure you have a USB connection from the PI to the Nucleo so that it can update controller firmware
* if you have `ssh` access, the rest should work without the need for keyboard and mouse attached to the PI

### Final configuration and initial software deployment

When the machine boots, you should see an icon that says `Ventilator update` on the desktop. Either double-click or use the touch screen to run this shortcut.
This will build the graphical interface and controller software, and deploy the latter to the STM32.

You may need to answer some questions using a keyboard. For your convenience, a virtual `matchbox-keyboard` utility has been installed, which can be found on the main application menu under `Accessories`.

## What you have available

On your desktop you should have:
* Ventilator GUI -- starts the graphical interface for controlling the ventilator
* Github repo -- opens a browser to the RespiraWorks ventilator github page, in case you need to consult documentation
* Ventilator update -- run this any time to update all firmware and software to the latest version
* debug -- opens console with ventilator debug interface and connects to the controller. You should be able to use this utility via `ssh` as well, by running `~/ventilator/software/controller/controller.sh debug`.
