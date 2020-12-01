# Ventilator Mainboard 

This directory contains files for the Ventilator mainboard design.

![](rev1_export/readme_photos/6boards.png)

Due to the coupled nature of the controls, electrical, and mechanical components and their design the RespiraWorks
ventilator contains a custom fabricated printed circuit board (PCB) to connect the microcontroller, interface processor,
and sensor interfaces. The use of a custom PCB greatly reduces the chance for electrical assembly and fabrication errors
and is far more amenable to a quality fabrication process than hand-soldered components. We do not intend for this
component to be hand-assembled, and engineering turnkey fabrication instructions have been generated as part of the
submission; no assembly instructions are provided here.

The RespiraWorks Ventilator Mainboard PCB is a custom component that integrates the Cycle Controller, UI Computer,
sensors, actuator drives, power supplies, filtering, and protection components.

Note that the Rev 1 PCB was done in Alitum.  For the next rev, Rev A, the project is currently moving to KiCad for better accessibility.  This Readme will transition over to supporting the new KiCad-based design as it develops.

## Rev 1.0

The exported complete Rev 1 files for manufacture can be found in the folder [rev1_export](rev1_export).  For support, setup instructions and errata on Rev 1, please refer to [rev 1 readme](rev1_export/README.md).

Instructions for adding stepper motor support to Rev 1.0 board are [here](rev1_export/stepper_driver_setup.md).

The old Altium-based design project, including a snapshot of this readme, can be found in the folder [rev1_export/archived-Altium-Version-RevA-work-in-progress](rev1_export/archived-Altium-Version-RevA-work-in-progress)

## Current issues

Issues for PCB work are tracked by tickets with the [PCB label](https://github.com/RespiraWorks/Ventilator/labels/pcb).

Somewhat related are the [Electrical design](https://github.com/RespiraWorks/Ventilator/labels/Electrical) issues.
  
## Requesting hardware for development

If you are a RespiraWorks contributor, request dev hardware by making an entry
[here](https://docs.google.com/spreadsheets/d/1rf-cOTlQL_LnzrZq-64n7_R_pFyFMdCAxsPC65YqKUg/edit?usp=sharing)
