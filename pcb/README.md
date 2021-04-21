# Ventilator Mainboard 

This directory contains files for the Ventilator mainboard design.

![](VentilatorPCB-RevA-Angled-Render.png)

Due to the coupled nature of the controls, electrical, and mechanical components and their design the RespiraWorks
ventilator contains a custom fabricated printed circuit board (PCB) to connect the microcontroller, interface processor,
and sensor interfaces. The use of a custom PCB greatly reduces the chance for electrical assembly and fabrication errors
and is far more amenable to a quality fabrication process than hand-soldered components. We do not intend for this
component to be hand-assembled, and engineering turnkey fabrication instructions have been generated as part of the
submission; no assembly instructions are provided here.

The RespiraWorks Ventilator Mainboard PCB is a custom component that integrates the Cycle Controller, UI Computer,
sensors, actuator drives, power supplies, filtering, and protection components.

The original Rev 1 PCB was done in Alitum.  As of Rev A, this project has moved to KiCad for better accessibility.

## Rev A

The latest revision of the PCB is Rev A, designed to be used in the top-level design revision Ventilator v0.4.

The main design is in this folder and the project file is here: [Ventilator Mainboard Rev A](Ventilator.pro)

The manufacturing packages can all be found here [Ventilator Mainboard Rev A Manufacturing Files](/manufacturing)

This revision integrates many of the modules and daughtercards in PCB Rev 1.0 into a single PCB, while breaking out the sensors to daughtercards to make it better adaptable to sensors available in other markets.  It adds some important new features:
 - Moved CycleController from STM32L452 to STM32L496 microcontroller for greater memory and storage.
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
 
## Rev 1.0

The exported complete Rev 1 files for manufacture can be found in the folder [rev1_export](rev1_export).

The old Altium-based design project, including a snapshot of this readme, can be found in the folder [rev1_export/archived-Altium-Version-RevA-work-in-progress](rev1_export/archived-Altium-Version-RevA-work-in-progress)

## Current issues

Issues for PCB work are tracked by tickets with the [PCB label](https://github.com/RespiraWorks/Ventilator/labels/pcb).

Somewhat related are the [Electrical design](https://github.com/RespiraWorks/Ventilator/labels/Electrical) issues.