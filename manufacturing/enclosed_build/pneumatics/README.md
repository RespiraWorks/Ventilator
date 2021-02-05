# Pneumatic assembly

This page documents the pneumatic assembly, which consists of the following components (colorized):

![](images/oxygen_diagram.jpg)

## Materials

### Custom components

Some components do not exist as off-the-shelf products, or would be too cumbersome to fit in the assembly. All of the
following can be manufactured, using some 3d printing or, eventually, could be mass produced as injection-molded
plastics.

Please see the [3d printing guidelines](../../3d_printing) for general advice on printing components.

* Pneumatics tray (**TODO**)
* [Blower assembly](blower)
* 3x [Venturi flow sensors](venturi) - one of each variant
* 2x [Pinch valves](pinch_valve)
* [Tubing reducer](reducer_3-4_1-4)
* [Oxygen mixing chamber](mixer)
* [Oxygen sensor and overpressure relief valve manifold](manifold)

The above components are also listed in the BOM below.

### Bill of Materials

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../../manufacturing/purchasing_guidelines.md

| Item  | Quantity | Manufacturer   | Part #              | Price (USD)  | Sources[*][ppg]| Notes |
| ----- |---------:| -------------- | ------------------- | ------------:|:--------------:| ----- |
|**A1** | 1        | McMaster-Carr  | 7768K22             | 11.62        | [C][a1mcmc]    | brass threaded check valve |
|**A2** | 1        | McMaster-Carr  | 50785K273           | 8.23         | [C][a2mcmc]    | through-wall straight connector, 1/4NPT female |
|**A3** | 1        | SMC Pneumatics | IR2010-N02B         | 77.62        | [Sm][a3smc]    | precision pressure regulator |
|**A4** | 1        | SMC Pneumatics | PVQ31-6G-40-01N     | 96.10        | [Sm][a4smc]    | Proportional solenoid |
|**A5** | 2        | McMaster-Carr  | 5121K391            | 5.58 / 10    | [C][a5mcmc]    | 1/8 NPT x 1/4" ID barbed adapter **(with B1 only)** |
|**A6** |(1)       | Uflow          | IBV19M              | 87.00        | [U][a6uflow]   | Proportional solenoid **(alternate for B1)** |
|**A7** |(2)       | McMaster-Carr  | 1786N249            | 8.16         | [C][a7mcmc]    | 1/8 BSPT x 1/4 NTP brass threaded pipe fitting **(with B3 only)** |
|**A8** |(2)       | McMaster-Carr  | 5121K451            | 5.78 / 10    | [C][a8mcmc]    | 1/4 NPT x 1/4" ID barbed adapter **(with B3 only)** |
|**A9** | 2        | McMaster-Carr  | 92095A182           | 8.38 / 100   | [C][a9mcmc]    | M3 x 10mm screws, button head hex drive, for PSOL mounting to assembly plate |
|**A10**| 2        | McMaster-Carr  | 91116A120           | 2.77 / 100   | [C][a10mcmc]   | M3 washers, 9mm OD, for PSOL mounting to assembly plate |
|**A11**| 1        | RespiraWorks   | reducer_3-4_1-4     |              | [Rw][a11rw]    | 1/4"ID x 3/4"ID reducer |
|**A12**| 1        | RespiraWorks   | venturi_oxygen_influx |            | [Rw][a12rw]    | Venturi flow sensor, oxygen influx variant |
|**A13**| 1        | RespiraWorks   | oxgen_mixer         |              | [Rw][a13rw]    | Oxygen mixing chamber |
|**A14**| 1        | RespiraWorks   | manifold            |              | [Rw][a14rw]    | Oxygen sensor and overpressure relief valve manifold |
|**A15**| 1        | Cables&Sensors | G0-040              | 76.00        | [Cs][a15cns]   | Oxygen sensor |
|**A16**| 1        | Generant       | VRVI-250B-V-0.8     | 54.00        | [Gn][a16gene]  | Overpressure relief valve |
|**A17**| ~2ft     | OCSParts       | 117062-5            | 22.99 / 5ft  | [Z][a17amzn]   | Soft 3/4"ID 1"OD tubing, for main line tubing |
|**A18**| ~50 mm   | McMaster-Carr  | 5234K57             | 21.50 / 5ft  | [C][a18mcmc]   | Soft latex tubing, 5/8" ID, 7/8" OD, for blower |
|**A19**| 2        | McMaster-Carr  | 9579K72             | 12.21 / 20   | [C][a19mcmc]   | Herbie clip J (20.3-23mm) for clamping 7/8"OD, blower tubing |
|**A20**| 6        | McMaster-Carr  | 9579K73             | 12.24 / 20   | [C][a20mcmc]   | Herbie clip K (22.2-24.9mm) for clamping 1"OD, main tubing |
|**A21**| ~3ft     | uxcell         | a16031400ux0163     | 6.49 / 16ft  | [Z][a21amzn]   | 2.5 mm tubing for connecting venturis to pressure sensors |
|**A22**| 1        | McMaster-Carr  | 5463K4              | 7.39 / 10    | [C][a22mcmc]   | sensing tube 3/23" t-junction, for teeing off to patient pressure sensor |
|**A23**| 1        | McMaster-Carr  | 5492K13             | 12.88        | [C][a23mcmc]   | Plastic threaded check valve, 1/2NPT |
|**A24**| 1        | McMaster-Carr  | 5372K128            | 4.89 / 10    | [C][a24mcmc]   | Barbed tube fitting, 5/8"ID x 1/2NPT |
|**A25**| 1        | McMaster-Carr  | 53055K434           | 5.95         | [C][a25mcmc]   | Barbed tube fitting, 3/8"ID x 1/2NPT |
|**A26**| 2        | RespiraWorks   | pinch_valve_assembly |             | [Rw][a26rw]    | Proportional pinch valve sub-assembly with tubing |
|**A27**| 1        | RespiraWorks   | venturi_air_influx  |              | [Rw][a27rw]    | Venturi flow sensor, air influx variant |
|**A28**| 1        | RespiraWorks   | venturi_exhale      |              | [Rw][a28rw]    | Venturi flow sensor, exhale variant |
|**A29**| ~3ft     | McMaster-Carr  | 5894K34             | 12.15 / 5ft  | [C][a29mcmc]   | 1/4"ID 3/8"OD Tygon tubing, for oxygen line tubing |
|**A30**| 5        | McMaster-Carr  | 5121K451            | 5.78 / 10    | [C][a30mcmc]   | 1/4 NPT x 1/4" ID barbed adapter |
|**A31**| 1        | RespiraWorks   | blower_assembly     |              | [Rw][a31rw]    | Blower mechanical sub-assembly |
|**A32**| 4        | McMaster-Carr  | 9579K67             |  12.63 / 20  | [C][a32mcmc]   | Herbie clip E, 12-13.8mm, to clamping 5/8"OD, pinch valve tubing |
|**A33**| 5        | McMaster-Carr  | 53415K153           |  13.16 / 10  | [C][a33mcmc]   | 3/4"ID barbed elbow |
|**A34**| 2        | McMaster-Carr  | 5121K907            |  8.15 / 10   | [C][a34mcmc]   | 3/8"ID barbed elbow |
|**A35**| 1        | McMaster-Carr  | 5463K648            |  8.40 / 10   | [C][a35mcmc]   | 3/4"ID x 3/8"ID barbed reducer |

