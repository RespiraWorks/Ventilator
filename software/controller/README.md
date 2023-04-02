# Ventilator controller software

Here resides the code for the ventilator controller.

Please also see the [Software design pages](../design/controller_architecture.md) for more information on controller architecture.

If you are not actively working on the controller code but simply need it for prototype testing, you will probably want to use the [automated deployment scripts](../utils/rpi_config).

## Rationale and structure

The ventilator actuators must be driven by control loops ensuring continuous breathing according to doctor-provided parameters, while also ensuring no harm is done to the patient.

Controller code is separated from the user interface code. This makes the code base smaller, easier to maintain, and lessens the risk of unexpected behavior.

The controller shares the [common code](../common) with the GUI. The part of the code specific to the controller resides here.

**Directories:**
* [lib](lib) - most of the substantive controller code, must all be libraries to be unit-testable by platformio
* [test](test) - unit tests
* [test_libs](test_libs) - libraries intended to be used only for unit tests (include mock libraries)
* [integration_tests](integration_tests) - code for (semi-)automated hardware-in-the-loop testbeds
* [platformio](platformio) - build configurations and deployment scripts
* [src](src) - just the main loop for controller

**Files:**
* [controller.sh](controller.sh) - general utility script for:
  * installation of dependencies
  * configuration of host system for deployment
  * building and uploading/deployment of controller firmware
  * running unit tests, static checks, integration tests
  * generating test coverage reports locally
  * shortcut to [debug interface](../utils/debug)
  * self-documented if you run it without parameters or with `--help`
* [platfomio.ini](platformio.ini) - the equivalent of a "make file" which governs how platformio builds targets
* [.ycm_extra_conf.py](.ycm_extra_conf.py) - configuration for [YouCompleteMe](https://github.com/ycm-core/YouCompleteMe) (for some IDEs)

## Quick start

For most everyday development, testing and deployment, the self-documented [controller.sh](controller.sh) script should be used. Scripts are geared to a linux distro that uses the `apt` package manager. You can start with:

* `./controller.sh install` - install all `apt` and `pip` packages needed for building and testing
* `./controller.sh config` - configure ports and permissions for deploying to device, may need a machine restart

Then you can start using the `run`, `test`, `unit` and `integrate` commands as needed. See `--help` for more details.

Below is info on more advanced usage, instructions for development with IDE's, advice for troubleshooting some common problems, some under-the-hood details. The dependencies are maintained in the script, and not all of them will be called out in the document. Please read the script source for the most reliable information.

## Development toolchain

The target platform for this code is the STM32 processor:
* currently, the code supports Nucleo L452RE used with v0.3 prototypes
* new [PCB](../../pcb) integrates STM32L496, to which we will soon transition

We use [platformio](https://platformio.org/) for building the controller code. Platformio has a CLI and an IDE. You'll need the platformio CLI in order to build from the command-line, even if you also install the IDE.

Instructions for installing:
 * [Platformio Core (CLI)](https://docs.platformio.org/en/latest/core/index.html)
 * [Platformio IDE](https://docs.platformio.org/en/latest/integration/ide/pioide.html)
 * [CLion](https://docs.platformio.org/en/latest/integration/ide/clion.html)

Note: To use with CLion, it seems to work best if you run `pio project init --ide clion` in this directory before loading the CMake files.

Some issues may prevent specific versions of platformio from building or running unit tests. It is currently recommended that you use `v6.1.6` (latest version at the time of this writing).

## Building and testing

After installing platformio, you should be able to build and run test as follows:

```shell
./controller.sh test
```

This is the same script that runs on our continuous integration server (Travis CI) and will also perform static checks with `cppcheck` and `clang-tidy`.
Run it frequently during development to catch errors/style violations early.

Sometimes PlatformIO can get into a bad state - e.g. if things don't build for you in `master`, try:

```shell
./controller.sh clean
```

If you encounter an error such as `Error: Unknown development platform 'native'`, try running

```shell
platformio platform install native
```

## Deploying to controller

To run this you will need at least a Nucleo dev board or some version of the PCB. For various builds of the physical system, please refer to the [manufacturing](../../manufacturing) section in this repository.

Plug in the STM32 to your machine, then ask platformio to list all devices connected. You should see a USB serial port corresponding to your device.

```
$ ./controller.sh devices
           /dev/ttyACM1     066EFF343334434257145023              Unregistered device
```

If this is the only device connected, you can deploy with
```shell
./controller.sh run
```

If you have multiple devices connected to your development machine, you should:
* run `./controller.sh patch_ocd` to patch ST-Link scripts
* add your devices to manifest spreadsheet on Google Drive in [this directory](https://drive.google.com/drive/folders/1EBQReayx557hWeHTPxkbUI4eeUq-1AWw?usp=sharing) (you will need team write access).
* run `./controller.sh debug -c "device update"` which will fetch you a copy of that manifest

Now the output for the `controller.sh devices` command should look something like this:
```
 p2                          066DFF303435554157105522 pizza1       Pizza #2 on the road
 e2         /dev/ttyACM1     066EFF343334434257145023 enclosed3    Enclosed #2 on the West Coast
 n6         /dev/ttyACM0     066CFF554857868367094021 v0.4         v0.4 prototype PCB#6 in Oakland
```

Any devices that are in the manifest but not connected to this system will be greyed out. The bottom 2 should be green and are the ones that are detected on the `/dev/tty*` ports. You can now deploy to them by using the alias in the first column, for example:

```shell
SN="e2" ./controller.sh run
```

For more details on how this works under the hood see the [platformio configuration guide](platformio).

## Integration tests

Integration tests are pared-down versions of the controller code that contain less logic and test only specific sub-systems in isolation. These tests can be deployed using some of the following commands:

```shell
# Run all integration tests sequentially, pausing for 10 seconds between tests
./controller.sh integrate all 10
# Make controller idle, i.e. all actuators in off/resting state
./controller.sh integrate idle
# Cycle buzzer between off and full-volume state
./controller.sh integrate buzzer 0.0f 1.0f
```

For more information, see the integration test code, which will also explain what parameters these deployment commands take. Integration tests are in the [integration_tests](integration_tests) directory.

## Debug interface

The interactive command-line debug interface can also be accessed from here, also forwarding any parameters you would pass, for example:

```shell
# Will start the debug interface in interactive mode
./controller.sh debug
# Will start the debug interface in interactive mode and connect to device with alias "e2"
./controller.sh debug -d e2
# Will connect to device with alias "e2" and fetch out all setting values, then exit
./controller.sh debug -d e2 -c "get set"
```

If you have the environment variable `SN` set to a registered device alias, then the script will pass that along to the `-d` argument for connecting. This then makes it possible for you to have local terminal sessions dedicated to deploying to and testing different devices without having to type out the alias every single time.

For more info about the debug interface and other manual testing utilities, see the [../utils](../utils) directory.

## Further details

Our automation scripts may not be up-to-date. If you encounter problems deploying to or connecting via debug interface, see the most up-to-date information here:
* [platformio FAQ](https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules)
* [platformio community forums](https://community.platformio.org/t/stm32-vs-code-mbed-upload-issue-error-libusb-open-failed-with-libusb-error-access-error-open-failed/10650)
