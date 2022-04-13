# Air filter panel

|         Rendering          |       Assembled        |
|:--------------------------:|:----------------------:|
| ![](images/rendering.jpg)  | ![](images/panel2.jpg) |

## Overview

The filter panel accommodates four [filter housing assemblies](filter_holder) - two with patient tubing adapters,
and two without. The filter housings are installed on a clear acrylic sheet. In case of different filters being
available and/or required in particular markets or settings, this assembly can be easily redesigned and
swapped out without significantly altering the rest of the ventilator design.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item   |  Quantity | Manufacturer  | Part #                                | Price (USD) | Sources[*][ppg] | Notes                                                          |
|--------|----------:|---------------|---------------------------------------|------------:|:---------------:|:---------------------------------------------------------------|
| **A1** | (~250cm²) | McMaster-Carr | 8560K357                              |    150 / m² |   [C][a1mcmc]   | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A2]` below  |
| **A2** |         1 | RespiraWorks  | [Filter panel acrylic plate][a2rw]    |        3.75 |       Rw        | Lower acrylic face panel, cut from acrylic `[A1]`              |
| **A3** |         2 | RespiraWorks  | [Filter holder: tubing adapter][a3rw] |       25.00 |       Rw        | Filter housing assembly, variant with patient tubing adapter   |
| **A4** |         2 | RespiraWorks  | [Filter holder: outlet vent][a4rw]    |       25.00 |       Rw        | Filter housing assembly, variant with outlet vent              |
| **A5** |         6 | McMaster-Carr | 94180A351                             |        0.15 |   [C][a5mcmc]   | Heat-set inserts for m4 screws                                 |

**Total assembly price:** USD ~105.00

[a1mcmc]: https://www.mcmaster.com/8560K357/
[a2rw]:   #custom-parts
[a3rw]:   filter_holder
[a4rw]:   filter_holder
[a5mcmc]: https://www.mcmaster.com/94180A351/

## Custom parts

All Inventor part models, assembly models and drawings are within this repo directory.

### Filter panel acrylic plate

|                                       |                                                 |
|---------------------------------------|-------------------------------------------------|
| ![](images/filter_panel_acrylic.jpg)  | ![](images/filter_panel_acrylic_rendering.jpg)  |

Recently exported files for cutting acrylic can be found on our Google Drive at
[Manufacturing/Output files v0.4/Acrylic](https://tinyurl.com/4czevhxs).

The plate may be produced using a laser cutter or with common woodworking tools.

## Assembly

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

* Install heat set inserts (**A5**) into the corner holes for mounting to main equipment bracket.

* Disassemble the individual holders and integrate the housings with the acrylic panel using M4 hardware (**A6-8**).

Recently exported drawings for sub-assemblies can be found on our Google Drive at
[Manufacturing/Output files v0.4/Subassemblies](https://tinyurl.com/4d3s7zmw).

|                                        |                        |
|----------------------------------------|------------------------|
| ![](images/filter_panel_assembly.jpg)  | ![](images/panel1.jpg) |