[a1mcmc]:  https://www.mcmaster.com/7768K22/
[a2mcmc]:  https://www.mcmaster.com/50785K273/
[a3smc]:   https://www.smcpneumatics.com/IR2010-N02B.html
[a4smc]:   https://www.smcpneumatics.com/PVQ31-6G-40-01N.html
[a5mcmc]:  https://www.mcmaster.com/5121K391/
[a6uflow]: https://www.uflowvalve.com/solenoid-proportional-flow-control-valve/product/2-2-way-miniature-high-flow-proportional-valve
[a7mcmc]:  https://www.mcmaster.com/1786N249/
[a8mcmc]:  https://www.mcmaster.com/5121K451/
[a9mcmc]:  https://www.mcmaster.com/92095A182/
[a10mcmc]: https://www.mcmaster.com/91116A120/
[a11rw]:   reducer_3-4_1-4
[a12rw]:   venturi
[a13rw]:   mixer
[a14rw]:   manifold
[a15cns]:  https://www.cablesandsensors.com/products/compatible-o2-cell-for-hamilton-medical-396008
[a16gene]: https://www.generant.com/product/vent-relief-valve/
[a17amzn]: https://www.amazon.com/gp/product/B01LZ5ZK53
[a18mcmc]: https://www.mcmaster.com/5234K57
[a19mcmc]: https://www.mcmaster.com/9579K72
[a20mcmc]: https://www.mcmaster.com/9579K73
[a21amzn]: https://www.amazon.com/gp/product/B01F4BJ7PI
[a22mcmc]: https://www.mcmaster.com/5463K44
[a23mcmc]: https://www.mcmaster.com/5492K13/
[a24mcmc]: https://www.mcmaster.com/5372K128/
[a25mcmc]: https://www.mcmaster.com/53055K434/
[a26rw]:   pinch_valve
[a27rw]:   venturi
[a28rw]:   venturi
[a29mcmc]: https://www.mcmaster.com/5894K34
[a30mcmc]: https://www.mcmaster.com/5121K451
[a31rw]:   blower
[a32mcmc]: https://www.mcmaster.com/9579K67
[a33mcmc]: https://www.mcmaster.com/53415K153/
[a34mcmc]: https://www.mcmaster.com/5121K907/
[a35mcmc]: https://www.mcmaster.com/5463K648

