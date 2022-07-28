# Internal assembly

|                                   |                             |
|:---------------------------------:|:---------------------------:|
| ![](images/rendering_smaller.jpg) | ![](images/top_smaller.jpg) |

## Design rationale

The internals of the ventilator comprise the main active components of the machine.  To ease assembly and maintainance, they are split into three panels which are designed to be assembled or serviced outside of the enclosure, and then secured into the enclosure as a unit.  The three panels are:
 - **Pneumatics Panel**, described on this page, which hosts the main pneumatic workings of the ventilator: blower, oxygen regulator, control valves, and sensors.
 - **[Display Panel](display_panel)**, which hosts the main electrical workings of the ventilator: touchscreen, controller PCB, and device interfaces.
 - **[Filter Panel](filter_panel)**, which hosts the patient connection fittings and isolation filters.  Air enters and exits the ventilator through this panel.

### Acrylic panels

For serviceability, ventilator internals should be completely removable from the enclosure as a single assembly.
It was necessary to secure front acrylic panels (display panel and filter panel) to the main equipment bracket,
while also allowing it to lay flush with the front face of the enclosure. Heat-set inserts allow the panels to be
mounted securely without obstruction.

The sizing for the mounting holes was based on [this](https://www.pemnet.com/wp-content/uploads/sites/2/2022/06/sidata.pdf) catalog
for heat set inserts. For M4 nuts (page 5, metric table, row 6), length code 1 (5.72 mm) is small enough to fit in the
6.35mm-thick acrylic. The corresponding outer diameter for insertion i.e. acrylic mounting diameter is 5.94 mm.

> TODO: do same rationale explanation for filter housings

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md

| Item    | Quantity | Manufacturer  | Part #                               | Price (USD) | Sources[*][ppg] | Notes                                                                    |
|---------|---------:|---------------|--------------------------------------|------------:|:---------------:|--------------------------------------------------------------------------|
| **A1**  |        1 | RespiraWorks  | [Equipment tray][a1rw]               |      172.13 |       Rw        | Metal+acrylic tray holding the internal assembly together                |
| **A2**  |        1 | RespiraWorks  | [Filter panel][a2rw]                 |      105.00 |       Rw        | Lower front panel assembly with air filters                              |
| **A3**  |        1 | RespiraWorks  | [Blower assembly][a3rw]              |       84.02 |       Rw        | Blower sub-assembly                                                      |
| **A4**  |        1 | RespiraWorks  | [Air check valve][a4rw]              |       21.25 |       Rw        | Air check valve sub-assembly                                             |
| **A5**  |        2 | RespiraWorks  | [Pinch valves][a5rw]                 |       22.93 |       Rw        | Proportional pinch valve sub-assemblies with tubing                      |
| **A6**  |        1 | RespiraWorks  | [Regulator assembly][a6rw]           |      201.80 |       Rw        | Oxygen pressure regulator assembly                                       |
| **A7**  |        1 | RespiraWorks  | [Proportional solenoid][a7rw]        |       97.44 |       Rw        | Proportional solenoid oxygen control valve assembly                      |
| **A8**  |        1 | RespiraWorks  | [Oxygen/overpressure manifold][a8rw] |      131.00 |       Rw        | Oxygen sensor and overpressure relief valve manifold                     |
| **A9**  |        1 | RespiraWorks  | [Venturi: air influx][a9rw]          |        2.00 |       Rw        | Venturi flow sensor, air influx variant                                  |
| **A10** |        1 | RespiraWorks  | [Venturi: oxygen influx][a10rw]      |        2.00 |       Rw        | Venturi flow sensor, oxygen influx variant                               |
| **A11** |        1 | RespiraWorks  | [Venturi: exhale][a11rw]             |        2.00 |       Rw        | Venturi flow sensor, exhale variant                                      |
| **A12** |        1 | RespiraWorks  | [Oxygen mixer][a12rw]                |        1.00 |       Rw        | Oxygen mixing chamber                                                    |
| **A13** |        1 | RespiraWorks  | [Back panel][a13rw]                  |       25.84 |       Rw        | Back panel, with oxygen and main power input                             |
| **A14** |        1 | McMaster-Carr | 53415K153                            |        1.32 |  [C][a14mcmc]   | 3/4"ID barbed elbow                                                      |
| **A15** |        1 | McMaster-Carr | 5463K648                             |        0.84 |  [C][a15mcmc]   | 3/4"ID x 3/8"ID barbed reducer                                           |
| **A16** |      ~1m | OCSParts      | 117062-5                             |   15.09 / m |  [Z][a16amzn]   | Soft 3/4"ID 1"OD tubing, for main line tubing                            |
| **A17** |       24 | McMaster-Carr | 9579K73                              |        0.61 |  [C][a17mcmc]   | Herbie clip K (22.2-24.9mm) for clamping 1"OD, main tubing               |
| **A18** |        1 | RespiraWorks  | [Display panel][a18rw]               |      220.00 |       Rw        | Display panel                                                            |
| **A19** |      ~1m | McMaster-Carr | 5894K34                              |    7.97 / m |  [C][a19mcmc]   | 1/4"ID 3/8"OD Tygon tubing, for oxygen line tubing                       |
| **A20** |        6 | McMaster-Carr | 9579K64                              |        0.50 |  [C][a20mcmc]   | Herbie clip BB (9.2-10.3mm) for clamping 3/8"OD, oxygen line tubing      |
| **A21** |      ~1m | uxcell        | a16031400ux0163                      |    1.33 / m |  [Z][a21amzn]   | 2.5 mm tubing for connecting venturis to pressure sensors                |
| **A22** |        1 | McMaster-Carr | 2974K392                             |        0.82 |  [C][a22mcmc]   | sensing tube 3/23" y-junction, for teeing off to patient pressure sensor |
| **A23** |        1 | RespiraWorks  | [Wiring harnesses][a23rw]            |        9.39 |       Rw        | Various wiring assemblies                                                |
| **A24** |        3 | GTSE          | GTSE-10025BLU-USSL                   |        0.04 |  [Z][a24amzn]   | Zip ties, for securing sensor tubing and wiring harnesses                |
| **A25** |        1 | RespiraWorks  | [Electronics panel][a25rw]           |      540.00 |       Rw        | Electronics panel                                                        |
| **A26** |        1 | RespiraWorks  | [U-bend adapter 60°][a26rw]          |        1.00 |       Rw        | Custom U-bend adapter for 3/4" tubing with 60° turn                      |
| **A27** |        1 | RespiraWorks  | [U-bend adapter 90°][a27rw]          |        1.00 |       Rw        | Custom U-bend adapter for 3/4" tubing with 90° turn                      |

**Total assembly price:** USD ~1650.00

[a1rw]:    tray
[a2rw]:    filter_panel
[a3rw]:    blower
[a4rw]:    air_check_valve
[a5rw]:    pinch_valve
[a6rw]:    regulator
[a7rw]:    PSOL
[a8rw]:    manifold
[a9rw]:   venturi
[a10rw]:   venturi
[a11rw]:   venturi
[a12rw]:   mixer
[a13rw]:   back_panel
[a14mcmc]: https://www.mcmaster.com/53415K153/
[a15mcmc]: https://www.mcmaster.com/5463K648
[a16amzn]: https://www.amazon.com/gp/product/B01LZ5ZK53
[a17mcmc]: https://www.mcmaster.com/9579K73
[a18rw]:   display_panel
[a19mcmc]: https://www.mcmaster.com/5894K34
[a20mcmc]: https://www.mcmaster.com/9579K64
[a21amzn]: https://www.amazon.com/gp/product/B01F4BJ7PI
[a22mcmc]: https://www.mcmaster.com/2974K392
[a23rw]:   ../wiring
[a24amzn]: https://www.amazon.com/GTSE-Small-Strength-Nylon-Cable/dp/B086H4TR5Y
[a25rw]:   electronics
[a26rw]:   custom_tubing_adapters
[a27rw]:   custom_tubing_adapters

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../methods).**

