# Ventilator Software

This directory contains the source code for the ventilator software, test code, sample data and debug utilities.

**DISCLAIMER: You should not use this on any living beings - as a ventilator, or in any other way.
These are utilities for bench testing with mechanical lung simulators only. It is also assumed that you are familiar with the software architecture and generally know what you are doing.**

## Contents

* [design](design) - documents on software architecture and requirements
* [common](common) - code common to both controller and GUI executables
* [controller](controller) - code for pneumatic system controller (stm32)
* [gui](gui) - code for the ventilator graphical interface (Qt)
* [utils](utils) - scripts for debugging controller and mocking communications interface

## Quick start

If a proper enclosed unit is built as described under [manufacturing](../manufacturing), it should be possible to deploy all GUI and controller software directly via the Raspberry pi that is integrated into the ventilator unit.

Scripts for doing so are available under [utils/rpi_config](utils/rpi_config).
