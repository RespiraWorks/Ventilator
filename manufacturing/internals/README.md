# Internal assembly

|  |  |
:------------------:|:-----------------:|
| [![](images/rendering_smaller.jpg)](images/rendering_full.jpg)  | [![](images/open_back_smaller.jpg)](images/open_back.jpg) |

## Design rationale

> **TODO:** Content..

### Acrylic panels

For serviceability, ventilator internals should be completely removable from the enclosure as a single assembly.
It was necessary to secure front acrylic panels (display panel and filter panel) to the main equipment bracket,
while also allowing it to lay flush with the front face of the enclosure. Heat-set inserts allow the panels to be
mounted securely without obstruction.

The sizing for the mounting holes was based on [this](https://www.pemnet.com/fastening_products/pdf/sidata.pdf) catalog
for heat set inserts. For M4 nuts (page 6, metric table, row 6), length code 1 (5.72 mm) is small enough to fit in the
6.35mm-thick acrylic. The corresponding outer diameter for insertion i.e. acrylic mounting diameter is 5.94 mm.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md

| Item  | Quantity | Manufacturer   | Part #                | Price (USD)  | Sources[*][ppg]| Notes |
| ----- |---------:| -------------- | --------------------- | ------------:|:--------------:| ----- |
|**A1** | 1        | RespiraWorks   | equipment_bracket     |              | [Rw][a1rw]     | Metal bracket holding the internal assembly together **from enclosure assemebly** |
|**A2** | 1        | RespiraWorks   | pneumatics_tray       |              | [Rw][a2rw]     | Acrylic tray upon which most of the pneumatics are mounted |
|**A3** | 1        | RespiraWorks   | filter_panel          |              | [Rw][a3rw]     | Lower front panel assembly with air filters |
|**A4** | 1        | RespiraWorks   | blower_assembly       |              | [Rw][a4rw]     | Blower sub-assembly |
|**A5** | 1        | RespiraWorks   | air_check_valve       |              | [Rw][a5rw]     | Air check valve sub-assembly |
|**A6** | 2        | RespiraWorks   | pinch_valve_assembly  |              | [Rw][a6rw]     | Proportional pinch valve sub-assemblies with tubing |
|**A7** | 1        | RespiraWorks   | regulator_assembly    |              | [Rw][a7rw]     | Oxygen pressure regulator assembly |
|**A8** | 1        | RespiraWorks   | PSOL_assembly         |              | [Rw][a8rw]     | Proportional solenoid oxygen control valve assembly |
|**A9** | 1        | RespiraWorks   | manifold              |              | [Rw][a9rw]     | Oxygen sensor and overpressure relief valve manifold |
|**A10**| 1        | RespiraWorks   | venturi_air_influx    |              | [Rw][a10rw]    | Venturi flow sensor, air influx variant |
|**A11**| 1        | RespiraWorks   | venturi_oxygen_influx |              | [Rw][a11rw]    | Venturi flow sensor, oxygen influx variant |
|**A12**| 1        | RespiraWorks   | venturi_exhale        |              | [Rw][a12rw]    | Venturi flow sensor, exhale variant |
|**A13**| 1        | RespiraWorks   | oxygen_mixer          |              | [Rw][a13rw]    | Oxygen mixing chamber |
|**A14**| 1        | RespiraWorks   | reducer_3-4_1-4       |              | [Rw][a14rw]    | 1/4"ID x 3/4"ID reducer |
|**A15**| 5        | McMaster-Carr  | 53415K153             | 13.16 / 10   | [C][a15mcmc]   | 3/4"ID barbed elbow |
|**A16**| 1        | McMaster-Carr  | 5463K648              | 8.40 / 10    | [C][a16mcmc]   | 3/4"ID x 3/8"ID barbed reducer |
|**A17**| ~1m      | OCSParts       | 117062-5              | 22.99 / 5ft  | [Z][a17amzn]   | Soft 3/4"ID 1"OD tubing, for main line tubing |
|**A18**| 24       | McMaster-Carr  | 9579K73               | 12.24 / 20   | [C][a18mcmc]   | Herbie clip K (22.2-24.9mm) for clamping 1"OD, main tubing |
|**A19**| 1        | RespiraWorks   | display_panel         |              | [Rw][a19rw]    | Display panel and electronics assembly |
|**A20**| ~1m      | McMaster-Carr  | 5894K34               | 12.15 / 5ft  | [C][a20mcmc]   | 1/4"ID 3/8"OD Tygon tubing, for oxygen line tubing |
|**A21**| 6        | McMaster-Carr  | 9579K64               | 9.90 / 20    | [C][a21mcmc]   | Herbie clip BB (9.2-10.3mm) for clamping 3/8"OD, oxygen line tubing |
|**A22**| ~1m      | uxcell         | a16031400ux0163       | 6.49 / 16ft  | [Z][a22amzn]   | 2.5 mm tubing for connecting venturis to pressure sensors |
|**A23**| 1        | McMaster-Carr  | 2974K392              | 8.16 / 10    | [C][a23mcmc]   | sensing tube 3/23" y-junction, for teeing off to patient pressure sensor |
|**A24**| -        | RespiraWorks   | wiring_harnesses      |              | [Rw][a24rw]    | Various wiring harnesses, as described on the linked page |
|**A25**| 3        | GTSE           | GTSE-10025BLU-USSL    | 4.49 / 100   | [Z][a25amzn]   | Zip ties, for securing sensor tubing and wiring harnesses |

> **TODO** there can be only one tray

[a1rw]:    tray
[a2rw]:    tray
[a3rw]:    filter_panel
[a4rw]:    blower
[a5rw]:    air_check_valve
[a6rw]:    pinch_valve
[a7rw]:    regulator
[a8rw]:    PSOL
[a9rw]:    manifold
[a10rw]:   venturi
[a11rw]:   venturi
[a12rw]:   venturi
[a13rw]:   mixer
[a14rw]:   reducer_3-4_1-4
[a15mcmc]: https://www.mcmaster.com/53415K153/
[a16mcmc]: https://www.mcmaster.com/5463K648
[a17amzn]: https://www.amazon.com/gp/product/B01LZ5ZK53
[a18mcmc]: https://www.mcmaster.com/9579K73
[a19rw]:   display_panel
[a20mcmc]: https://www.mcmaster.com/5894K34
[a21mcmc]: https://www.mcmaster.com/9579K64
[a22amzn]: https://www.amazon.com/gp/product/B01F4BJ7PI
[a23mcmc]: https://www.mcmaster.com/2974K392
[a24rw]:   ../wiring
[a25amzn]: https://www.amazon.com/GTSE-Small-Strength-Nylon-Cable/dp/B086H4TR5Y

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../methods).**

