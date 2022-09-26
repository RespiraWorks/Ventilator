# Blower Sub-assembly

|                                     |                            |
|:-----------------------------------:|:--------------------------:|
| ![](images/assembly_rendering.jpg)  | ![](images/integrated.jpg) |

The blower/fan provides air pressure to the ventilator's pneumatic circuit. Currently this is only responsible for
delivery of ambient air, independent of the question of oxygen supply. In a full system that includes oxygen supply,
the blower is a subsystem to the overarching gas delivery control system.

Characterization tests for the blower are [here](characterization-tests).

All Inventor assembly models and drawings are within this repo directory.

Recently exported files for manufacturing can be found on our Google Drive at
[Manufacturing/Output files v0.4](https://tinyurl.com/y4t5tz9f).

## Enclosed assembly

![](images/blower_assembly.jpg)

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item    |  Quantity | Manufacturer  | Part #                               | Price (USD) | Sources[*][ppg] | Notes                                                                                      |
|---------|----------:|---------------|--------------------------------------|------------:|:---------------:|--------------------------------------------------------------------------------------------|
| **A1**  |         1 |               | WS7040-12-X200N                      |       65.00 |   [A][a1ali]    | 12v blower and driver. Make sure to choose with driver when ordering.                      |
| **A2**  |       (1) |               | WS1208                               |       24.00 |   [W][a2won]    | **optional** blower driver, in case blower above did not come with driver.                 |
| **A3**  |         1 | RespiraWorks  | [Tubing adapter][a3rw]               |        1.00 |       Rw        | 3D printed from data in link                                                               |
| **A4**  | (~200cm²) | McMaster-Carr | 8560K357                             |  20.44 / m² |   [C][a4mcmc]   | 1/4" (6.35mm) thick clear acrylic sheet, to make **A5-7** below                            |
| **A5**  |         1 | RespiraWorks  | [Driver mounting plate][a5rw]        |        1.10 |       Rw        | Driver mounting plate cut from acrylic (**A4**) using drawing in link, uses ~75 cm²        |
| **A6**  |         1 | RespiraWorks  | [Blower holding plate][a6rw]         |        0.90 |       Rw        | Blower holding plate cut from acrylic (**A4**) using drawing in link, uses ~60 cm²         |
| **A7**  |         1 | RespiraWorks  | [Tubing adapter holding plate][a7rw] |        0.90 |       Rw        | Tubing adapter holding plate cut from acrylic (**A4**) using drawing in link, uses ~60 cm² |
| **A8**  |  (~80cm²) | McMaster-Carr | 1370N13-1370N131                     |   0.16 / m² |   [C][a8mcmc]   | 1/16" durometer 30 neoprene gasket material, to make items **A9** and **A10** below        |
| **A9**  |         1 | RespiraWorks  | [Blower gasket][a9rw]                |        1.20 |       Rw        | Blower gasket cut from (**A8**) using drawing in link, uses ~40 cm²                        |
| **A10** |         1 | RespiraWorks  | [Tubing adapter gasket][a10rw]       |        1.20 |       Rw        | Tubing adapter Gasket cut from (**A8**) using drawing in link, uses ~40 cm²                |
| **A11** |         4 | McMaster-Carr | 93625A150                            |        0.06 |  [C][a11mcmc]   | M4 lock nuts                                                                               |
| **A12** |        12 | McMaster-Carr | 93475A230                            |        0.02 |  [C][a12mcmc]   | M4 washers, 9mm OD                                                                         |
| **A13** |         4 | McMaster-Carr | 92095A168                            |        0.19 |  [C][a13mcmc]   | M3 x 14mm button head screws, for blower driver                                            |
| **A14** |         4 | McMaster-Carr | 91290A574                            |        1.55 |  [C][a14mcmc]   | M4 x 85mm socket head screws                                                               |
| **A15** |         4 | McMaster-Carr | 99072A203                            |        1.21 |  [C][a15mcmc]   | 10mm nylon spacers                                                                         |
| **A16** |         4 | McMaster-Carr | 91828A231                            |        0.06 |  [C][a16mcmc]   | M4 nuts (non locking)                                                                      |
| **A17** |         4 | McMaster-Carr | 93625A100                            |        0.05 |  [C][a17mcmc]   | M3 lock nuts                                                                               |

**Total assembly price:** USD 84.02

[a1ali]:   https://www.aliexpress.us/item/2251832793886957.html
[a2won]:   https://wonsmart-motor.en.made-in-china.com/product/hsjxFewOppVg/China-Air-Pump12V-Brushless-Motor-12V-Blower-Fan-Driver.html
[a3rw]:    #tubing-adapter-c1
[a4mcmc]:  https://www.mcmaster.com/8560K357/
[a5rw]:    #acrylic-plates
[a6rw]:    #acrylic-plates
[a7rw]:    #acrylic-plates
[a8mcmc]:  https://www.mcmaster.com/1370N13-1370N131/
[a9rw]:    #gaskets
[a10rw]:   #gaskets
[a11mcmc]: https://www.mcmaster.com/93625A150/
[a12mcmc]: https://www.mcmaster.com/93475A230/
[a13mcmc]: https://www.mcmaster.com/92095A168/
[a14mcmc]: https://www.mcmaster.com/91290A574/
[a15mcmc]: https://www.mcmaster.com/99072A203/
[a16mcmc]: https://www.mcmaster.com/91828A231/
[a17mcmc]: https://www.mcmaster.com/93625A100/

## Assembly

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

Recently exported drawings for sub-assemblies can be found on our Google Drive at
[Manufacturing/Output files v0.4/Subassemblies](https://tinyurl.com/4d3s7zmw).

### Tubing adapter (C1)

Recently exported files for 3d printing can be found on our Google Drive at
[Manufacturing/Output files v0.4/3d printing](https://tinyurl.com/2kuwps9h).

|                                          |                                |
|:----------------------------------------:|:------------------------------:|
| ![](images/tubing_adapter_rendering.jpg) | ![](images/tubing_adapter.jpg) |

### Gaskets

Recently exported files for making gaskets can be found on our Google Drive at
[Manufacturing/Output files v0.4/Gaskets](https://tinyurl.com/yc8cx38z).

|     Blower gasket `[A9]`      |     Tubing adapter gasket `[A10]`     |
|:-----------------------------:|:-------------------------------------:|
| ![](images/blower_gasket.png) | ![](images/tubing_adapter_gasket.png) |

### Acrylic plates

You will have to cut the following mounting and holding plates from an acrylic sheet `[A4]`.

Recently exported files for cutting acrylic can be found on our Google Drive at
[Manufacturing/Output files v0.4/Acrylic](https://tinyurl.com/4czevhxs).

|     Driver mounting plate `[A5]`      |     Blower holding plate `[A6]`      |         Tubing adapter holding plate `[A7]`          |
|:-------------------------------------:|:------------------------------------:|:----------------------------------------------------:|
| ![](images/driver_mounting_plate.jpg) | ![](images/blower_holding_plate.jpg) | ![](images/tubing_adapter_holding_plate_drawing.jpg) |

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
