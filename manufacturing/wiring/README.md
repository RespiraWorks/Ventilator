# Wiring assemblies

**DEPRECATED - this page is deprecated, as it is being migrated to Sphinx-generated documentation located at [respiraworks.github.io/Ventilator](file:///home/martin/dev/ventilator/docs/_build/html/wiring/index.html)**

> TODO: Migrate contents to Sphinx

The following page describes the custom wiring components required in the electrical assembly. Manufacturing
these components will require soldering and crimping.

For a single ventilator unit, the following custom wiring assemblies are needed:
* [Blower power](#blower-power)
* [Blower signal/control](#blower-control-cable)
* [Stepper motor pigtails (x2)](#stepper-wiring)
* [Proportional solenoid](#proportional-solenoid-valve)
* [Oxygen sensor](#oxygen-sensor-cable)

It is probably a good idea to manufacture and test these before proceeding with the rest of the mechanical assembly.

Total price estimate: USD 24.00

Stand-alone items (not in other assemblies): USD 9.39

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md

| Item    | Quantity | Manufacturer    | Part #                |  Price (USD) |   Sources[*][ppg]    | Notes                                                          |
|---------|---------:|-----------------|-----------------------|-------------:|:--------------------:|----------------------------------------------------------------|
| **A1**  |       20 | JST             | SXH-001T-P0.6         |         0.10 |      [K][a1key]      | XH crimp contacts, for sensor & actuator connections           |
| **A2**  |        1 | JST             | XHP-2                 |         0.10 |      [K][a2key]      | XH 2-pin housings, for solenoid                                |
| **A3**  |        1 | JST             | XHP-3                 |         0.10 |      [K][a3key]      | XH 3-pin housings, for oxygen sensor                           |
| **A4**  |        1 | JST             | XHP-6                 |         0.12 |      [K][a4key]      | XH 6-pin housings, for blower control                          |
| **A5**  |        7 | JST             | SVH-21T-P1.1          |         0.11 | [K][a5key] [R][a5rs] | VH crimp contacts, for main power, for blower power            |
| **A6**  |        2 | TE              | 1-1123722-2           |         0.18 | [K][a6key] [R][a6rs] | VH housing 2 pin, for blower power                             |
| **A10** |        8 | TE Connectivity | 102107-3              |         0.47 |     [K][a10key]      | 2.54mm male pin, for stepper interface                         |
| **A11** |        2 | TE Connectivity | 87499-7               |         0.73 |     [K][a11key]      | 2.54mm 4-pin housing, for stepper interface                    |
| **A12** |   ~2.5 m | Remington       | 16UL1007STRKIT        |     0.66 / m |     [Z][a12amzn]     | 16 AWG wire, for power cables                                  |
| **A13** |   ~1.5 m | Remington       | 22UL1015STRKIT10COLOR |     0.50 / m |     [Z][a13amzn]     | 22 AWG wire, for signal/control cables                         |
| **A14** |     ~4 m | Remington       | 26UL1007STRKIT10COLOR |     0.42 / m |     [Z][a14amzn]     | 26 AWG wire, for signal/control cables                         |
| **A15** |      (1) | WGCD            | 4330589508            |         8.99 |     [Z][a15amzn]     | XH connector kit for 2/3/4/5 pins, **alternative for A1-3**    |
| **A16** |      (1) | CQRobot         | CQRJST254-BGB         |         9.99 |     [Z][a16amzn]     | XH connector kit for 6/7/8 pins, **alternative for A1 and A4** |
| **A17** |      (1) | Chenbo          | WJ-Z01                |        11.98 |     [Z][a17amzn]     | 2.54mm connector kit, **alternative for A10 and A11**          |
| **A18** |        2 | Molex           | 22013037              |         0.20 |     [K][a18key]      | KK-series 3-pin sockets connector housing (oxygen sensor)      |
| **A19** |        4 | Molex           | 08550102              |         0.21 |     [K][a19key]      | crimp sockets for the above                                    |
| **A20** |     ~1 m | uxcell          | a11110900ux0059       |     1.32 / m |     [Z][a20amzn]     | Shrink tubing, black, 3mm diameter                             |
| **A21** |     ~2 m | uxcell          | A17032300ux0991       |     1.50 / m |     [Z][a21amzn]     | Shrink tubing, black, 4mm diameter                             |
| **A22** |   ~0.5 m | XHF             | B07WC9BGRT            |     0.95 / m |     [Z][a22amzn]     | Shrink tubing, red, assorted diameters                         |

[a1key]:   https://www.digikey.com/short/z44f8d
[a2key]:   https://www.digikey.com/short/z44f8f
[a3key]:   https://www.digikey.com/short/z44fb9
[a4key]:   https://www.digikey.com/short/z44ff8
[a5key]:   https://www.digikey.com/short/z44fjr
[a5rs]:    https://export.rsdelivers.com/product/jst/svh-41t-p11/jst-nv-vh-female-crimp-terminal-contact-16awg-svh/7620692
[a6key]:   https://www.digikey.com/short/z44fwj
[a6rs]:    https://export.rsdelivers.com/product/jst/vhr-2n/jst-vhr-female-connector-housing-396mm-pitch-2/8201172
[a10key]:  https://www.digikey.com/en/products/detail/te-connectivity-amp-connectors/102107-3/298993
[a11key]:  https://www.digikey.com/en/products/detail/te-connectivity-amp-connectors/87499-7/29911
[a12amzn]: https://www.amazon.com/gp/product/B00N51OJJ4
[a13amzn]: https://www.amazon.com/gp/product/B073SDGNKW
[a14amzn]: https://www.amazon.com/gp/product/B011JC76OA
[a15amzn]: https://www.amazon.com/gp/product/B06ZZ45G7G
[a16amzn]: https://www.amazon.com/gp/product/B079MJ1RYN
[a17amzn]: https://www.amazon.com/CHENBO-Connector-Housing-Assortment-Terminal/dp/B077X8XV2J
[a18key]:  https://www.digikey.com/en/products/detail/molex/0022013037/26433
[a19key]:  https://www.digikey.com/en/products/detail/molex/0008550102/172056
[a20amzn]: https://www.amazon.com/uxcell-Polyolefin-Insulation-Shrink-Tubing/dp/B008DFW8JA
[a21amzn]: https://www.amazon.com/uxcell-4mm-Shrink-Sleeve-Tubing/dp/B071VGQH97
[a22amzn]: https://www.amazon.com/XHF-140-PCS-Red-Wear-Resistant/dp/B07WC9BGRT

## Tools

| Item | Manufacturer | Part number        | Price($) | Sources[*][ppg] | Notes                              |
|------|--------------|--------------------|---------:|-----------------|------------------------------------|
| B1   | Preciva      | PR-3254 (PR102001) |    39.99 | [Z][b1amzn]     | Crimping tool (with connector kit) |
| B2   | NovelLife    | TS100              |    74.99 | [Z][b2amzn]     | 65W Soldering iron kit             |
| B3   | Chandler     | HG603D             |    24.92 | [Z][b3amzn]     | Heat gun, for shrink tubing        |
| B4   | Kester       | KE1107-ND          |    58.98 | [K][b4key]      | Solder                             |

[b1amzn]: https://www.amazon.com/gp/product/B07R1H3Z8X
[b2amzn]: https://www.amazon.com/NovelLife-Mini-TS100-Soldering-Digital/dp/B07D35B75T
[b3amzn]: https://www.amazon.com/Chandler-Tool-Embossing-Heat-Gun/dp/B07GC5N3QC
[b4key]:  https://www.digikey.com/en/products/detail/kester-solder/24-6040-0010/265622

**Crimper Tips:**

* Position 1 on the crimper will crimp JST XH contacts `[A1]` and Molex KK contacts `[A19]`
* Position 2 on the crimper will crimp VH contacts `[A5]`.  However, with certain gauges of wire, this crimper may bend the contact into a shrimp-like shape.  If this happens, bend the contact back straight (it should be pretty obvious as it blocks insertion)
* Position 3 on the crimper will crimp 2.54mm (aka Dupont) sockets and pins `[A10]`.
* A proper crimp should crimp the insulation in the lower half and the wire in the upper half of the crimp portion of the contact.
* The crimper kit `[B1]` comes with a lot of XH contacts, but not any VH contacts.

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../methods).**

Recently exported drawings for wiring harnesses can be found on our Google Drive at
[Manufacturing/Output files v0.4/Wiring](https://tinyurl.com/3rcj85m9).

As much as possible we should try to keep the cables one-to-one
(for example the wiring of the PCB to blower driver cables are all pin 1 to pin 1,
pin 2 to pin 2, etc. with the same connector style on both ends.

### Blower power

The power cable that comes included with the blower will be too short to reach the PCB,
so a new one must be manufactured.

* Using:
  - 16AWG 50cm in each of black and red `[A12]`
  - 4x VH crimp contacts `[A5]`
  - 2x VH 2-pin housings `[A6]`
  - Shrink tubing `[A21]`

- Instructions:
  - Crimp the VH contacts onto one end of each of the wires.
  - Slide on a few extra lengths of heat shrink tubing for cable management before you proceed.
  - Crimp the VH contacts onto the other end of the wires.
  - Push the contacts into the housings per the drawing until they click.
  - Do a gentle pull test to confirm proper seating of the contact.
  - Shrink the remaining pieces of heat shrink to make the cable more manageable.

- Total price: USD 2.87

|                     Cable Image                      |                                Cable Drawing                                |
|:----------------------------------------------------:|:---------------------------------------------------------------------------:|
| ![Blower Power Cable](images/blower_power_cable.jpg) | ![Blower Power Cable Drawing](images/blower_power_cable_drawing_image.jpg)  |

### Blower control cable

The signal cable that comes included with the blower will be too short to reach the PCB,
so a new one must be manufactured.

* Using:
  - 26AWG 50cm each of 6 different colors `[A14]`
  - 12x XH crimp contacts `[A1]`
  - 2x XH 6-pin housings `[A4]`
  - Shrink tubing `[A21]`

- Instructions:
  - Crimp the XH contacts onto one end of each of the wires.
  - Slide on a few extra lengths of heat shrink tubing for cable management before you proceed.
  - Crimp the XH contacts onto the other end of the wires.
  - Push the contacts into the housings per the drawing until they click.
  - Do a gentle pull test to confirm proper seating of the contact.
  - Shrink the remaining pieces of heat shrink to make the cable more manageable.

- Total price: USD 3.45

|                       Cable Image                        |                                  Cable Drawing                                  |
|:--------------------------------------------------------:|:-------------------------------------------------------------------------------:|
| ![Blower Control Cable](images/blower_control_cable.jpg) | ![Blower Control Cable Drawing](images/blower_control_cable_drawing_image.jpg)  |

### Stepper wiring

To power and control the pinch valves, they will need to be wired to the stepper driver boards. Two pigtails
can be produced for this purpose:

Recipe for one pigtail:
* 4x 22AWG 15cm `[A13]`
* 4x male 2.54mm crimp pins `[A10]`
* 2.54mm 4-pin housing `[A11]`

On the other end, each wire should be stripped about 5mm and solder applied to bind the threads.

It is advisable to label these pigtails to identify which stepper driver they shall be wired up to.
The same can be done for the stepper motors belonging to each pinch valve assembly.

Single pigtail price: USD 3.00

|         Pigtails         |         Stepper wires          |
|:------------------------:|:------------------------------:|
| ![](images/pigtails.jpg) | ![](images/stepper_labels.jpg) |

You will also need 2x small (about 5cm) 16AWG wires for connecting the two stepper boards together for power.

### Proportional solenoid valve

To connect the solenoid to the PCB, you will need to crimp a 2-pin XH connector to the wires.

- Using:
  - 2x XH crimp contacts `[A1]`
  - XH 2-pin housings `[A2]`
  - Shrink tubing `[A20]`

- Instructions:
  - Note that the polarity of these connections does not matter.
  - Slide a few extra lengths of heat shrink tubing onto the solenoid wires for cable management before you proceed.
  - Crimp the XH contacts onto the other end of the wires.
  - Push the contacts into the housings per the drawing until they click.
  - Do a gentle pull test to confirm proper seating of the contact.
  - Shrink the remaining pieces of heat shrink to make the cable more manageable.

- Total price: USD 0.96

|                      Cable Image                      |                                Cable Drawing                                 |
|:-----------------------------------------------------:|:----------------------------------------------------------------------------:|
| ![Proportional Solenoid Cable](images/psol_cable.jpg) | ![Proportional Solenoid Cable Drawing](images/psol_cable_drawing_image.jpg)  |

### Oxygen Sensor Cable

This cable connects the Oxygen Sensor to the Interim Board in the PCB stack.

- Using:
  - 26AWG 40cm in each of green and white `[A14]`
  - 4x KK series crimp contacts `[A19]`
  - 2x KK series 3-pin housings `[A18]`
  - Shrink tubing `[A20]`

- Instructions:
  - Strip the ends of the wires
  - Using the crimper `[B1]` in position 1, crimp 4x contacts `[A19]` onto the ends of the wires.
  - Sheathe the length of the wires in 3mm shrink tubing `[A20]` before inserting the ends into housing.
  - Insert the wires into the connector housings `[A18]`, according to the drawing, until they click.  Pin 1 is marked by a tiny '1' molded into the connector housing.
  - Do a gentle pull test to make sure that the contacts are properly crimped and seated.

- Total price: USD 2.11

|                      Cable Image                       |                                 Cable Drawing                                 |
|:------------------------------------------------------:|:-----------------------------------------------------------------------------:|
| ![Oxygen Sensor Cable](images/oxygen_sensor_cable.jpg) | ![Oxygen Sensor Cable Drawing](images/oxygen_sensor_cable_drawing_image.jpg)  |