[Pneumatics assembly model](internals.iam)

>**TODO:** drawings?

| Top | Bottom |
:------------------:|:-----------------:|
| [![](images/top_smaller.jpg)](images/top.jpg) | [![](images/bottom_smaller.jpg)](images/bottom.jpg) |

The rough order/hierarchy of internals assembly is as follows:

* Assemble the [equipment tray](tray) which will hold everything else (`[A1]` and `[A2]`)
* Install the [filter panel](filter_panel) to the equipment bracket
* Install the [blower assembly](blower) onto the pneumatics tray
* Attach the [air check valve](air_check_valve) to the blower assembly
* Install 2x [Pinch valves](pinch_valve), for air influx and exhale control
  * remember to label the pinch valve wiring "inhale" and "exhale" appropriately
* Install the [oxygen regulator assembly](regulator)
* Install the [proportional solenoid](PSOL) valve for oxygen control
* Install the main tubing, sensors and manifolds:
    * [Manifold](manifold) - for oxygen sensor and overpressure relief valve
    * [Oxygen mixer](mixer) - for mixing air and oxygen before delivery to patient
    * [Venturi flow sensors](venturi) - in-house design of affordable flow sensors
    * Various tubing adapters `[14-16]`
    * 3/4 tubing `[17]` and clamps `[18]`
* Install the [display panel](display_panel)
* Install oxygen line tubing `[A20]` and clamps `[21]`

### Sensor tubing

Sensor tubing `[A22-23]` connects venturis to pressure sensors on the main board.

>**TODO:** document sensor tube connections

### Final wiring

By this point you should have completed all the mechanical and pneumatic sub-assemblies of the ventilator.

You can now connect all the wiring (except main power):
* connect blower power wires to PCB
* connect blower control wires
  * to the interim board, if you have one
  * else, to the PCB
* connect proportional solenoid to PCB
* connect oxygen sensor to interim board
* connect the pinch valves to their driver pigtails as follows:
  * top - inhale
  * bottom - exhale
* Secure sensor tubing and wiring with zip ties `[A25]`

>**TODO:** do renderings of sub-systems and intermediate steps

>**TODO:** point out components in photos
