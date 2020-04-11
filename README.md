# VentilatorSoftware

[![Build Status](https://travis-ci.com/RespiraWorks/VentilatorSoftware.svg?branch=master)](https://travis-ci.com/RespiraWorks/VentilatorSoftware)

This is a common repository for all software components of the RespiraWorks open source ventilator project.

## Structure

common - code shared across controller and GUI

controller - arduino/atmega code for hardware controller

gui - graphical interface

## Building

*This section is a work in progress.*

We use [platformio](https://platformio.org/) for building the controller code.  platformio has a CLI and an IDE.  You'll need the platformio CLI in order to build from the command-line, even if you also install the IDE.

 * Install CLI: https://docs.platformio.org/en/latest/core/installation.html#super-quick-mac-linux
 * Install IDE: https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation

Here's a [video introduction](https://www.youtube.com/watch?v=EIkGTwLOD7o) to platformio.
