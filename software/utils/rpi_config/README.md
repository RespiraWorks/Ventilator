# Automatic deployment on integrated prototypes

The following scripts and configurations are here to help alleviate the frustration of having to perform the many arcane incantations to get a prototype running with software.

**DISCLAIMER: You should not use this on any living beings - as a ventilator, or in any other way. These are utilities for bench with mechanical lung simulators only. It is also assumed that you are familiar with the software architecture and generally know what you are doing.**

The following assumes that you have a working hardware prototype with all necessary electronics, correctly wired for power and communication. See the [manufacturing](../../../manufacturing) section for details.

## How to set up

* Start with a clean SD card of at least 32GB. Building the software can be a resource-greedy process.
* Prepare an SD card with a [Ubuntu MATE](https://ubuntu-mate.org/download/arm64/focal/) distribution. Scripts have been tested with the `20.04.3 Focal Fossa`.
* Make sure you have a USB connection from Raspi to Nucleo if you intend to flash firmware locally
* Boot up and go through the standard Raspi configuration steps - locale, Wifi, etc..
  * set up your username to be`respira`
  * allow it to boot without login (recommended)
* Reboot

Now, open up a browser, and come back to this page.

Just copy and paste this script into a terminal:

# TODO: change command to point to master

```
bash <(wget -qO- https://raw.githubusercontent.com/RespiraWorks/Ventilator/issue_1180_cmake_build_on_rpi/software/utils/rpi_config/bootstrap.sh)
```

Follow the onscreen directions.

When the initial install is complete, the system will reboot again.

You will need to run the "Ventilator update" app that you should find on your desktop for the first time to make the
ventilator functional.

## What you have available

On your desktop you should have:
* Ventilator update -- run this any time to update all firmware and software to the latest version
* Ventilator GUI -- graphical interface for controlling the ventilator
* Vent debug -- opens console with ventilator debug interface and connects to the controller
* Ventilator repo -- opens a browser to the RespiraWorks ventilator github page
