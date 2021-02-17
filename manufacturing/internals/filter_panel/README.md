# Air filter panel

| Rendering | Assembled |
:------------------:|:-----------------:|
| ![](images/rendering.jpg)  | ![](images/panel2.jpg)  |

## Overview

The filter panel accommodates four [filter housing assemblies](filter_holder) - two with patient tubing adapters,
and two without. The filter housings are installed on a clear acrylic sheet. In case of different filters being
available and/or required in particular markets or settings, this assembly can be easily redesigned and
swapped out without significantly altering the rest of the ventilator design.

The acrylic sheet is to be mounted to the main equipment bracket using heat inserts and screws.
An explanation for the choice of heat-set inserts is given
[here](../display_panel/README.md#bracket-mounts)

## Custom parts

This is the design for the custom cut acrylic plate for the filter housings.

| Acrylic plate rendering |
:------------------:|
| ![](images/filter_panel_acrylic_plate_rendering.jpg)  |

| Download Type | Link   |
|:--------------|:-------|
| Source: Inventor part file | [.ipt](filter_panel_acrylic_plate.ipt) |
| Drawing                    | [.idw](filter_panel_acrylic_plate.idw) [.pdf](filter_panel_acrylic_plate.pdf)|
| Export for laser cutting   | [.dxf](filter_panel_acrylic_plate.dxf) |

The plate may be produced using a laser cutter or with common woodworking tools.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item | Quantity | Manufacturer  | Part #                         | Price (USD) | Sources[*][ppg]| Notes |
| ---- |---------:| ------------- | ------------------------------ | ------------:|:--------------:|:------|
|**A1**| ~250cm²  | McMaster-Carr | 8560K357                       | 150 / m²     | [C][a1mcmc]    | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A2]` below |
|**A2**| 1        | RespiraWorks  | lower_acrylic_face_panel       |              | [Rw][a2rw]     | Lower acrylic face panel, cut from acrylic `[A1]`  |
|**A3**| 2        | RespiraWorks  | patient_tubing_filter_assembly |              | [Rw][a3rw]     | Filter housing assembly, variant with patient tubing |
|**A4**| 2        | RespiraWorks  | outlet_filter_assembly         |              | [Rw][a4rw]     | Filter housing assembly, variant with outlet vent |
|**A5**| 4        | McMaster-Carr | 94180A351                      | 15.47 / 100  | [C][a5mcmc]    | Heat-set inserts for m4 screws |
|**A6**| 16       | McMaster-Carr | 93625A150                      | 6.04 / 100   | [C][a6mcmc]    | M4 lock nut |
|**A7**| 20       | McMaster-Carr | 93475A230                      | 1.86 / 100   | [C][a7mcmc]    | M4 washer, 9mm OD |
|**A8**| 16       | McMaster-Carr | 92095A193                      | 8.95 / 100   | [C][a8mcmc]    | M4 screw, 14mm |
|**A9**| 4        | McMaster-Carr | 92095A189                      | 8.89 / 100   | [C][a9mcmc]    | M4 screw, 8mm |

[a1mcmc]:  https://www.mcmaster.com/8560K357/
[a2rw]: #custom-parts
[a3rw]: filter_holder
[a4rw]: filter_holder
[a5mcmc]: https://www.mcmaster.com/94180A351/
[a6mcmc]: https://www.mcmaster.com/93625A150/
[a7mcmc]: https://www.mcmaster.com/93475A230/
[a8mcmc]: https://www.mcmaster.com/92095A193/
[a9mcmc]: https://www.mcmaster.com/92095A189/

## Assembly

| Part  | Source |
|:------|:-------|
| Filter panel assembly | [.iam](filter_panel_assembly.iam) |

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

* Install heat set inserts into the corner holes for mounting to main equipment bracket.

* Disassemble the individual holders and integrate the housings with the acrylic panel using M4 hardware.

|                            |
|:--------------------------:|
|![](images/panel1.jpg) |

|                            |
|:--------------------------:|
|![](images/panel2.jpg) |
