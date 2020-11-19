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

**NOTE:** Before you buy anything on this list, check with Edwin, Martin or Ethan. Blower takes a while to deliver.
We may have parts to send you from our inventory.


| Item | Quantity | Manufacturer  | Part #                | Price (USD)  |Sources                | Notes |
| ---- |---------:| ------------- | --------------------- | ------------:|:---------------------:|-------|
| 1    |        1 |               | WS7040-12-X200N       | 65.00        | [A][1ali]             | 12v blower and driver. Make sure to choose with driver when ordering. |
| 2    |        1 |               | WS1208                | 24.00        | [W][2won]             | blower driver, in case blower above did not come with driver. |
| 3    |   ~50 mm | McMaster-Carr | 5234K57               | 21.50 / 5ft  | [C][3mcmc]            | Soft latex tubing, 5/8" ID, 7/8" OD |
| 4    |        2 | McMaster-Carr | 9579K72               | 12.21 / 20   | [C][4mcmc]            | Herbie clip J (20.3-23mm) for clamping 7/8"OD tubing |
| 9    |   2 or 4 | JST           | SVH-21T-P1.1          | 0.11         | [K][9digi] [R][9rs]   | VH crimps, for power connector |
| 10   |   1 or 2 | TE            | 1-1123722-2           | 0.18         | [K][10digi] [R][10rs] | VH housing 2 pin, for power connector |

[1ali]:   https://www.aliexpress.com/item/32980201709.html
[2won]:   https://wonsmart-motor.en.made-in-china.com/product/hsjxFewOppVg/China-Air-Pump12V-Brushless-Motor-12V-Blower-Fan-Driver.html
[3mcmc]:  https://www.mcmaster.com/5234K57
[4mcmc]:  https://www.mcmaster.com/9579K72
[9digi]:  https://www.digikey.com/short/z44fjr
[9rs]:    https://export.rsdelivers.com/product/jst/svh-41t-p11/jst-nv-vh-female-crimp-terminal-contact-16awg-svh/7620692
[10digi]: https://www.digikey.com/short/z44fwj
[10rs]:   https://export.rsdelivers.com/product/jst/vhr-2n/jst-vhr-female-connector-housing-396mm-pitch-2/8201172

### Tools

| Item | Quantity | Manufacturer  | Part #              | Price (USD) | Sources         | Notes |
| ---- |---------:| ------------- | ------------------- | -----------:|-----------------| ----- |
| a1   |        1 | Preciva       | PR-3254 (PR102001)  |    39.99 | [Z][a1amzn]     | Crimping tool for HX and HV connectors |

[a1amzn]:https://www.amazon.com/gp/product/B07R1H3Z8X

**NOTE: Items 9-10 and a1 above are also duplicated in the [wiring BOM](../../design/electrical-system/wiring.md)**

### Instructions for basic assembly

Use short segment of tubing, clamp to blower and adapter.
Use Herbie clip to secure tubing to blower. Alternatively, you can use a zip-tie.

You will also have to crimp connectors to the second end of the wiring that connects the driver board
to the [PCB](../../pcb).

A basic assembly is pictured at the top of this page.

**please note:** the picture above includes an optional tubing reducer that is specified on the [pizza build](../pizza_build) page.

## Enclosed assembly

The basic assembly needs further modifications to be mounted into an [enclosed ventilator assembly](../enclosed_build).
The pneumatic interface includes gaskets and adapter for tubing that runs to the a filtered air inlet on the front
panel. The mechanical aspect of this assembly includes all hold-down and mounting plates for the blower and blower
driver board, and for the above pneumatic interface components. Electrically, the enclosed assembly also requires longer
wiring.

### Enclosure parts

