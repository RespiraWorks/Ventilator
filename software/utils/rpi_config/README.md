# Deployment on integrated prototypes

The following scripts and configurations are here to help alleviate the frustration of having to perform the many arcane incantations to get a prototype running with software.

**DISCLAIMER: You should not use this on any living beings - as a ventilator, or in any other way. These are utilities for bench testing with mechanical lung simulators only. It is also assumed that you are familiar with the software architecture and generally know what you are doing.**

The following assumes that you have a working hardware prototype with all necessary electronics, correctly wired for power and communication. See the [manufacturing](../../../manufacturing) section for details.

## Initial OS setup

The first few configuration steps must be done manually when writing the operating system to the SD card.

* Use an SD card of at least 32GB. Building the software can be a resource-greedy process.
* Flash the SD card using the [Raspberry Pi Imager](https://www.raspberrypi.com/software/) making sure to configure it as follows:
  * **IMPORTANT:** Click 'CHOOSE OS' > 'Raspberry Pi OS (Other)' and select the `64-bit Desktop / January 28, 2022` release of the Raspberry Pi OS. Other versions have not been tested to work.
  * Click 'STORAGE' and select your SD card
  * Click the gear icon in the lower right corner, and set up the following parameters:
   * hostname (recommended): `rw-ventilator` - could make it something else unique if you have multiple prototypes on your network
   * username (mandatory): `admin` - system should boot to this user by default, otherwise desktop shortcuts may not work
   * password (recommended): `respira` - will need this for `sudo` tasks, so don't forget it
   * enable *ssh* and set up credentials to access the machine remotely, especially if you cannot attach a keyboard to your prototype
   * configure wifi SSID and password settings if needed
   * set time zone and disable `first run wizard`
   * Click 'SAVE'
   * Click 'WRITE'
* Once the card is writted, remove it and plug it into the RPi, then power it on.
* Once it is booted, you should be able to *ssh* into the RasPi from another machine by running `ssh admin@rw-ventilator` (or whatever host name you gave it). Otherwise, you can find out*RPi*'s IP address with `ifconfig` and then you can reach it as `ssh admin@IP_ADDRESS`. If you had logged into another instance with the same hostname, you may need to clear the old credentials with `ssh-keygen -R rw-ventilator.local`.

## Automatic setup

With the above configured, the next step should be pretty automatic. You can still use a stand-alone RasPi without all the hardware.

Whether by `ssh` or attached keyboard, copy and paste this command into a terminal:

<!-- \TODO: change command to point to master before merging!!! -->
```shell
bash <(wget -qO- https://raw.githubusercontent.com/RespiraWorks/Ventilator/master/software/utils/rpi_config/bootstrap.sh)
```

Follow the onscreen directions. When the initial installation is complete, the system will reboot.

This is a good time to:
* transfer the SD card to the target machine
* make sure you have a USB connection from the PI to the Nucleo so that it can update controller firmware
* if you have `ssh` access, the rest should work without the need for keyboard and mouse attached to the PI

### Final configuration and initial software deployment

When the machine boots, you should see an icon that says `Ventilator update` on the desktop. Either double-click or use the touch screen to run this shortcut.
This will build the graphical interface and controller software, and deploy the latter to the STM32.

**NOTE:** You may also want to check that audio is set to be piped to `HDMI` rather than the `AV Jack` by right clicking on the volume icon in the task bar.

## What you have available

On your desktop you should have:
* Ventilator GUI -- starts the graphical interface for controlling the ventilator
* Github repo -- opens a browser to the RespiraWorks ventilator github page, in case you need to consult documentation
* Ventilator update -- run this any time to update all firmware and software to the latest version
* debug -- opens console with ventilator debug interface and connects to the controller. You should be able to use this utility via `ssh` as well, by running `~/ventilator/software/controller/controller.sh debug`.
* For your convenience, a virtual `matchbox-keyboard` utility has been installed, which can be found on the main application menu under `Accessories`.
