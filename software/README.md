# Ventilator Software

This directory contains the source code for the ventilator software, test code, sample data and debug utilities.

**DISCLAIMER: You should not use this on any living beings - as a ventilator, or in any other way.
These are utilities for bench testing with mechanical lung simulators only. It is also assumed that you are familiar with the software architecture and generally know what you are doing.**

## Contents

* [bootstrap](bootstrap) - initial deployment scripts for all software directly via the Raspberry pi integrated in a ventilator. Can also be used to configure a linux machine for software development or integration testing.
* [design](design) - documents on software architecture and requirements
* [common](common) - code common to both controller and GUI executables
* [controller](controller) - code for pneumatic system controller (stm32)
* [tools](tools) - controller debug client and other tools for calibration, configuration and testing
* [gui](gui) - code for the ventilator graphical interface (Qt)
