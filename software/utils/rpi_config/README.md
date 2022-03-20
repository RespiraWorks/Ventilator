# Deployment on integrated prototypes

The following scripts and configurations are here to help alleviate the frustration of having to perform the many arcane incantations to get a prototype running with software.

**DISCLAIMER: You should not use this on any living beings - as a ventilator, or in any other way. These are utilities for bench testing with mechanical lung simulators only. It is also assumed that you are familiar with the software architecture and generally know what you are doing.**

The following assumes that you have a working hardware prototype with all necessary electronics, correctly wired for power and communication. See the [manufacturing](../../../manufacturing) section for details.

## Initial setup

The first few configuration steps must be done manually. These steps can be done on any Raspberry Pi (not necessarily integrated with the entire hardware setup), and later transfer the correctly configured SD card.

* Start with a clean SD card of at least 32GB. Building the software can be a resource-greedy process.
* Flash the SD card with [Raspberry Pi OS](https://www.raspberrypi.com/software/operating-systems/). This setup has most recently been tested with `Raspberry Pi OS (64-bit) / January 28, 2022` release.
* If you do have a keyboard and mouse attached, boot on the Raspberry Pi directly and configure it as follows:
  * username (mandatory): `admin` - system should boot to this user by default, otherwise shortcuts may not work
  * password (recommended): `respira` - will need this for `sudo` tasks, so don't forget it
  * hostname (recommended): `rw-ventilator` - could make it something else unique if you have multiple prototypes on your network
  * enable *ssh* and set up credentials to access the machine remotely
* If you cannot attach keyboard and mouse to your Raspberry Pi, you can configure the machine remotely via *ssh* as described our guide [here](headless_pi.md).
* To find out the *RPi*'s IP address, run `ifconfig`. If configured as above, you should be able to ssh into the *RPi* from another machine by running `ssh admin@IP_ADDRESS`.
* Regardless of the approach taken above, you should now run `sudo raspi-config` and apply the following settings:
  * splash screen: `off`
  * screen blanking: `off`
  * Serial: `shell off`, `interface on`
  * Locale: `[your locale]`
  * Time zone: `[your time zone]`
  * Boot into Desktop with automatic login

## Automatic setup

With the above configured, the next step should be pretty automatic. You can still use a stand-alone Raspberry Pi without all the hardware.

Use `ssh` or open up a terminal on the Raspberry Pi and just copy and paste this script into a terminal:

```shell
# \TODO: change command to point to master before merging!!!

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

## What you have available

On your desktop you should have:
* Ventilator GUI -- starts the graphical interface for controlling the ventilator
* Github repo -- opens a browser to the RespiraWorks ventilator github page, in case you need to consult documentation
* Ventilator update -- run this any time to update all firmware and software to the latest version
* debug -- opens console with ventilator debug interface and connects to the controller. You should be able to use this utility via `ssh` as well, by running `~/ventilator/software/controller/controller.sh debug`.
