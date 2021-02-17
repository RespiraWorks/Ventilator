# Blower Sub-assembly

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](images/assembly_rendering.jpg) | ![](images/integrated.jpg) |

The blower/fan provides air pressure to the ventilator's pneumatic circuit. Currently this is only responsible for
delivery of ambient air, independent of the question of oxygen supply. In a full system that includes oxygen supply,
the blower is a subsystem to the overarching gas delivery control system.

Characterization tests for the blower are [here](characterization-tests).

## Enclosed assembly

![](exports/blower_assembly.jpg)

|Download Type|Link|
|:------|:-------|
|Assembly Drawing PDF|[PDF](exports/blower_assembly.pdf)|
|Autodesk Inventor CAD Assembly|[.iam](blower_assembly.iam)|

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                       | Price (USD)      | Sources[*][ppg] | Notes |
| ----- |---------:| ------------- | ---------------------------- | ----------------:|:---------------:|-------|
|**A1** | 1        |               | WS7040-12-X200N              | 65.00            | [A][a1ali]      | 12v blower and driver. Make sure to choose with driver when ordering. |
|**A2** | (1)      |               | WS1208                       | 24.00            | [W][a2won]      | **optional** blower driver, in case blower above did not come with driver. |
|**A3** | 1        | RespiraWorks  | tubing_adapter               | 1 (material)     | [Rw][a3rw]      | 3D printed from data in link |
|**A4** | ~200cm²  | McMaster-Carr | 8560K357                     | 55 / 3716cm²     | [C][a4mcmc]     | 1/4" (6.35mm) thick clear acrylic sheet, to make **A3-C5** below |
|**A5** | 1        | RespiraWorks  | driver_mounting_plate        | ~1.1 (material)  | [Rw][a5rw]      | Driver mounting plate cut from acrylic (**A2**) using drawing in link, uses ~75 cm² |
|**A6** | 1        | RespiraWorks  | blower_holding_plate         | ~0.9 (material)  | [Rw][a6rw]      | Blower holding plate cut from acrylic (**A2**) using drawing in link, uses ~60 cm² |
|**A7** | 1        | RespiraWorks  | tubing_adapter_holding_plate | ~0.9 (material)  | [Rw][a7rw]      | Tubing adapter holding plate cut from acrylic (**A2**) using drawing in link, uses ~60 cm² |
|**A8** | ~80cm²   | McMaster-Carr | 1370N13-1370N131             | 6.75 / 232cm²    | [C][a8mcmc]     | 1/16" durometer 30 neoprene gasket material, to make items **A7** and **A8** below |
|**A9** | 1        | RespiraWorks  | blower_gasket                | ~1.2 (material)  | [B][a9rw]       | Blower gasket cut from (**A6**) using drawing in link, uses ~40 cm² |
|**A10**| 1        | RespiraWorks  | tubing_adapter_gasket        | ~1.2 (material)  | [B][a10rw]      | Tubing adapter Gasket cut from (**A6**) using drawing in link, uses ~40 cm² |
|**A11**| 4        | McMaster-Carr | 93625A150                    | 6.04 / 100       | [C][a11mcmc]    | M4 lock nuts |
|**A12**| 12       | McMaster-Carr | 93475A230                    | 1.86 / 100       | [C][a12mcmc]    | M4 washers |
|**A13**| 4        | McMaster-Carr | 92095A168                    | 9.55 / 50        | [C][a13mcmc]    | M3 x 14mm button head screws, for blower driver |
|**A14**| 4        | McMaster-Carr | 91290A574                    | 15.45 / 10       | [C][a14mcmc]    | M4 x 85mm socket head screws |
|**A15**| 4        | McMaster-Carr | 99072A203                    | 1.21             | [C][a15mcmc]    | 10mm nylon spacers |
|**A16**| 4        | McMaster-Carr | 91828A231                    | 6.45 / 100       | [C][a16mcmc]    | M4 nuts (non locking) |
|**A17**| 4        | McMaster-Carr | 93625A100                    | 5.31 / 100       | [C][a17mcmc]    | M3 lock nuts |

[a1ali]:  https://www.aliexpress.com/item/32980201709.html
[a2won]:  https://wonsmart-motor.en.made-in-china.com/product/hsjxFewOppVg/China-Air-Pump12V-Brushless-Motor-12V-Blower-Fan-Driver.html
[a3rw]:    exports/tubing_adapter.stl
[a4mcmc]:  https://www.mcmaster.com/8560K357/
[a5rw]:    exports/driver_mounting_plate.dxf
[a6rw]:    exports/blower_holding_plate.dxf
[a7rw]:    exports/tubing_adapter_holding_plate.dxf
[a8mcmc]:  https://www.mcmaster.com/1370N13-1370N131/
[a9rw]:    exports/blower_gasket.dxf
[a10rw]:    exports/tubing_adapter_gasket.dxf
[a11mcmc]:  https://www.mcmaster.com/93625A150/
[a12mcmc]: https://www.mcmaster.com/93475A230/
[a13mcmc]: https://www.mcmaster.com/92095A168/
[a14mcmc]: https://www.mcmaster.com/91290A574/
[a15mcmc]: https://www.mcmaster.com/99072A203/
[a16mcmc]: https://www.mcmaster.com/91828A231/
[a17mcmc]: https://www.mcmaster.com/93625A100/

## Assembly

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

### Tubing adapter (C1)


|For 3D printing: [tubing_adapter.stl](exports/tubing_adapter.stl)| Inventor source: [tubing_adapter.ipt](tubing_adapter.ipt)|
|:---:|:---:|
|![](images/tubing_adapter_rendering.jpg)|[![](exports/tubing_adapter.jpg)](exports/tubing_adapter.pdf)|

