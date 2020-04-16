# VentilatorSoftware

[![Build Status](https://travis-ci.com/RespiraWorks/VentilatorSoftware.svg?branch=master)](https://travis-ci.com/RespiraWorks/VentilatorSoftware)

This is a common repository for all software components of the [RespiraWorks](http://respira.works) open source ventilator project.

## Structure

common - code shared across controller and GUI

controller - arduino/atmega code for hardware controller

gui - graphical interface

## Building

*This section is a work in progress.*

### Building the controller

We use [platformio](https://platformio.org/) for building the controller code.  platformio has a CLI and an IDE.  You'll need the platformio CLI in order to build from the command-line, even if you also install the IDE.

 * Install CLI: https://docs.platformio.org/en/latest/core/installation.html#super-quick-mac-linux
 * Install IDE: https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation

Here's a [video introduction](https://www.youtube.com/watch?v=EIkGTwLOD7o) to platformio.

After installing platformio, you should be able to build and test as follows.

```
# Build for Arduino.
$ platformio run

# Test on local machine (i.e. your laptop/desktop).
$ platformio test -e native

# If you do this, you'll get an error because it tries to test on all platforms,
and we don't currently support testing on the device.
$ platformio test  # BAD
Error: Please specify `upload_port` for environment or use global `--upload-port` option
```

This has been tested with an RF-nano Arduino board, which has similar parts and
pinout to an Arduino Nano.

## Developing

Please install [pre-commit](http://pre-commit.com) and activate it for this
repository.  We use pre-commit to check for consistent formatting and other
things.

```
# Install pre-commit
$ brew install pre-commit
# OR
$ apt-get install pre-commit

# Then activate pre-commit in this repository.
$ pre-commit install
```
