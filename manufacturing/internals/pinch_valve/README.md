# Stepper Proportional Pinch Valve

|                               CAD Rendering                               |             Assembled              |         In action         |
|:-------------------------------------------------------------------------:|:----------------------------------:|:-------------------------:|
| [![](images/rendered_assembly_smaller.jpg)](images/rendered_assembly.jpg) | ![](images/assembled_smaller.jpg)  | ![](images/animation.gif) |

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
[life leader](../../../quality-assurance/testing/README.md#durability-and-life-leader-testing) to demonstrate the
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

**v1.6** Broader shelf, slightly larger bump.

**v1.7** set screw removed, 3/8"OD version added for potential use as oxygen supply valve.

**v1.8** gussets added to support main shelf, for improved durability. Last version developed in
[OnShape](https://cad.onshape.com/documents/3fe0c1f79c482144c267173d/w/2ad1c08071a25185f9c78c68/e/03a49465e4e026f9f102d0af).

**v2.0** All CAD migrated to Inventor. Following improvements implemented:
* rotor space in the base is tighter, for improved durability
* rotor is taller to better interface with 30mm axle bolt
* mounting screw positions changed for improved assembly and durability
* stop shelf distance from shaft is dictated by tube diameter and compression slack parameters
* highly parametrized model for improved adaptability in the future
* full assembly model with hardware

## Custom parts

All Inventor part models, assembly models and drawings are within this repo directory.

Recently exported files for 3d printing can be found on our Google Drive at
[Manufacturing/Output files v0.4/3d printing](https://tinyurl.com/2kuwps9h).

These parts should not be printed with FDM. Instead, a leak-tight method like SLA or MJF should be used.

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

> **TODO: Drawings**

|              |            Stepper base (Item 9)             |            Stepper rotor (Item 10)             |
|-------------:|:--------------------------------------------:|:----------------------------------------------:|
|    Rendering |        ![](images/rendered_base.jpg)         |         ![](images/rendered_rotor.jpg)         |
| Inventor IPT | [Pinch valve base.ipt](pinch_valve_base.ipt) | [Pinch valve rotor.ipt](pinch_valve_rotor.ipt) |

There is also an Inventor assembly with all the hardware [Pinch valve assembly.iam](pinch_valve_assembly.iam).

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item    | Quantity | Manufacturer  | Part #                | Price (USD) |     Sources[*][ppg]     | Notes                                                              |
|---------|---------:|---------------|-----------------------|------------:|:-----------------------:|--------------------------------------------------------------------|
| **A1**  |        1 | RespiraWorks  | [Stepper base][a1rw]  |         1.0 |           Rw            | BASE - 3D printed                                                  |
| **A2**  |        1 | RespiraWorks  | [Stepper rotor][a2rw] |         1.0 |           Rw            | ROTOR - 3D printed                                                 |
| **A3**  |        1 | STEPPERONLINE | 17HS19-2004S2         |       11.59 | [T][a3step] [Z][a3amzn] | Stepper motor. Make sure to get version with the full-cut D-shaft. |
| **A4**  |        3 | McMaster-Carr | 91294A128             |        0.05 |       [C][a4mcmc]       | M3 screws, 8mm, flat head: mounting stepper to base                |
| **A5**  |        1 | McMaster-Carr | 92095A218             |        0.31 |       [C][a5mcmc]       | M5 screws, 30mm: axle for bearing                                  |
| **A6**  |        3 | FastEddy      | TFE4262               |        1.25 | [F][a6fast] [C][a6mcmc] | 5x16x5 Metal shielded bearings                                     |
| **A7**  |        4 | McMaster-Carr | 92095A478             |        0.11 |       [C][a7mcmc]       | M4 screws, 18mm: base mounting to main assembly                    |
| **A8**  |        4 | McMaster-Carr | 93625A150             |        0.06 |       [C][a8mcmc]       | M4 lock nuts                                                       |
| **A9**  |        8 | McMaster-Carr | 93475A230             |        0.02 |       [C][a9mcmc]       | M4 washers, 9mm OD                                                 |
| **A10** |     15cm | McMaster-Carr | 5894K38               |   20.18 / m |      [C][a10mcmc]       | PVC Tubing, 3/8" ID, 5/8" OD, continuous-flex soft Tygon           |
| **A11** |        2 | McMaster-Carr | 9579K67               |        0.63 |      [C][a11mcmc]       | Herbie clip E (12-13.8mm) for clamping 5/8"OD tubing               |

**Total assembly price:** USD 22.93

[a1rw]:    #Custom-parts
[a2rw]:    #Custom-parts
[a3step]:  https://www.omc-stepperonline.com/full-d-cut-shaft-nema-17-bipolar-59ncm-84oz-in-2a-42x48mm-4-wires-w-1m-cable-connector-17hs19-2004s2
[a3amzn]:  https://www.amazon.com/dp/B07Z1J8JWH
[a4mcmc]:  https://www.mcmaster.com/91294A128
[a5mcmc]:  https://www.mcmaster.com/92095A218
[a6fast]:  https://www.fasteddybearings.com/5x16x5-metal-shielded-bearing-625-zz-10-units/
[a6mcmc]:  https://www.mcmaster.com/6153K15/
[a7mcmc]:  https://www.mcmaster.com/92095A478/
[a8mcmc]:  https://www.mcmaster.com/93625A150/
[a9mcmc]:  https://www.mcmaster.com/93475A230/
[a10mcmc]: https://www.mcmaster.com/5894K38
[a11mcmc]: https://www.mcmaster.com/9579K67

## Tools

| Item   | Quantity | Manufacturer   | Part #   | Price (USD) | Sources[*][ppg] | Notes                                        |
|--------|---------:|----------------|----------|------------:|:---------------:|----------------------------------------------|
| **B1** |        1 |  McMaster-Carr | 26475A38 |        9.55 |   [Z][b1mcmc]   | M5x0.8mm taper chamfer tap, for bearing axle |

[b1mcmc]: https://www.mcmaster.com/26475A38

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../methods).**

> **TODO: Drawings**

![Components](images/components.jpg)

You’ll need a vice to press on the rotors to the stepper motor shafts.  You may also need a tap `[B1]` to
chase the threads in the rotor `[A2]` for the bolt `[A5]` holding the bearings. On some
machines they way print too tight and you may crack the rotor trying to just muscle in the bolt without chasing the
threads first.  Prop tip: only chase a portion of the threads for the bolt that holds on the bearings, leaving a bit
to act as a lock nut.

|                         Rotor Assembly                          |                 Base Assembly                  |
|:---------------------------------------------------------------:|:----------------------------------------------:|
|                 ![Assembled](images/rotor.jpg)                  |       ![Assembled](images/assembled.jpg)       |
 | 3mm hex and 2.5mm hex driver are required for assembling rotor. | 2mm hex driver is required for attaching base. |

With the Tygon tube and clamps, a single pinch valve sub assembly appears like so:

![Assembled](images/full_assembly.jpg)

## System integration

Integration into the electrical/controls system is covered in
[electronics assembly page](../electronics).
