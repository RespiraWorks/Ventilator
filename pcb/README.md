# Ventilator Mainboard 

This directory contains files for the Ventilator mainboard design.

This includes the original Altium files, exported files of latest release for manufacture, and instructions for setting
up the board and optional stepper drivers.

![picture of board](rev1_export/readme_photos/IMG_9306.jpg)

## Contents

The following are Altium files for various aspects of the circuit design.

* PrjPcb - the top level project
* SchDoc - schematic pages
* PcbDoc - PCB physical design
* OutJob - automated job for creating manufacturing package
* Net - netlist, logical electrical connections

**TODO:** identify each file individually.

## Rev 1.0

Exported files for the most recent release of the board and set-up instructions are provided [here](rev1_export).

Instructions for adding stepper motor support to Rev 1.0 board are [here](rev1_export/stepper_driver_setup.md).


## Current issues

Issues for PCB work are tracked by tickets with the [PCB label](https://github.com/RespiraWorks/Ventilator/labels/pcb).

Somewhat related are the [Electrical design](https://github.com/RespiraWorks/Ventilator/labels/Electrical) issues.

### Important Rev 1.0 PCB Errata

Here are problems discovered with the latest release of the PCB that need solving for the next iteration.

* [#739](https://github.com/RespiraWorks/Ventilator/issues/739): DNP resistors populated on board, typing 3.3V to the 5V
  rail (bad) - if you have SN 01, 02, 03, or 05 - don't continue using the board without executing the fix in this issue
  ticket.  See ticket for rework instructions and photos.  All other serial number boards are shipping with this issue
  fixed.
* [#738](https://github.com/RespiraWorks/Ventilator/issues/738): Yellow and Green alarm LEDs don't work due to a pin use
  conflict on the nucleo
* [#745](https://github.com/RespiraWorks/Ventilator/issues/745): Blower command is 5V but nucleo outputs 3.3V.  All
  boards are currently shipping with a level translator fitted which addresses this problem, however the blower will
  turn on full blast briefly at reset ([#740](https://github.com/RespiraWorks/Ventilator/issues/740) - to be fixed in
  next rev)
* [#736](https://github.com/RespiraWorks/Ventilator/issues/736) I2C sensor connectors labeled A, C, B, D. Should be A,
  B, C, D :facepalm:  To be clear, what this means is, the I2C multiplexer assignments, as labeled on the board in Rev
  1.0 are: MUX0: SENSOR A; MUX1: SENSOR C; MUX2: SENSOR B, MUX3: SENSOR D
* [#846](https://github.com/RespiraWorks/Ventilator/issues/846): anti-miller caps C33 and C34 filter out PWM signal and
  prevent proper operation.  The Miller effect is only present at startup and doesn't cause a problem with permanently
  connected loads like the PSOL.  Remove C33 and C34.
  
## Requesting hardware for development

If you are a RespiraWorks contributor, request dev hardware by making an entry
[here](https://docs.google.com/spreadsheets/d/1rf-cOTlQL_LnzrZq-64n7_R_pFyFMdCAxsPC65YqKUg/edit?usp=sharing)
