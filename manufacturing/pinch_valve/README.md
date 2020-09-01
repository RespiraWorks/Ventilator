# Stepper Proportional Pinch Valve

|   CAD File         |  Assembled        |  In action    |
|:------------------:|:-----------------:|:-------------:|
![](assets/rendering-1-6.png)  |  ![](assets/assembled_smaller.jpg)  |  ![](assets/animation.gif) |

## Overview

This contains details, BOM, & instructions for one pinch valve assembly for the pneumatic circuit.

For a full prototype build you will need 2 pinch valves, in which case it makes sense to purchase a 2-axis stepper driver,
though you can work with two single-stepper drivers if that's what you already have.

## Design

Two custom pinch valves are used to modulate flow through the ventilator: a valve on the inspiratory path works in
conjunction with the blower to enable quick responses to required flow changes; an identical valve on the expiratory
path closes periodically to minimize oxygen wastage due to excess bias flow during high pressure periods of the
breathing cycle.

Flexible tubing-based pinch valves were selected based on the requirements for quick response time, and the ability to
keep the gas pathway clean (a feature unique to pinch valves, as most other forms of actuated valves require passing the
gas through the valve workings).

The soft rubber tubing selected is manufactured and rated for peristaltic pumps, which have similar loads consisting of
cyclical stresses on the tubing walls. This tubing is 5/8"OD x 3/8"ID, somewhat smaller than the main ventilator circuit
tubing standard in order to make it easier to completely seal the tube when the valve is closed. Roller bearings on the
rotor eliminate frictional wear on the outer surface of the tubing. While this tubing is rugged enough for weeks of use
in this application, it will likely need to be replaced periodically. Current testing is underway using a
[life leader](../../quality-assurance/testing/README.md#durability-and-life-leader-testing) to demonstrate the
lifetime of the tubing and to quantify degradation in performance over time.

The valves are an assembly of two custom plastic parts with commonly available off-the-shelf hardware. The design of the
custom plastic parts requires minor modification to be amenable to injection molding in ABS plastic, which is the
intended at-scale production method.

### Fabrication at Scale

For the pinch valve fabrication at scale, the key difference would be to use the enclosure mounting plate as part of the
assembly, removing the housing (item 10) in the BOM above. The tube supports would be replaced with routing clamps and
the pinch plate would be a single injection molded ABS piece affixed to the mounting plate.

The rotor is slightly more complicated, and a few different iterations have been proposed. Likely the cheapest and
fastest method to produce would be to procure aluminum 60601 bar stock with the correct outer dimensions. Blanks for
each rotor would be cut and milled to final size. A group of 10-15 rotors would be placed in a vice with the central
channel milled out. The set screw hole would be through-drilled and tapped. A third operation would be required to drill
and tap the axel and bearing shaft holes.

The boss on the rotors to isolate the inner race of the bearing would be replaced with a small shim washer.

### Change history

The stop is moved down by 3 mm, which should buy us a lot more leverage.
The stop is now flat. We might need to tweak exact dimensions further to get a perfect close, but this should be better.
Watching the video I'm pretty concerned watching that stop flex. I made it run the full width, I made it thicker, and I also bulked out the tube holders.
Bearings are now better aligned with the tube.

v1.6: Broader shelf, slightly larger bump.

v1.7: set screw removed, 3/8"OD version added for potential use as oxygen supply valve.

## 3D Printing

### Guidelines

These files should not be printed with FDM. Instead, a leak-tight method like SLA or MJF should be used.

### Files

- [Base - .step format](assets/exhaust-pinch-valve-1.6-base.step)
- [Rotor - .step format](assets/exhaust-pinch-valve-1.6-rotor.step)

Original designs found
[here](https://cad.onshape.com/documents/3fe0c1f79c482144c267173d/w/2ad1c08071a25185f9c78c68/e/03a49465e4e026f9f102d0af).

You may also find other variations of the design meant for special testing/calibration units.


## Parts List (BOM)

**Note: If you are a member of the RespiraWorks team, review the
[part purchasing guidelines](../../manufacturing/README.md#part-purchasing-guidelines)
BEFORE purchasing any parts.**

### Purchasing Source Key

* **C**  = McMaster-Carr
* **Z**  = Amazon
* **F**  = Fast Eddy Bearings
* **K**  = Digikey
* **M**  = Mouser
* **3D** = 3D printed (RespiraWorks)
* **S**  = ST
* **R** = RS computing
(**note**: links provided are for worldwide customers; there are local RS retail sites for most countries that use the same product reference (RS Stock No.) and will probably ship faster if you order from your country's site)
* **T**  = Stepperonline.com

### Parts

| Item   | Quantity | Manufacturer    | Part #              | Price (USD)  | Sources              | Notes |
| ------ |---------:| --------------- | ------------------- | ------------:| ---------------------| ----- |
| 1      |        1 | STEPPERONLINE   | 17HS19-2004S2       |11.59 / each  | [T][1step] [Z][1amzn]| Stepper motor. Make sure to get one with the full-cut D-shaft. |
| 2      |        1 | McMaster-Carr   | 92095A218           |   7.64 / 25  | [C][2mcmc]           | M5x30mm button head. Axle for bearing. |
| 3      |        3 | McMaster-Carr   | 91294A128           |  4.82 / 100  | [C][3mcmc]           | M3x8mm flat head. Attaches frame to stepper body |
| 4      |        3 | FastEddy        | TFE4262             |  12.50 / 10  | [F][4fast]           | 5x16x5 Metal shielded bearings |
| 5      | 6in/15cm | McMaster-Carr   | 5894K38             |  12.30 / 2 ft| [C][5mcmc]           | Continuous-Flex Soft Tygon PVC Tubing, 3/8" ID, 5/8" OD |
| 6      |        2 | McMaster-Carr   | 9579K67             |  12.63 / 20  | [C][6mcmc]           | Herbie clip E, 12-13.8mm, to clamp tubing to fittings |
| 7      |        1 | Digikey         | X-NUCLEO-IHM03A1    | 10.64 / each | [K][7key] [M][7mr]   | Stepper driver dev board |
| 8      |        1 | ST              | X-NUCLEO-IHM02A1    | 15.00 / each | [S][8st] [R][8rs]    | **ALT for item 10:** Two-axis stepper driver dev board |
| 9      |        1 | RespiraWorks    | N/A                 |       N/A    | [3D][9print]         | BASE - 3D printed |
| 10     |        1 | RespiraWorks    | N/A                 |       N/A    | [3D][10print]        | ROTOR - 3D printed |

[1step]:   https://www.omc-stepperonline.com/nema-17-stepper-motor/nema-17-bipolar-59ncm-84oz-in-2a-42x48mm-4-wires-w-1m-cable-and-connector-full-d-cut-shaft.html
[1amzn]:   https://www.amazon.com/dp/B07Z1J8JWH
[2mcmc]:   https://www.mcmaster.com/92095A218
[3mcmc]:   https://www.mcmaster.com/91294A128
[4fast]:   https://www.fasteddybearings.com/5x16x5-metal-shielded-bearing-625-zz-10-units/
[5mcmc]:   https://www.mcmaster.com/5894K38
[6mcmc]:   https://www.mcmaster.com/9579K67
[7key]:    https://www.digikey.com/short/z442qt
[7mr]:     https://www.mouser.com/ProductDetail/511-X-NUCLEO-IHM03A1
[8st]:     https://www.st.com/en/ecosystems/x-nucleo-ihm02a1.html#sample-and-buy
[8rs]:     https://export.rsdelivers.com/product/stmicroelectronics/x-nucleo-ihm02a1/stmicroelectronics-x-nucleo-ihm02a1-two-axis/1646982
[9print]:  #files
[10print]: #files

### Tools

| Item | Quantity | Manufacturer  | Part #      | Price (USD) | Sources         | Notes |
| ---- |---------:| ------------- | ----------- | ----------:|-----------------| ----- |
| a1   |        1 | TEKTON        | 37122       |       9.99 | [Z][a1amzn]      | Some sort of wrench/pliers, to clamp the herbie clips around the tubing+fitting. |
| a2   |        1 | Wiha          | 71397       |       6.76 | [Z][a2amzn]      | 2, 3mm hex drivers required for assembly.  |
| a3   |        1 | McMaster-Carr | 26475A38    |       9.55 | [Z][a3mcmc]      | M5x0.8mm taper chamfer tap, for bearing axle.  |

[a1amzn]:  https://www.amazon.com/TEKTON-2-Inch-Joint-Pliers-37122/dp/B00KLY1FAY
[a2amzn]:  https://www.amazon.com/Wiha-71397-Metric-Insert-6-Piece/dp/B0084B7S70/ref=sr_1_4?dchild=1&keywords=3mm+hex&qid=1590347774&sr=8-4
[a3mcmc]:  https://www.mcmaster.com/26475A38

## Prototype fabrication

![Components](assets/components.jpg)

You’ll need a vice to press on the rotors to the stepper motor shafts.  You may also need a tap ([Item a3](#tools)) to
chase the threads in the rotor ([Item 10](#parts)) for the bolt ([Item 2](#parts)) holding the bearings. On some
machines they way print too tight and you may crack the rotor trying to just muscle in the bolt without chasing the
threads first.  Prop tip: only chase a portion of the threads for the bolt that holds on the bearings, leaving a bit
to act as a lock nut.

Rotor Assembly                     |  Base Assembly           |
:---------------------------------:|:-------------------------:
![Assembled](assets/rotor.jpg)|  ![Assembled](assets/assembled.jpg)
3mm hex and 2.5mm hex driver are required for assembling rotor.|  2mm hex driver is required for attaching base.

Each pinch valve must also include some additional wiring and a driver board for controlling the stepper motor. A
pigtail with male dupont connectors must be manufactured for interfacing to the driver board. Furthermore, since a
single ventilator contains two pinch valves, each must come with its own driver board configured with 0-ohm resistors
soldered appropriately. All of this is documented in a subset of the PCB documentation
[here](../../pcb/rev1_export/stepper_driver_setup.md).

With the tygon tube and clamps, a single pinch valve sub assembly appears like so:

![Assembled](assets/full_assembly.jpg)
