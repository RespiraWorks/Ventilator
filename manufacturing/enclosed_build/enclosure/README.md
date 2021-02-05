# Enclosure

| Assembly CAD Rendering | Exploded View |
:------------------:|:-----------------:|
| ![](images/enclosure_rendering.jpg)  | ![](images/enclosure_exploded.jpg)  |

|Download Type|Link|
|:------|:-------|
|Enclosure Assembly CAD|[.iam](CAD/enclosure_assembly.iam)|

#TODO : Add photos of only the assembled enclosure?

## Overview

This readme contains the details, BOM, assembly instructions for one display panel assembly for the enclosed build.

## Design Rationale

The principal reasons for using a sheet metal enclosure were threefold:
1. Cheap and quick - Great for prototyping stages, sheet metal is cheap to fabricate. Unlike plastic parts, it doesn't require any expensive molds. Any changes in the design would still end up going through the same process - cutting, forming and finishing. Plastic enclosures necessitate new molds for every design revision. Getting a new prototype ready based on a new design would take a very short amount of time.
2. Pre-assembly assembly - Our principal requirement for the design was the ability to assemble the entire pneumatic setup outside the enclosure, with the last few steps being attaching the pneumatics sub-assembly to the enclosure. A sheet metal design offered a very simple solution by means of two separate parts - the main enclosure and the removable closure panel.
3. Strength and durability - A well designed and meticulously fabricated sheet metal enclosure would not only be durable but also corrosion resistant.

## Design Overview
The ventilator enclosure is made up of three folded, 1.52mm thick, stainless steel sheets; the main panel, the rear
closure panel and the internal equipment tray. All three are custom parts. 

|Download Type|Link|
|:------|:-------|
|Enclosure Main Panel CAD|[.ipt](CAD/enclosure_main_panel.ipt)|
|Enclosure Equipment Tray CAD|[.ipt](CAD/enclosure_equipment_tray.ipt)|
|Enclosure Closure Panel CAD|[.ipt](CAD/enclosure_closure_panel.ipt)|

The main panel is folded from a single sheet and fastened together using stainless steel blind rivets. There
are rubber bumper feet on the bottom of the enclosure that are threaded into M6 press-fit nuts. The rubber bumpers
can be removed to allow the enclosure to be mounted to a medical cart.

The rear closure panel is folded from a single sheet.

The internal equipment bracket is folded from a single sheet. The bracket is designed to be simple and lightweight.
Acrylic sheets are either fastened to or hung from the bracket to allow for design adjustments while utilizing the same
bracket.

We are currently on v0.3. However, the design is being altered. Changes and updates are being tracked in the issue tickets [#957](https://github.com/RespiraWorks/Ventilator/issues/957) (this is an umbrella ticket and contains references to some of the other tickets), [#958](https://github.com/RespiraWorks/Ventilator/issues/958), [#959](https://github.com/RespiraWorks/Ventilator/issues/959), [#961](https://github.com/RespiraWorks/Ventilator/issues/961), [#962](https://github.com/RespiraWorks/Ventilator/issues/962), [#963](https://github.com/RespiraWorks/Ventilator/issues/963), [#964](https://github.com/RespiraWorks/Ventilator/issues/964), [#965](https://github.com/RespiraWorks/Ventilator/issues/965), [#970](https://github.com/RespiraWorks/Ventilator/issues/970).

## Fabrication Drawings

Enclosure fabrication drawings.

|             |     |
:------------------:|:-----------------:|
| ![](images/fab_drawing_1.png) | ![](images/fab_drawing_2.png) |
| ![](images/fab_drawing_3.png) | ![](images/fab_drawing_4.png) |
| ![](images/fab_drawing_5.png) | ![](images/fab_drawing_6.png) |
| ![](images/fab_drawing_7.png) | ![](images/fab_drawing_8.png) |
| ![](images/fab_drawing_9.png) | |


## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

**#TODO: locknuts and washers for main tray?**

**#TODO: How much sheet metal?**


| Item  | Quantity | Manufacturer  | Part #                   | Price (USD) | Sources[*][ppg]| Notes |
| ----- |---------:| ------------- | ------------------------ | -----------:|:----------:|:------|
|**A1** | 1        | RespiraWorks  | ventilator_enclosure     |             | [Rw][a1rw]   | Ventilator enclosure, bent sheet metal |
|**A2** | 1        | RespiraWorks  | equipment_bracket        |             | [Rw][a2rw]   | Ventilator equipment bracket, bent sheet metal |
|**A3** | 1        | RespiraWorks  | ventilator_closure       |             | [Rw][a3rw]   | Ventilator closure (back panel), bent sheet metal |
|**A4** | 10       | McMaster-Carr | 97525A415                | 8.87 / 100  | [C][a4mcmc]  | 1/8" blind rivets |
|**A5** | 4        | McMaster-Carr | 96439A710                | 6.04 / 5    | [C][a5mcmc]  | M6 self-clinching / press-fit nut |
|**A6** | 12       | McMaster-Carr | 96439A650                | 3.89 / 10   | [C][a6mcmc]  | M4 self-clinching / press-fit nut |
|**A7** | 4        | McMaster-Carr | 9541K82                  | 8.34 / 10   | [C][a7mcmc]  | M6 threaded-stud bumper, used as feet |
|**A8** | 4        | McMaster-Carr | 92095A193                | 8.95 / 50   | [C][a8mcmc]  | M4 x 14mm screws, hex drive |
|**A9** | 12       | McMaster-Carr | 92095A188                | 10.04 / 100 | [C][a9mcmc]  | M4 x 6mm screws, hex drive |

[a1rw]:   #fabrication-drawings
[a2rw]:   #fabrication-drawings
[a3rw]:   #fabrication-drawings
[a4mcmc]: https://www.mcmaster.com/97525A415/
[a5mcmc]: https://www.mcmaster.com/96439A710/
[a6mcmc]: https://www.mcmaster.com/96439A650/
[a7mcmc]: https://www.mcmaster.com/9541K82/
[a8mcmc]: https://www.mcmaster.com/92095A193/
[a9mcmc]: https://www.mcmaster.com/92095A188/