[Pneumatics assembly model](internals.iam)

The rough order/hierarchy of internals assembly is as follows:

1. Assemble the [equipment tray](tray) which will hold everything else
2. Install the [filter panel](filter_panel) to the equipment bracket
3. Install the [blower assembly](blower) onto the pneumatics tray
4. Attach the [air check valve](air_check_valve) to the blower assembly
5. Install 2x [Pinch valves](pinch_valve), for air influx and exhale control
    * remember to label the pinch valve wiring "inhale" and "exhale" appropriately
6. Install the [oxygen regulator assembly](regulator)
7. Install the [proportional solenoid](PSOL) valve for oxygen control

|          Step 1           |         Steps 2-7         |
|:-------------------------:|:-------------------------:|
| ![](images/process_1.jpg) | ![](images/process_2.jpg) |

8. Install the main tubing, sensors and manifolds:
    * [Manifold](manifold) - for oxygen sensor and overpressure relief valve
    * [Oxygen mixer](mixer) - for mixing air and oxygen before delivery to patient
    * [Venturi flow sensors](venturi) - in-house design of affordable flow sensors
    * Various tubing adapters `[13-15]`, `[26-27]`
    * 3/4 tubing `[16]` and clamps `[17]`
9. Install the [display panel](display_panel)
10. Install oxygen line tubing `[A19]` and clamps `[20]`

