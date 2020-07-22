# Ventilator controller software

Here resides the code for the ventilator controller.

## High-level graph of components

<!-- See https://github.com/TLmaK0/gravizo -->
![Graph of components in cycle controller](https://g.gravizo.com/source/cc_graph_mark?https%3A%2F%2Fraw.githubusercontent.com%2FRespiraWorks%2FVentilator%2Fmaster%2Fsoftware%2Fcontroller%2FREADME.md)
<!--
cc_graph_mark
digraph G {
  subgraph cluster_RPI {
    label="GUI (Raspberry Pi)"
    labelloc=b;
    GUI [label="Qt app"]
  }

  subgraph cluster_CC {
    label = "Cycle Controller (STM32)";
    labelloc = b;
    "Main loop" -> "Controller"
    "Main loop" -> "Comms"

    "Controller" -> "Sensors"
    "Controller" -> "Actuators"
    "Controller" -> "Breath FSM"

    "Actuators" -> HAL
    "Sensors" -> HAL
  }

  "Comms" -> GUI
  GUI -> "Comms"
}
cc_graph_mark
-->

## Rationale and structure

The ventilator actuators must be driven by control loops ensuring continuous
breathing according to doctor-provided parameters, while also ensuring
no harm is done to the patient.

Controller code is separated from the user interface code. This makes the code base
smaller, easier to maintain, and lessens the risk of unexpected behavior.

The controller shares the [common communications code](../common) with the GUI.
The part of the code specific to the controller resides here.

**Directories:**
* [lib](lib) - most of the substantive controller code, must all be libraries to be unit-testable by platformio
* [test](test) - unit tests
* [integration_tests](integration_tests) - code for (semi-)automated hardware-in-the-loop testbeds
* [platformio](platformio) - build configurations and deployment scripts
* [src](src) - just the main loop for controller
* [src_test](src_test) - just the main loop for unit tests

**Files:**
* [platfomio.ini](platformio.ini) - the equivalent of a "make file" which governs how platformio builds targets
* [controller_coverage.sh](controller_coverage.sh) - script for generating unit test code coverage reports
* [test.sh](test.sh) - script for running all unit tests
* [upload.sh](upload.sh) - uploads firmware to ventilator controller

## Development toolchain

The target platform for this code is the STM32 processor (currently, the Nucleo L452RE to be precise).

We use [platformio](https://platformio.org/) for building the controller code.
Platformio has a CLI and an IDE. You'll need the platformio CLI in order to build
from the command-line, even if you also install the IDE.

Instructions for installing:
 * [CLI](https://docs.platformio.org/en/latest/core/installation.html#super-quick-mac-linux)
 * [IDE](https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation)

In case of unix systems, the `pip`-provided platformio package tends to be more up to date, so opt for that over `apt`.

Here's a [video introduction](https://www.youtube.com/watch?v=EIkGTwLOD7o) to platformio.

PlatformIO works on Windows as well.
You'll need to install [Atom](https://atom.io/),
[Clang+LLVM](https://releases.llvm.org/download.html),
and [Python](https://www.python.org/downloads/windows/).
(Note: you may be asked to install Python 2.7, but PlatformIO works with Python 3.5+ as well, ostensibly.)

## Building and testing

After installing platformio, you should be able to build and test as follows.

```
$ ./test.sh
# This will run a few commands, such as "platformio run" and
# "platformio test -e native".
```

This is the same script that runs on our continuous integration server (Travis CI).
Run it frequently during development to catch errors/style violations early.

Sometimes PlatformIO can get into a bad state -
e.g. if things don't build for you in `master`, try:


```
$ rm -rf .pio/
```

## Running on the controller

To run this you will need at a Nucleo dev board and/or some version of the PCB.
For various builds of the physical system, please refer to the `system-design`
folder in this repository.

Plug in the STM32 to your machine, then ask platformio to list all devices connected.
You should see a USB serial port corresponding to your device.

```
$ pio device list
/dev/cu.usbserial-14220
-----------------------
Hardware ID: USB VID:PID=1A86:7523 LOCATION=20-2.2
Description: USB2.0-Serial
```

Now you can build and upload to the device.
**Note:** The following command should be run in this directory (same as where `platformnio.ini` lives).

```
$ pio run -t upload
```

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

For more details on this, see the following articles:
[platformio FAQ](https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules)
[community forums](https://community.platformio.org/t/stm32-vs-code-mbed-upload-issue-error-libusb-open-failed-with-libusb-error-access-error-open-failed/10650)

Alternatively, you can upload the firmware.elf and firmware.bin files to the controller mounted as a USB storage device.

## Testing with hardware

Some (semi-)automated integration tests are in the [integration_tests](integration_tests) directory.

Debug interface and manual testing utilities are in the [../utils](../utils) directory.