**TODO: review adapter and clip counts**

### Tools

| Item | Manufacturer | Part number             | Price (USD) | Sources[*][ppg] | Notes |
| ---- |-------------:| ----------------------- | ----------- |:---------------:|------ |
|**B1**| TEKTON       | 37122                   | 13.00       | [Z][b1amzn]     | 8 inch slip-joint pliers, for clamping tubes onto fittings |
|**B2**| Duda Energy  | LPpvc075-005ft          | 19.99       | [Z][b2amzn]     |**Adjustable wrench, at least 1" wide opening, for screwing in NTP fittings |
|**B3**| DEWILL       | (unknown)               | 15.88       | [Z][b3amzn]     | Pipe and PVC cutter, for cutting tubing. Scissors might also work. |
|**B4**| IRWIN        | 2078216                 | 8.90        | [Z][b4amzn]     | Long nose pliers, for removing herbie clips |
|**B5**| Vanguard     | vanguardSealantsPTFE-01 | 6.49        | [Z][b5amzn]     | It's advisable to teflon tape all NTP threaded components. Not only will they be more airtight, but also they will be easier to tighten and release. |

[b1amzn]: https://www.amazon.com/TEKTON-2-Inch-Joint-Pliers-37122/dp/B00KLY1FAY
[b2amzn]: https://www.amazon.com/GETUPOWER-10-Inch-Adjustable-Opening-Vanadium/dp/B07RGV2VK1
[b3amzn]: https://www.amazon.com/gp/product/B07Y997XKC
[b4amzn]: https://www.amazon.com/Tools-VISE-GRIP-Pliers-6-Inch-2078216/dp/B000A0OW2M
[b5amzn]: https://www.amazon.com/gp/product/B01L2F428C

## Assembly instructions

### Pneumatics tray

[Pneumatics tray](pneumatics_tray.ipt)

**TODO:** Make this look good and explain manufacturing process

### Oxygen input and check valve

The oxygen check valve is installed on the upper acrylic panel. There is a separate page about the
[display panel sub-assembly](../display_panel).

The oxygen input assembly also requires a 1/4"ID tubing adapter (**A24**) to interface with tubing.

It is not clear whether the check valve has the correct cracking pressure and other pneumatic properties for our system.

Related issues are being tracked here:
[#653](https://github.com/RespiraWorks/Ventilator/issues/653)
[#646](https://github.com/RespiraWorks/Ventilator/issues/646)

### Oxygen regulator

The oxygen regulator requires 2x of 1/4"ID tubing adapters (**A24**) to interface with tubing.

### Proportional solenoid valve

Supply of oxygen is controlled by a proportional solenoid. We are currently testing with 2 different solenoids.
Parts for both variants are listed in BOM above.

Note that mounting the Uflow solenoid may require tapping of M3 threads in the aluminium manifold.

**ATTENTION: tools for tapping not listed here.**

### Oxygen sensor

**TODO:** Short description of the type of sensor this fundamentally is.

### Blower assembly

The pneumatic aspect of the blower assembly includes the blower and a short segment of tubing compatible with blower
outlet diameter.

Attach a short segment of tubing (**A18**) to blower (**A31**) outlet.

Use Herbie clip (**A19**) to secure tubing to blower outlet.

### Blower check valve

A check valve following the blower should be included with any build where oxygen is used.

**TODO:** explain choice and assembly

### Full Assembly

**TODO:** document sensor tube connections

**TODO:** point out components in photo

![](images/pneumatic_assembly.jpg)