### Gaskets

There are notes on manufacturing gaskets in the [manufacturing methods guide][mmg].

|     | Blower gasket (C9)         | Tubing adapter gasket (C10) |
|----:|:--------------------------:|:---------------------------:|
| PNG | ![](exports/blower_gasket.png) | ![](exports/tubing_adapter_gasket.png) |
| DXF |[blower_gasket.dxf](exports/blower_gasket.dxf)|[tubing_adapter_gasket.dxf](exports/tubing_adapter_gasket.dxf)|
| Inventor |[blower_gasket.ipt](blower_gasket.ipt)|[tubing_adapter_gasket.ipt](tubing_adapter_gasket.ipt)|

### Acrylic plates

You will have to cut the following mounting and holding plates from an acrylic sheet `[A4]`.

|     | Driver mounting plate (C5) | Blower holding plate (C6) | Tubing adapter holding plate (C7) |
|:-----:|:--------------------------:|:-------------------------:|:---------------------------------:|
|  |![](exports/driver_mounting_plate.jpg)|![](exports/blower_holding_plate.jpg)|![](exports/tubing_adapter_holding_plate.jpg)|
| Drawings |[Driver Mounting Plate](exports/driver_mounting_plate.pdf)|[Blower Holding Plate](exports/blower_holding_plate.pdf)|[Tubing Adapter Holding Plate](exports/tubing_adapter_holding_plate.pdf)|
| DXF |[driver_mounting_plate.dxf](exports/driver_mounting_plate.dxf)|[blower_holding_plate.dxf](exports/blower_holding_plate.dxf)|[tubing_adapter_holding_plate.dxf](exports/tubing_adapter_holding_plate.dxf)|
| Inventor | [driver_mounting_plate.ipt](driver_mounting_plate.ipt)|[blower_holding_plate.ipt](blower_holding_plate.ipt)| [tubing adapter_holding_plate.ipt](tubing_adapter_holding_plate.ipt)|

The above manufactured components (excluding gaskets) should appear as follows:

![](images/assembly_parts.jpg)

### Mount the blower driver

Use the following hardware:
* 4x M3 14mm screws `[A13]`
* 4x M3 lock nuts `[A17]`

Mount the blower driver `[A2]` to the Driver mounting plate `[A5]`. It is advisable to run the screws from the
bottom up, because the screw heads may be too wide to fit next to the circuit board.

![](images/blower_driver_mounting.jpg)

### Make blower sandwich

|                            |                             |
|:--------------------------:|:---------------------------:|
|Take the mounted blower driver from step above, and run four 85mm screws `[A14]` with M4 washers `[A12]` downwards. | Flip this assembly upside down. Put four spacers `[A15]` on the screws. Run the wiring of blower `[A1]` through hole in blower holding plate `[A6]`|
|![](images/blower_mounting.jpg) | ![](images/blower_mounting2.jpg) |

|                            |                             |
|:--------------------------:|:---------------------------:|
| Slide the blower holding plate over the screws so that wiring comes out through the space between the two plates, with blower on top. Place blower gasket `[A9]` on main enclosed ventilator assembly. | Carefully to not lose the screws, flip the blower sandwich and install it onto the main assembly. Fasten with m4 washers `[A12]` and regular non-locking nuts `[A16]`.|
|![](images/blower_mounting3.jpg) | ![](images/blower_mounting4.jpg) |

Tighten carefully one nut at a time, ensuring a tight gasket seal. Do not over-tighten. If you see the acrylic holding plate begin to bend, relax the fasteners somewhat.

### Install tubing adapter

You will likely need some sort of small clamps for this step, to ensure that components stay in place while you fasten them.

* Make sure the air filter assembly plate is (temporarily) fitted to the main ventilator assembly.
* Flip the ventilator assembly face-down, so that the bottom of the main assembly plate faces you vertically.
* Insert tubing adapter `[A3]` through the tubing adapter holding plate `[A7]` and place this assembly on the bottom side of the main mounting plate.
* Now insert the tubing adapter gasket `[A10]` in the gap, adjust its position to be centered on the blower inlet and apply pressure with the tubing adapter to hold it in place.
* Turn the tubing adapter so that the tubing barb faces the general direction of the air filter assembly that it will connect to.
* Temporarily clamp the entire blower assembly in place.
* Fasten with m4 washers `[A12]` and lock nuts `[A11]`.

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](images/tubing_adapter_holding_plate.jpg) | ![](images/integrated.jpg) |

## System integration

Integration into the pneumatic system is covered in either [pizza build](../../../quality-assurance/testing/pizza_build) or
[enclosed build pneumatics](..) pages, depending on which version you are building.

Integration into the electrical/controls system is covered in [wiring page](../../wiring).

## Price estimate

**#TODO:** out of date (used old BOMs), need to update this

**Basic assembly:**

65 (A1) + 0.70 (A3) + 1.20 (A4) + 0.22 (A5) + 0.18 (A6) = 67.30 USD

**Complete assembly for enclosure:**

67.30 (basic assembly) + 1 (C1) + 3 (C3-5) + 2.5 (C7-8) + 0.48 (C9) + 0.3 (C10) + 0.4 (C11) + 6.18 (C12) + 4.84 (C13)  +
0.26 (C14) + 1.2 (C15) + 0.28 (C16) + 1.2 (C18) + 1.3 (C19) + 0.22 (A5) + 0.18 (A6) = 90.64 USD