| Item | Quantity | Manufacturer  | Part #                       | Price (USD)      |Sources                  | Notes |
| ---- |---------:| ------------- | ---------------------------- | ----------------:|:-----------------------:|-------|
| b1   |        1 | RespiraWorks  | tubing_adapter               | $1 (material)    | [B][b1rw]               | 3D printed from data in link |
| b2   |          | McMaster-Carr | 8560K357                     | $54.68 / 3716cm² | [C][b2mcmc]             | 1/4" (~6mm) thick clear acrylic sheet, to make b3-b5 below |
| b3   |        1 | RespiraWorks  | tubing_adapter_holding_plate | (material)       | [B][b3rw]               | Acrylic (b2 above) cut using the drawing in link |
| b4   |        1 | RespiraWorks  | driver_mounting_plate        | (material)       | [B][b4rw]               | Acrylic (b2 above) cut using the drawing in link |
| b5   |        1 | RespiraWorks  | blower_holding_plate         | (material)       | [B][b5rw]               | Acrylic (b2 above) cut using the drawing in link |
| b6   |          | McMaster-Carr | 8785K82-8785K822             | $22.22 / 929cm²  | [C][b6mcmc]             | Gasket material, to make items b6+b8 below |
| b7   |        1 | RespiraWorks  | blower_gasket                | (material)       | [B][b7rw]               | Gasket material (b6 above) cut using drawing in link |
| b8   |        1 | RespiraWorks  | tubing_adapter_gasket        | (material)       | [B][b8rw]               | Gasket material (b6 above) cut using drawing in link |
| b9   |        8 | McMaster-Carr | 93625A150                    | 6.04 / 100       | [C][b9mcmc]             | M4 nylon insert lock nuts |
| b10  |       16 | McMaster-Carr | 93475A230                    | 1.86 / 100       | [C][b10mcmc]            | M4 washers |
| b11  |        4 | McMaster-Carr | 92000A224                    | 9.60 / 100       | [C][b11mcmc]            | M4 x 14mm Philips head screws, for blower driver |
| b12  |        4 | McMaster-Carr | 91290A574                    | 15.45 / 10       | [C][b12mcmc]            | M4 x 85 mm hex head screws |
| b13  |        4 | McMaster-Carr | 93657A079                    | 1.21             | [C][b13mcmc]            | 10mm nylon spacers |
| b14  |        4 | McMaster-Carr | 91828A231                    | 6.45 / 100       | [C][b14mcmc]            | M4 nut |
| b15  |       12 | JST           | SXH-001T-P0.6                | 0.10             | [K][b15digi] [R][b15rs] | XH crimps, for control connector |
| b16  |        2 | JST           | XHP-6                        | 0.14             | [K][b16digi] [R][b16rs] | XH 6-pin housing, for control connector |
| b17  |        - | CQRobot       | CQRJST254-BGB                | 9.99             | [Z][b17amzn]            | XH connector kit for 6/7/8 pins, alternative for b15+b16 above |
| b18  |        - | Remington     | 16UL1007STRKIT               | 29.70            | [K][b18amzn]            | 16 AWG wire, for power cable |
| b19  |        - | Remington     | 26UL1007STRKIT10COLOR        | 31.60            | [K][b19amzn]            | 26 AWG wire, for control cable  |

[b1rw]:    CAD/tubing_adapter.stl
[b2mcmc]:  https://www.mcmaster.com/8560K357/
[b3rw]:    CAD/tubing_adapter_holding_plate.dxf
[b4rw]:    CAD/driver_mounting_plate.dxf
[b5rw]:    CAD/blower_holding_plate.dxf
[b6mcmc]:  https://www.mcmaster.com/8785K82-8785K822/
[b7rw]:    CAD/blower_gasket.dxf
[b8rw]:    CAD/tubing_adapter_gasket.dxf
[b9mcmc]:  https://www.mcmaster.com/93625A150/
[b10mcmc]: https://www.mcmaster.com/93475A230/
[b11mcmc]: https://www.mcmaster.com/92000A224/
[b12mcmc]: https://www.mcmaster.com/91290A574/
[b13mcmc]: https://www.mcmaster.com/93657A079/
[b14mcmc]: https://www.mcmaster.com/91828A231/
[b15digi]: https://www.digikey.com/short/z44f8d
[b15rs]:   https://export.rsdelivers.com/product/jst/sxh-002t-p06/jst-xh-female-crimp-terminal-contact-26awg-sxh/1755645
[b16digi]: https://www.digikey.com/short/z44ff8
[b16rs]:   https://export.rsdelivers.com/product/jst/xhp-6/jst-xhp-female-connector-housing-25mm-pitch-6-way/8201620
[b17amzn]: https://www.amazon.com/gp/product/B079MJ1RYN
[b18amzn]: https://www.amazon.com/gp/product/B00N51OJJ4
[b19amzn]: https://www.amazon.com/gp/product/B011JC76OA

**NOTE: Items b15-b19 above are also duplicated in the [wiring BOM](../../design/electrical-system/wiring.md)**

### Custom component manufacture

#### Tubing adapter

To 3d print this component see the [general 3d printing guidelines](../3d_printing).

| Rendering      | STL        | Source |
|:--------------------------:|:---------------------------:|:----------------------------:|
|![](CAD/tubing_adapter.jpg)|[tubing_adapter.stl](CAD/tubing_adapter.stl)|[tubing_adapter.SLDPRT](CAD/tubing_adapter.SLDPRT)|

#### Gaskets

