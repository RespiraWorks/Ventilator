# Internal assembly

|  |  |
:------------------:|:-----------------:|
| [![](images/rendering_smaller.jpg)](images/rendering_full.jpg)  | [![](images/open_back_smaller.jpg)](images/open_back.jpg) |

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md

| Item  | Quantity | Manufacturer   | Part #                | Price (USD)  | Sources[*][ppg]| Notes |
| ----- |---------:| -------------- | --------------------- | ------------:|:--------------:| ----- |
|**A1** | (1)      | RespiraWorks   | equipment_bracket     |              | [Rw][a1rw]     | Metal bracket holding the internal assembly together **from enclosure assemebly** |
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
|**A13**| 1        | RespiraWorks   | oxgen_mixer           |              | [Rw][a13rw]    | Oxygen mixing chamber |
|**A14**| 1        | RespiraWorks   | reducer_3-4_1-4       |              | [Rw][a14rw]    | 1/4"ID x 3/4"ID reducer |
|**A15**| 5        | McMaster-Carr  | 53415K153             | 13.16 / 10   | [C][a15mcmc]   | 3/4"ID barbed elbow |
|**A16**| 1        | McMaster-Carr  | 5463K648              | 8.40 / 10    | [C][a16mcmc]   | 3/4"ID x 3/8"ID barbed reducer |
|**A17**| ~1m      | OCSParts       | 117062-5              | 22.99 / 5ft  | [Z][a17amzn]   | Soft 3/4"ID 1"OD tubing, for main line tubing |
|**A18**| 24       | McMaster-Carr  | 9579K73               | 12.24 / 20   | [C][a18mcmc]   | Herbie clip K (22.2-24.9mm) for clamping 1"OD, main tubing |
|**A19**| 1        | RespiraWorks   | display_panel         |              | [Rw][a19rw]    | Display panel and electronics assembly |
|**A20**| ~1m      | McMaster-Carr  | 5894K34               | 12.15 / 5ft  | [C][a20mcmc]   | 1/4"ID 3/8"OD Tygon tubing, for oxygen line tubing |
|**A21**| 6        | McMaster-Carr  | 9579K64               | 9.90 / 20    | [C][a21mcmc]   | Herbie clip BB (9.2-10.3mm) for clamping 3/8"OD, oxygen line tubing |
|**A22**| ~1m      | uxcell         | a16031400ux0163       | 6.49 / 16ft  | [Z][a22amzn]   | 2.5 mm tubing for connecting venturis to pressure sensors |
|**A23**| 1        | McMaster-Carr  | 5463K4                | 7.39 / 10    | [C][a23mcmc]   | sensing tube 3/23" t-junction, for teeing off to patient pressure sensor |

[a1rw]:    ../enclosure
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
[a23mcmc]: https://www.mcmaster.com/5463K44

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../methods).**

[Pneumatics assembly model](internals.iam)

>**TODO:** drawings?

|  |  |
:------------------:|:-----------------:|
| [![](images/top_no_display_smaller.jpg)](images/top_no_display.jpg) | [![](images/botton_no_display_smaller.jpg)](images/botton_no_display.jpg) |

The rough order/hierarchy of assembly is as follows:

* Equipment bracket -- from the [enclosure assembly](../enclosure)
* [Pneumatics tray](tray) - for mounting the main pneumatics assembly
* [Filter panel](filter_panel) - mechanical support for air filter assemblies
    * [Air filter holders](filter_panel/filter_holder) - custom housings for air filters
* [Blower](blower) - this provides pressurized air
* [Air check valve](air_check_valve) - prevents concentrated oxygen back-flow
* [Pinch valves](pinch_valve) - air influx and exhale control valves
* [Oxygen regulator](regulator) - oxygen pressure regulator sub-assembly
* [Proportional solenoid](PSOL) - oxygen control valve
* Main tubing, sensors and manifolds:
    * [Manifold](manifold) - for oxygen sensor and overpressure relief valve
    * [Oxygen mixer](mixer) - for mixing air and oxygen before delivery to patient
    * [Venturi flow sensors](venturi) - in-house design of affordable flow sensors
    * Various tubing adapters `[14-16]`
    * 3/4 tubing `[17]` and clamps `[18]`
* [Display panel](display_panel) - touchscreen, electronics and oxygen input
    * [Electronics assembly](display_panel/electronics) - encompasses the electrical and computing components
* Oxygen line tubing `[A20]` and clamps `[21]`
* Sensor tubing `[A22-23]`, connecting venturis to pressure sensors on mainboard

>**TODO:** document sensor tube connections

>**TODO:** do renderings of sub-systems and intermediate steps

>**TODO:** point out components in photos
