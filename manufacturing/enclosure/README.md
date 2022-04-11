# Enclosure

|              Front              |              Back               |
|:-------------------------------:|:-------------------------------:|
| ![](images/rendering_front.jpg) | ![](images/rendering_back.jpg)  |

## Design rationale

> **TODO:** Content..

## Parts

> #TODO: Update BOM to match CAD

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md

| Item   | Quantity | Manufacturer  | Part #              | Price (USD) | Sources[*][ppg] | Notes                                 |
|--------|---------:|---------------|---------------------|------------:|:---------------:|:--------------------------------------|
| **A1** |        1 | RespiraWorks  | [Bottom][a1rw]      |   (?)150.00 |       Rw        | Bottom assembly                       |
| **A2** |        1 | RespiraWorks  | [Front][a2rw]       |   (?)150.00 |       Rw        | Front                                 |
| **A3** |        1 | RespiraWorks  | [Side: right][a3rw] |   (?)150.00 |       Rw        | Side panel assembly: right mirror     |
| **A4** |        1 | RespiraWorks  | [Side: left][a4rw]  |   (?)150.00 |       Rw        | Side panel assembly: left mirror      |
| **A5** |        1 | RespiraWorks  | [Back][a5rw]        |   (?)150.00 |       Rw        | Back/cover                            |
| **A6** |       20 | McMaster-Carr | 97525A415           |        0.09 |   [C][a6mcmc]   | 1/8" blind rivets (can also be 3mm)   |
| **A7** |        6 | McMaster-Carr | 96439A650           |        0.39 |   [C][a7mcmc]   | M4 self-clinching / press-fit nut     |
| **A8** |        4 | McMaster-Carr | 96439A710           |        1.21 |   [C][a8mcmc]   | M6 self-clinching / press-fit nut     |
| **A9** |        4 | McMaster-Carr | 9541K82             |        0.83 |   [C][a9mcmc]   | M6 threaded-stud bumper, used as feet |

**Total assembly price:** USD ~750.00

> #TODO: How much raw sheet metal?

[a1rw]:    #sheet-metal-fabrication
[a2rw]:    #sheet-metal-fabrication
[a3rw]:    #sheet-metal-fabrication
[a4rw]:    #sheet-metal-fabrication
[a5rw]:    #sheet-metal-fabrication
[a6mcmc]:  https://www.mcmaster.com/97525A415/
[a7mcmc]:  https://www.mcmaster.com/96439A650/
[a8mcmc]:  https://www.mcmaster.com/96439A710/
[a9mcmc]:  https://www.mcmaster.com/9541K82/

## Custom parts

All Inventor part models, assembly models and drawings are within this repo directory.

### Sheet metal fabrication

Enclosure is manufactured from 0.050" powder-coated aluminum.

Recently exported files for sheet metal components can be found on our Google Drive at
[Manufacturing/Output files v0.4/Sheet metal](https://tinyurl.com/2p89pjja).

|                               |                                 |
|-------------------------------|---------------------------------|
| ![](images/drawing_front.jpg) | ![](images/drawing_back.jpg)    |
| ![](images/drawing_side.jpg)  | ![](images/drawing_bottom.jpg)  |

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../methods).**

The panels should be fastened together using stainless steel blind rivets. There
are rubber bumper feet on the bottom of the enclosure that are threaded into M6 press-fit nuts. The rubber bumpers
can be removed to allow the enclosure to be mounted to a medical cart.
