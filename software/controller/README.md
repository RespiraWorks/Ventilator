# Ventilator controller software

Here resides the code for the ventilator controller.

Please also see the [Software design pages](../design/controller_architecture.md) for more information on controller architecture.

## Rationale and structure

The ventilator actuators must be driven by control loops ensuring continuous breathing according to doctor-provided parameters, while also ensuring no harm is done to the patient.

Controller code is separated from the user interface code. This makes the code base
smaller, easier to maintain, and lessens the risk of unexpected behavior.

The controller shares [common code](../common) with the GUI. The part of the code specific to the controller resides here.

**Directories:**
* [lib](lib) - most of the substantive controller code, must all be libraries to be unit-testable by platformio
* [test](test) - unit tests
* [integration_tests](integration_tests) - code for (semi-)automated hardware-in-the-loop testbeds
* [platformio](platformio) - build configurations and deployment scripts
* [src](src) - just the main loop for controller

**Files:**
* [controller.sh](controller.sh) - general utility script for:
  * building controller firmware
  * running unit tests
  * running static checks
  * generating coverage reports locally
  * uploading/deploying firmware
  * shortcut to [debug interface](../utils/debug)
  * self documented if you run it without parameters or with `--help`
* [platfomio.ini](platformio.ini) - the equivalent of a "make file" which governs how platformio builds targets
* [.ycm_extra_conf.py](.ycm_extra_conf.py) - configuration for [YouCompleteMe](https://github.com/ycm-core/YouCompleteMe) (for some IDEs)

## Development toolchain

The target platform for this code is the STM32 processor (currently, the Nucleo L452RE to be precise).

We use [platformio](https://platformio.org/) for building the controller code. Platformio has a CLI and an IDE. You'll need the platformio CLI in order to build from the command-line, even if you also install the IDE.

Instructions for installing:
 * [CLI](https://docs.platformio.org/en/latest/core/installation.html#super-quick-mac-linux)
 * [IDE](https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation)
 * platformio can be used inside CLion, but you may need to run `pio init --ide clion` from its integrated terminal. You should run this in the same directory as `platformio.ini`.

Some issues may prevent specific version sof platformio from building or running unit tests. It is currently recommended that you use `v5.1.1`, which you may install with i.e.:
```
pip install platformio==5.1.1
```

Here's a [video introduction](https://www.youtube.com/watch?v=EIkGTwLOD7o) to platformio.

PlatformIO works on Windows as well. You'll need to install [Atom](https://atom.io/), [Clang+LLVM](https://releases.llvm.org/download.html), and [Python](https://www.python.org/downloads/windows/). (Note: you may be asked to install Python 2.7, but PlatformIO works with Python 3.5+ as well, ostensibly.)

You also need to install the package `libtinfo5` on Linux. Clang-tidy needs this package to run its checks, but platformio will just say all checks have passed without giving an error if it's missing.

## Building and testing

After installing platformio, you should be able to build and run tests as follows:

```
$ ./controller.sh test
```

This is the same script that runs on our continuous integration server (Travis CI).
Run it frequently during development to catch errors/style violations early.

Sometimes PlatformIO can get into a bad state -
e.g. if things don't build for you in `master`, try:


```
$ ./controller.sh clean
```

If you encounter an error such as `Error: Unknown development platform 'native'`, try running

```
$ platformio platform install native
```

## Running on the controller

To run this you will need at a Nucleo dev board and/or some version of the PCB.
For various builds of the physical system, please refer to the [manufacturing](../../manufacturing)
section in this repository.

Plug in the STM32 to your machine, then ask platformio to list all devices connected.
You should see a USB serial port corresponding to your device.

```
$ pio device list
/dev/cu.usbserial-14220
-----------------------
Hardware ID: USB VID:PID=1A86:7523 LOCATION=20-2.2
Description: USB2.0-Serial
```

**TODO:** add this search functionality to script

Now you can build and upload to the device.
**Note:** The following command should be run in this directory (same as where `platformnio.ini` lives).

```
$ ./controller.sh run
```

If you have multiple Nucleos that you want to deploy to, you should consult the [platformio configuration guide](platformio).

**TODO:** make the script do this configuring for you

### USB permission problems

If `pio device list` did not show the Nucleo as, for example, when attempting to deploy directly from the Raspberry Pi, you may have to give yourself rw permission on the USB device.

Find the device ID with `lsusb`. Let us assume in this case, that it shows
`Bus 001 Device 004: ID 0483:374b STMicroelectronics ST-LINK/V2.1`.

Add a udev rule for that device that mounts it with 666 permission. For example, create a file
`/etc/udev/rules.d/99-openocd.rules` with the following line:

```
ATTRS{idVendor}=="0483", ATTRS{idProduct}=="374b", MODE="666"
```

After that, either you unplug and re-plug the USB for STM32 or you restart the pi, you should see something like this:

```
pi@raspberrypi:~ $ pio device list
/dev/ttyACM0
------------
Hardware ID: USB VID:PID=0483:374B SER=0663FF303435554157115746 LOCATION=1-1.4:1.2
Description: STM32 STLink - ST-Link VCP Ctrl
/dev/ttyAMA0
------------
Hardware ID: fe201000.serial
Description: ttyAMA0
```

**TODO:** make the script do this configuring for you

### Other udev problems

If you get the following error, it means platformio was unable to find a connected device.

```
# This means pio couldn't find a device to upload to.  Check that it's connected?
Error: Please specify `upload_port` for environment or use global `--upload-port` option.
```

You may have to modify your `udev` rule to enable flashing of the controller as follows:

```
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
sudo service udev restart
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
```
and then either re-login or restart machine.

**TODO:** make the script do this configuring for you

For more details on this, see the following articles:
[platformio FAQ](https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules)
[community forums](https://community.platformio.org/t/stm32-vs-code-mbed-upload-issue-error-libusb-open-failed-with-libusb-error-access-error-open-failed/10650)

Alternatively, you can upload the firmware.elf and firmware.bin files to the controller mounted as a USB storage device.

## Testing with hardware

Some (semi-)automated integration tests are in the [integration_tests](integration_tests) directory.

Debug interface and manual testing utilities are in the [../utils](../utils) directory.
