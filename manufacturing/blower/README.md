# Blower Sub-assembly

![](images/basic_assembly.jpg)

The blower/fan is what provides air pressure to the ventilator's pneumatic circuit.

Characterization tests for the blower are [here](characterization-tests).

**#TODO:** unit price analysis

## Basic assembly

This entails the bare essentials for a functional testing setup, such as a [pizza build](../pizza_build).
The pneumatic aspect of the blower assembly includes the blower and a short segment of tubing compatible with blower
outlet diameter. The electrical aspect of the blower assembly encompasses the driver board and wires to interface
with the [PCB](../../pcb).

**Note: If you are a member of the RespiraWorks team, review the
[part purchasing guidelines](../../manufacturing/README.md#part-purchasing-guidelines)
BEFORE purchasing any parts.**

### Purchasing Source Key

* **A** = Ali Express
* **C** = McMaster-Carr
* **W** = Wonsmart
* **K** = Digikey
* **B** = Built by RespiraWorks
* **Z** = Amazon
* **R** = RS computing
(**note**: links provided are for worldwide customers; there are local RS retail sites for most countries that use the
same product reference (RS Stock No.) and will probably ship faster if you order from your country's site)

### Parts

| Item | Quantity | Manufacturer  | Part #                | Price (USD)  |Sources                | Notes |
| ---- |---------:| ------------- | --------------------- | ------------:|:---------------------:|-------|
| a1   |        1 |               | WS7040-12-X200N       | 65.00        | [A][a1ali]             | 12v blower and driver. Make sure to choose with driver when ordering. |
| a2   |        1 |               | WS1208                | 24.00        | [W][a2won]             | blower driver, in case blower above did not come with driver. |
| a3   |   ~50 mm | McMaster-Carr | 5234K57               | 21.50 / 5ft  | [C][a3mcmc]            | Soft latex tubing, 5/8" ID, 7/8" OD |
| a4   |        2 | McMaster-Carr | 9579K72               | 12.21 / 20   | [C][a4mcmc]            | Herbie clip J (20.3-23mm) for clamping 7/8"OD tubing |
| a5   |   2 or 4 | JST           | SVH-21T-P1.1          | 0.11         | [K][a5digi] [R][a5rs]   | VH crimps, for power connector |
| a6   |   1 or 2 | TE            | 1-1123722-2           | 0.18         | [K][a6digi] [R][a6rs] | VH housing 2 pin, for power connector |

[a1ali]:  https://www.aliexpress.com/item/32980201709.html
[a2won]:  https://wonsmart-motor.en.made-in-china.com/product/hsjxFewOppVg/China-Air-Pump12V-Brushless-Motor-12V-Blower-Fan-Driver.html
[a3mcmc]: https://www.mcmaster.com/5234K57
[a4mcmc]: https://www.mcmaster.com/9579K72
[a5digi]: https://www.digikey.com/short/z44fjr
[a5rs]:   https://export.rsdelivers.com/product/jst/svh-41t-p11/jst-nv-vh-female-crimp-terminal-contact-16awg-svh/7620692
[a6digi]: https://www.digikey.com/short/z44fwj
[a6rs]:   https://export.rsdelivers.com/product/jst/vhr-2n/jst-vhr-female-connector-housing-396mm-pitch-2/8201172

**Please note:** Quantities of items a5+a6 depend on whether you intend to integrate the blower into an enclosed
ventilator assembly as [described below](#enclosed-assembly).

### Tools

| Item | Quantity | Manufacturer  | Part #              | Price (USD) | Sources         | Notes |
| ---- |---------:| ------------- | ------------------- | -----------:|-----------------| ----- |
| b1   |        1 | Preciva       | PR-3254 (PR102001)  | 39.99       | [Z][b1amzn]     | Crimping tool for HX and HV connectors |

[b1amzn]:https://www.amazon.com/gp/product/B07R1H3Z8X

**Note: Items a5, a6 and b1 above are duplicated in the [wiring BOM](../../design/electrical-system/wiring.md)**

### Instructions for basic assembly

Attach a short segment of tubing (**a3**) to blower (**a1**) outlet.

Use Herbie clip (**a4**) to secure tubing to blower outlet. Alternatively, you can use a zip-tie.

You will also have to crimp connectors (**a5 + a6**) to the second end of the wiring (using **b1**) that connects the
driver board to the [PCB](../../pcb).

A basic assembly is pictured at the top of this page.

**Please note:** the picture above includes an optional tubing reducer that is specified on the [pizza build](../pizza_build) page.

## Enclosed assembly

The basic assembly needs further modifications to be mounted into an [enclosed ventilator assembly](../enclosed_build).
The pneumatic interface includes gaskets and adapter for tubing that runs to the filtered air inlet on the front panel.
The mechanical aspect of this assembly includes all hold-down and mounting plates for the blower and blower
driver board, and for the above pneumatic interface components. Electrically, the enclosed assembly also requires longer
wiring.

### Enclosed assembly parts

| Item | Quantity | Manufacturer  | Part #                       | Price (USD)      |Sources                  | Notes |
| ---- |---------:| ------------- | ---------------------------- | ----------------:|:-----------------------:|-------|
| c1   |        1 | RespiraWorks  | tubing_adapter               | 1 (material)     | [B][c1rw]               | 3D printed from data in link |
| c2   | ~200cm²  | McMaster-Carr | 8560K357                     | 54.68 / 3716cm²  | [C][c2mcmc]             | 1/4" (~6mm) thick clear acrylic sheet, to make **b3-b5** below |
| c3   |        1 | RespiraWorks  | tubing_adapter_holding_plate | ~0.9 (material)  | [B][c3rw]               | Acrylic (**c2**) cut using drawing in link, uses ~60 cm² |
| c4   |        1 | RespiraWorks  | driver_mounting_plate        | ~1.1 (material)  | [B][c4rw]               | Acrylic (**c2**) cut using drawing in link, uses ~75 cm² |
| c5   |        1 | RespiraWorks  | blower_holding_plate         | ~0.9 (material)  | [B][c5rw]               | Acrylic (**c2**) cut using drawing in link, uses ~60 cm² |
| c6   | ~100cm²  | McMaster-Carr | 8785K82-8785K822             | 22.22 / 929cm²   | [C][c6mcmc]             | Gasket material, to make items **b6+b8** below |
| c7   |        1 | RespiraWorks  | blower_gasket                | ~1.2 (material)  | [B][c7rw]               | Gasket material (**c6**) cut using drawing in link, uses ~50 cm² |
| c8   |        1 | RespiraWorks  | tubing_adapter_gasket        | ~1.2 (material)  | [B][c8rw]               | Gasket material (**c6**) cut using drawing in link, uses ~50 cm² |
| c9   |        8 | McMaster-Carr | 93625A150                    | 6.04 / 100       | [C][c9mcmc]             | M4 nylon insert lock nuts |
| c10  |       16 | McMaster-Carr | 93475A230                    | 1.86 / 100       | [C][c10mcmc]            | M4 washers |
| c11  |        4 | McMaster-Carr | 92000A224                    | 9.60 / 100       | [C][c11mcmc]            | M4 x 14mm Philips head screws, for blower driver |
| c12  |        4 | McMaster-Carr | 91290A574                    | 15.45 / 10       | [C][c12mcmc]            | M4 x 85 mm hex head screws |
| c13  |        4 | McMaster-Carr | 93657A079                    | 1.21             | [C][c13mcmc]            | 10mm nylon spacers |
| c14  |        4 | McMaster-Carr | 91828A231                    | 6.45 / 100       | [C][c14mcmc]            | M4 nut |
| c15  |       12 | JST           | SXH-001T-P0.6                | 0.10             | [K][c15digi] [R][c15rs] | XH crimps, for control connector |
| c16  |        2 | JST           | XHP-6                        | 0.14             | [K][c16digi] [R][c16rs] | XH 6-pin housing, for control connector |
| c17  |        - | CQRobot       | CQRJST254-BGB                | 9.99             | [Z][c17amzn]            | XH connector kit for 6/7/8 pins, alternative for b15+b16 above |
| c18  |      ~1m | Remington     | 16UL1007STRKIT               | 29.70 / 25m      | [K][c18amzn]            | 16 AWG wire, for power cable, ~42cm per wire |
| c19  |      ~3m | Remington     | 26UL1007STRKIT10COLOR        | 31.60 / 75m      | [K][c19amzn]            | 26 AWG wire, for control cable, ~42cm per wire  |

[c1rw]:    CAD/tubing_adapter.stl
[c2mcmc]:  https://www.mcmaster.com/8560K357/
[c3rw]:    CAD/tubing_adapter_holding_plate.dxf
[c4rw]:    CAD/driver_mounting_plate.dxf
[c5rw]:    CAD/blower_holding_plate.dxf
[c6mcmc]:  https://www.mcmaster.com/8785K82-8785K822/
[c7rw]:    CAD/blower_gasket.dxf
[c8rw]:    CAD/tubing_adapter_gasket.dxf
[c9mcmc]:  https://www.mcmaster.com/93625A150/
[c10mcmc]: https://www.mcmaster.com/93475A230/
[c11mcmc]: https://www.mcmaster.com/92000A224/
[c12mcmc]: https://www.mcmaster.com/91290A574/
[c13mcmc]: https://www.mcmaster.com/93657A079/
[c14mcmc]: https://www.mcmaster.com/91828A231/
[c15digi]: https://www.digikey.com/short/z44f8d
[c15rs]:   https://export.rsdelivers.com/product/jst/sxh-002t-p06/jst-xh-female-crimp-terminal-contact-26awg-sxh/1755645
[c16digi]: https://www.digikey.com/short/z44ff8
[c16rs]:   https://export.rsdelivers.com/product/jst/xhp-6/jst-xhp-female-connector-housing-25mm-pitch-6-way/8201620
[c17amzn]: https://www.amazon.com/gp/product/B079MJ1RYN
[c18amzn]: https://www.amazon.com/gp/product/B00N51OJJ4
[c19amzn]: https://www.amazon.com/gp/product/B011JC76OA

**NOTE: Items **c15-c19** above are also duplicated in the [wiring BOM](../../design/electrical-system/wiring.md)**

### Custom component manufacture

#### Tubing adapter

To 3d print this component see the [general 3d printing guidelines](../3d_printing).

| Rendering      | STL        | Source |
|:--------------------------:|:---------------------------:|:----------------------------:|
|![](CAD/tubing_adapter.jpg)|[tubing_adapter.stl](CAD/tubing_adapter.stl)|[tubing_adapter.SLDPRT](CAD/tubing_adapter.SLDPRT)|

#### Gaskets

You will want to use an Exacto knife to cut these from the gasket material (**c6**).

|    | Blower gasket              | Tubing adapter gasket       |
|---:|:--------------------------:|:---------------------------:|
|PNG |![](CAD/blower_gasket.png)|![](CAD/tubing_adapter_gasket.png)|
|DXF |[blower_gasket.dxf](CAD/blower_gasket.dxf)|[tubing_adapter_gasket.dxf](CAD/tubing_adapter_gasket.dxf)|

#### Acrylic plates

You will have to cut the following mounting and holding plates from an acrylic sheet (**c2**).

You will likely need some of the following tools:
* jigsaw or laser cutter for straight cuts
* drill press
* sandpaper to clean up edges
* 4.5mm (11/64") drill bit for holes (make them slightly larger than specified in drawings)
* 30mm (1-1/4") Forstner bit or hole cutter
* 37mm (1-1/2") Forstner bit or hole cutter

|    | Driver mounting plate      | Blower holding plate        | Tubing adapter holding plate |
|---:|:--------------------------:|:---------------------------:|:----------------------------:|
|PNG |![](CAD/driver_mounting_plate.png)|![](CAD/blower_holding_plate.png)|![](CAD/tubing_adapter_holding_plate.png)|
|DXF |[driver_mounting_plate.dxf](CAD/driver_mounting_plate.dxf)|[blower_holding_plate.dxf](CAD/blower_holding_plate.dxf)|[tubing_adapter_holding_plate.dxf](CAD/tubing_adapter_holding_plate.dxf)|

You can use one of these plates as a template to drill the appropriate holes in the main acrylic plate and the metal
fame that supports it.

The above manufactured components (excluding gaskets) should appear as follows:

![](images/assembly_parts.jpg)

### Assembly instructions

#### Mount the blower driver

Use:
* 4x M4 lock nuts (**c9**)
* 4x M4 washers (**c10**)
* 4x M4 14mm screws (**c11**)

Mount the blower driver (**a2**) to the Driver mounting plate (**c4**). It is advisable to run the screws from the
bottom up, because the screw heads may be too wide to fit next to the metal base of the driver board.

![](images/blower_driver_mounting.jpg)

#### Make blower sandwich

|                            |                             |
|:--------------------------:|:---------------------------:|
|Take the mounted blower driver from step above, and run four 85mm screws (**c12**) with m4 washers (**c10**) downwards. | Flip this assembly upside down. Put four spacers (**c13**) on the screws. Run the wiring of blower (**a1**) through hole in blower holding plate (**c5**)|
|![](images/blower_mounting.jpg) | ![](images/blower_mounting2.jpg) |

|                            |                             |
|:--------------------------:|:---------------------------:|
| Slide the blower holding plate over the screws so that wiring comes out through the space between the two plates, with blower on top. Place blower gasket (**c7**) on main enclosed ventilator assembly. | Carefully to not lose the screws, flip the blower sandwich and install it onto the main assembly. Fasten with m4 washers (**c10**) and regular non-locking nuts (**c14**).|
|![](images/blower_mounting3.jpg) | ![](images/blower_mounting4.jpg) |

Tighten carefully one nut at a time, ensuring a tight gasket seal. Do not over-tighten. If you see the acrylic holding plate begin to bend, relax the fasteners somewhat.

#### Install tubing adapter

You will likely need some sort of small clamps for this step, to ensure that components stay in place while you fasten them.

* Make sure the air filter assembly plate is (temporarily) fitted to the main ventilator assembly.
* Flip the ventilator assembly face-down, so that the bottom of the main assembly plate faces you vertically.
* Insert tubing adapter (**c1**) through the tubing adapter holding plate (**c3**) and place this assembly on the bottom side of the main mounting plate.
* Now insert the tubing adapter gasket (**c8**) in the gap, adjust its position to be centered on the blower inlet and apply pressure with the tubing adapter to hold it in place.
* Turn the tubing adapter so that the tubing barb faces the general direction of the air filter assembly that it will connect to.
* Temporarily clamp the entire blower assembly in place.
* Fasten with m4 washers (**c10**) and lock nuts (**c9**).

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](images/tubing_adapter_holding_plate.jpg) | ![](images/integrated.jpg) |

#### Manufacture extended wiring

The power and signal cables that come included with the blower are too short to reach the PCB, so new ones must be
manufactured using **a5, a6, and c15-c19**. Please follow the old cables included with blower as examples as well as
any advice on the [wiring assembly page](../../design/electrical-system/wiring.md).

The new cables should each be about 42cm in length.

**#TODO:** Some sort of shrink tubing should also be used

![](images/wired.jpg)
