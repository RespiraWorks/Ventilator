# Ventilator Mainboard 

This directory contains files for the Ventilator mainboard design.

![](VentilatorPCB-RevA-Angled-Render.jpg)

The RespiraWorks Ventilator Mainboard PCB is a custom component that integrates the Cycle Controller, UI Computer,
sensors, actuator drives, power supplies, filtering, and protection components.

Due to the coupled nature of the controls, electrical, and mechanical components and their design the RespiraWorks
ventilator contains a custom fabricated printed circuit board (PCB) to connect the microcontroller, interface processor,
and sensor interfaces. The use of a custom PCB greatly reduces the chance for electrical assembly and fabrication errors
and is far more amenable to a quality fabrication process than hand-soldered components. We do not intend for this
component to be hand-assembled, and engineering turnkey fabrication instructions have been generated as part of the
submission; no assembly instructions are provided here.

The original Rev 1 PCB was done in Altium.  As of Rev A, this project has moved to KiCad for better accessibility.

## Rev A

The latest revision of the PCB is Rev A, designed to be used in the top-level design revision Ventilator v0.4.

The main design is in this folder and the project file is here: [Ventilator Mainboard Rev A](Ventilator.pro)

Recently exported files for PCB manufacture can be found on our Google Drive at
[Manufacturing/Output files v0.4/PCB](https://tinyurl.com/2p8af3fm).

This revision integrates many of the modules and daughter-cards in PCB Rev 1.0 into a single PCB, while breaking out the sensors to daughter-cards to make it better adaptable to sensors available in other markets.  It adds some important new features:
 - Moved CycleController from STM32L452 to STM32L496 microcontroller for more I/O pins, memory, and storage.
 - Added an independent FPGA-based supervisor.
 - Higher power capability now up to 120W.
 - Self-powered power loss alarm.
 - Integrated stepper drivers.
 - Ambient pressure sensing for altitude compensation.
 - Real time clock capability.
 - EEPROM for storing calibration data.
 - Additional pressure sensor channels to enable nested flow control.
 - Temperature sensors to monitor component health.
 - Integrated Li-Ion battery backup, charging, and monitoring.
 - Additional power connections for display, fans, and indicator lights.

## Sensor daughter-boards

> TODO: add documentation about these

## Rev 1.0

An archive of the previous version of the PCB, including the Altium-based design project and a snapshot of the old
readme is available on our Google Drive at
[Manufacturing/Output files v0.2-0.3/PCB](https://drive.google.com/drive/folders/1xHPLvo59tGI5uU38FrWMkVGmK_UR2sjQ?usp=sharing)

## Current issues

Issues for PCB work are tracked by tickets with the [PCB label](https://github.com/RespiraWorks/Ventilator/labels/pcb).

Somewhat related are the [Electrical design](https://github.com/RespiraWorks/Ventilator/labels/Electrical) issues.