|          Step 8           | Steps 9-10 (except clamps) |
|:-------------------------:|:--------------------------:|
| ![](images/process_3.jpg) | ![](images/process_4.jpg)  |


### Sensor tubing

Sensor tubing `[A21-22]` connects the venturis to pressure sensors in the electronics assembly.

The tubes you will use will vary in length. Use your own discretion.

Use the following principles to guide this process:
* The venturis have arrows embossed on the sides indicating flow direction. You can thus determine which of the two
  sensor ports is "upstream" vs. which one is "downstream".
* The differential sensors on the PCB (or daughter-card) have 2 ports each:
  * the upper port (further from board) is for upstream
  * the lower port (closer to board) is for downstream

Connect the tubing as follows
* upstream port on the `air influx venturi` to upper port on the `INHALE FLOW` sensor on the PCB
* downstream port on the `air influx venturi` to lower port on the `INHALE FLOW` sensor on the PCB
* downstream port on the `exhale venturi` to lower port on the `EXHALE FLOW` sensor on the PCB
* upstream port on the `exhale venturi` to wye splitter `[A22]`
* wye splitter `[A22]` to the upper port on the pressure sensor on the daughter-card
* wye splitter `[A22]` to the upper port on the `EXHALE FLOW` sensor on the PCB
* the lower port on the pressure sensor on the daughter-card remains open to atmosphere
* upstream port on the `oxygen influx venturi` to upper port on the `PATIENT PRESSURE` sensor on the PCB
* downstream port on the `oxygen influx venturi` to lower port on the `PATIENT PRESSURE` sensor on the PCB

>**TODO:** update drawings with updated tubing (O2 venturi on U3 / patient pressure on the daughter-card)
> This allows us to take advantage of the higher maximum pressure of the MPXV5010DP sensor on the daughter-card.

|           Air influx           |         Oxygen influx          |  Outflow and patient pressure  |
|:------------------------------:|:------------------------------:|:------------------------------:|
| ![](images/sensor_tubes_1.jpg) | ![](images/sensor_tubes_2.jpg) | ![](images/sensor_tubes_3.jpg) |


### Final wiring

By this point you should have completed all the mechanical and pneumatic sub-assemblies of the ventilator.

You can now connect all the wiring (except main power):
* RaspberryPi's `USB` to touchscreen's `5V+Touch` microUSB, using `[A8]`
* RaspberryPi's `microHDMI` to touchscreen's ribbon cable
* connect blower power wires to PCB
* connect blower control wires
  * to the interim board, if you have one
  * else, to the PCB
* connect proportional solenoid to PCB
* connect oxygen sensor to interim board
* connect the pinch valves to their driver pigtails as follows:
  * top - inhale
  * bottom - exhale
* Secure wiring with zip ties `[A24]`
* Secure sensor tubing with zip ties `[A24]`, but do so lightly. Don't tighten them to the point where the
  air flow is restricted in the tubes or where the wires can't move at all.

|          Top view           |          Bottom view           |
|:---------------------------:|:------------------------------:|
| ![](images/top_smaller.jpg) | ![](images/bottom_smaller.jpg) |
