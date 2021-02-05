# Air Filter Panel

| Assembly CAD Rendering | Assembled View |
:------------------:|:-----------------:|
| ![](images/rendering.jpg)  | ![](images/panel2.jpg)  |

|Download Type|Link|
|:------|:-------|
|Filter Panel Assembly CAD|[.iam](CAD/filter_panel_assembly.iam)|

## Overview

This readme contains the details, BOM, assembly instructions for one display panel assembly for the enclosed build.

## Design Rationale

The filter panel accommodates four filter assemblies. You can read more about those in detail [here](filter_holder/README.md). The filter assembly houses these by means of a clear acrylic sheet. An acrylic sheet was chosen because it is cheap and easy to customize. The acrylic sheet is mounted to the enclosure main panel using heat inserts and screws. An explanation for the choice of heat-set inserts is given [here](../display_panel/README.md#Display-Panel-mounting-choice) 

## Custom Parts

In addition to the individual filter assemblies themselves (which have several custom parts), the other custom part in this assembly is the custom cut filter acrylic plate. For our build, we used *insert type of acrylic here* and used *insert laser cutting machine specs here* to cut the acrylic. 

| Filter panel acrylic plate CAD rendering |
:------------------:|
| ![](images/filter_panel_acrylic_plate_rendering.jpg)  | 

The necessary files are presented below:
|Download Type|Link|
|:------|:-------|
|Filter Panel Acrylic Plate CAD|[.ipt](CAD/filter_panel_acrylic_plate.ipt)|
|Filter Panel Acrylic Plate drawing|[.idw](CAD/filter_panel_acrylic_plate.idw)|
|Filter Panel Acrylic Plate drawing|[.pdf](CAD/filter_panel_acrylic_plate.pdf)|
|Filter Panel Acrylic Plate for laser cutting|[.dxf](CAD/filter_panel_acrylic_plate.dxf)| 

 

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item | Quantity  | Manufacturer  | Part #                              | Price (USD)         | Sources[*][ppg]| Notes |
| ---- |----------:| ------------- | ----------------------------------- | -------------------:|:----------:|:------|
|**A1**| 1         | RespiraWorks  | lower_acrylic_face_panel            |                     | [Rw][a1rw]  | Lower acrylic face panel |
|**A2**| 2         | RespiraWorks  | patient_tubing_filter_assembly      |                     | [Rw][a2rw]  | Filter housing assembly, variant with patient tubing |
|**A3**| 2         | RespiraWorks  | outlet_filter_assembly              |                     | [Rw][a3rw]  | Filter housing assembly, variant with outlet vent |
|**A4**| 4         | McMaster-Carr | 94180A351                           | 15.47 / 100         | [C][a4mcmc] | Heat-set inserts for m4 screws |
|**A5**| 16        | McMaster-Carr | 93625A150                           | 6.04 / 100          | [C][a5mcmc] | M4 lock nut |
|**A6**| 16        | McMaster-Carr | 93475A230                           | 1.86 / 100          | [C][a6mcmc] | M4 washer |
|**A7**| 16        | McMaster-Carr | 92000A224                           | 9.60 / 100          | [C][a7mcmc] | M4 14mm screw, phillips drive |
|**A8**| (16)      | McMaster-Carr | 92095A193                           | 8.95 / 100          | [C][a8mcmc] | M4 14mm screw, hex drive, **alternate to A7** |

[a1rw]: #lower-face-plate
[a2rw]: filter_holder
[a3rw]: filter_holder
[a4mcmc]: https://www.mcmaster.com/94180A351/
[a5mcmc]: https://www.mcmaster.com/93625A150/
[a6mcmc]: https://www.mcmaster.com/93475A230/
[a7mcmc]: https://www.mcmaster.com/92000A224/
[a8mcmc]: https://www.mcmaster.com/92095A193/

## Assembly Instructions

Disassemble the individual holders and integrate the housings with the acrylic panel. For latest enclosed build, this requires M4x14mm screws (assuming 1/4" or 6.5mm acrylic panel).  
For instructions on how to install heat-set inserts, see similar instructions for the display panel [here](../display_panel/README.md#Heat-Set-Inserts-Installation)

|                            |
|:--------------------------:|
|![](images/panel1.jpg) |

|                            |
|:--------------------------:|
|![](images/panel2.jpg) |