You will want to use an Exacto knife to cut these from the gasket material (**Item b6** in [BOM above](#enclosure-parts)).

|    | Blower gasket              | Tubing adapter gasket       |
|---:|:--------------------------:|:---------------------------:|
|PNG |![](CAD/blower_gasket.png)|![](CAD/tubing_adapter_gasket.png)|
|DXF |[blower_gasket.dxf](CAD/blower_gasket.dxf)|[tubing_adapter_gasket.dxf](CAD/tubing_adapter_gasket.dxf)|

#### Acrylic plates

You will have to cut the following mounting and holding plates from an acrylic sheet (**Item b2** in [BOM above](#enclosure-parts)).

You will likely need some of the following
* jigsaw or laser cutter for straight cuts
* drill press
* 4.5mm or 11/64" drill bit for holes (make slightly wider than specified in drawings)
* 30mm or 1-1/4" hole cutter or Forstner bit
* 37mm or 1-1/2" hole cutter or Forstner bit

|    | Driver mounting plate      | Blower holding plate        | Tubing adapter holding plate |
|---:|:--------------------------:|:---------------------------:|:----------------------------:|
|PNG |![](CAD/driver_mounting_plate.png)|![](CAD/blower_holding_plate.png)|![](CAD/tubing_adapter_holding_plate.png)|
|DXF |[driver_mounting_plate.dxf](CAD/driver_mounting_plate.dxf)|[blower_holding_plate.dxf](CAD/blower_holding_plate.dxf)|[tubing_adapter_holding_plate.dxf](CAD/tubing_adapter_holding_plate.dxf)|

You can use one of these plates as a template to drill the appropriate holes in the main acrylic plate and the metal
fame that supports it.

All of the above components (except gaskets) should appear as follows:

![](images/assembly_parts.jpg)

### Assembly instructions

#### Mount the blower driver

Use:
* 4x M4 14mm screws (b11)
* 4x M4 washers (b10)
* 4x M4 lock nuts (b9)

Mount the blower driver (**Item 2**) to the Driver mounting plate (**Item b4**). It is advisable to run the screws
from the bottom up, because the screw heads may be too big to fit next to the aluminum base of the driver plate.

![](images/blower_driver_mounting.jpg)

#### Make blower sandwich

|                            |                             |
|:--------------------------:|:---------------------------:|
|Take the mounted blower driver from step above, and run four 85mm screws (b12) with m4 washers (b10) downwards. | Flip this assembly upside down. Put four spacers (b13) on the screws. Run the wiring of blower (Item 1) through hole in blower holding plate (b5)|
|![](images/blower_mounting.jpg) | ![](images/blower_mounting2.jpg) |

|                            |                             |
|:--------------------------:|:---------------------------:|
| Slide the blower holding plate over the screws so that wiring comes out through the space between the two plates, with blower on top. Place blower gasket (b7) on main enclosed ventilator assembly. | Carefully to not lose the screws, flip the blower sandwich and install it onto the main assembly. Fasten with M4 washers (b10) and regular non-locking nuts (b14).|
|![](images/blower_mounting3.jpg) | ![](images/blower_mounting4.jpg) |

Tighten carefully one nut at a time, ensuring a tight gasket seal. Do not over-tighten. If you see the acrylic holding plate begin to bend, relax the fasteners somewhat.

#### Install tubing adapter

You will likely need some sort of small clamps for this step, to ensure that components stay in place while you fasten them.

* Make sure the air filter assembly plate is (temporarily) fitted to the main ventilator assembly.
* Flip the ventilator assembly face-down, so that the bottom of the main assembly plate faces you vertically.
* Insert tubing adapter (b1) through the tubing adapter holding plate (b3) and place this assembly on the bottom side of the main mounting plate.
* Now insert the tubing adapter gasket (b8) in the gap, adjust its position to be centered on the blower inlet and apply pressure with the tubing adapter to hold it in place.
* Turn the tubing adapter so that the tubing barb faces the general direction of the air filter assembly that it will connect to.
* Temporarily clamp the entire blower assembly in place.
* Fasten with m4 washers (b10) and lock nuts (b9).

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](images/tubing_adapter_holding_plate.jpg) | ![](images/integrated.jpg) |

#### Manufacture extended wiring

The power and signal cables that come included with the blower are too short to reach the PCB, so new ones must be
manufactured. Please follow the old cables as examples as well as any advice on the
[wiring assembly page](../../design/electrical-system/wiring.md).

The new cables should each be about 42cm in length.

**#TODO:** Some sort of shrink tubing should also be used

![](images/wired.jpg)
