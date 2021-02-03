# Blower Sub-assembly

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](images/assembly_rendering.jpg) | ![](images/integrated.jpg) |

The blower/fan provides air pressure to the ventilator's pneumatic circuit. Currently this is only responsible for
delivery of ambient air, independent of the question of oxygen supply. In a full system that includes oxygen supply,
the blower is a subsystem to the overarching gas delivery control system.

Characterization tests for the blower are [here](characterization-tests).

## Enclosed assembly

![](CAD/exports/blower_assembly.jpg)

|Download Type|Link|
|:------|:-------|
|Assembly Drawing PDF|[PDF](CAD/exports/blower_assembly.pdf)|
|Autodesk Inventor CAD Assembly|[.iam](CAD/blower_assembly.iam)|

### Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                       | Price (USD)      | Sources[*][ppg] | Notes |
| ----- |---------:| ------------- | ---------------------------- | ----------------:|:---------------:|-------|
|**A1** | 1        |               | WS7040-12-X200N              | 65.00            | [A][a1ali]      | 12v blower and driver. Make sure to choose with driver when ordering. |
|**A2** | 1        |               | WS1208                       | 24.00            | [W][a2won]      | **optional** blower driver, in case blower above did not come with driver. |
|**A3** | 1        | RespiraWorks  | tubing_adapter               | 1 (material)     | [Rw][a3rw]      | 3D printed from data in link |
|**A4** | ~200cm²  | McMaster-Carr | 8560K357                     | 54.68 / 3716cm²  | [C][a4mcmc]     | 1/4" (~6mm) thick clear acrylic sheet, to make **A3-C5** below |
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
[a3rw]:    CAD/exports/tubing_adapter.stl
[a4mcmc]:  https://www.mcmaster.com/8560K357/
[a5rw]:    CAD/exports/driver_mounting_plate.dxf
[a6rw]:    CAD/exports/blower_holding_plate.dxf
[a7rw]:    CAD/exports/tubing_adapter_holding_plate.dxf
[a8mcmc]:  https://www.mcmaster.com/1370N13-1370N131/
[a9rw]:    CAD/exports/blower_gasket.dxf
[a10rw]:    CAD/exports/tubing_adapter_gasket.dxf
[a11mcmc]:  https://www.mcmaster.com/93625A150/
[a12mcmc]: https://www.mcmaster.com/93475A230/
[a13mcmc]: https://www.mcmaster.com/92095A168/
[a14mcmc]: https://www.mcmaster.com/91290A574/
[a15mcmc]: https://www.mcmaster.com/99072A203/
[a16mcmc]: https://www.mcmaster.com/91828A231/
[a17mcmc]: https://www.mcmaster.com/93625A100/

### Tools

| Item   | Quantity | Manufacturer   | Part #             | Price (USD)  | Sources[*][ppg] | Notes |
| ------ |---------:| -------------- | -------------------| ------------:|:---------------:|:------|
| **B1** | 1        | Fiskars        | 167110-1001        | 5.49         | [A][b1amzn]     | Craft knife, for cutting gaskets |
| **B2** | 1        | Black & Decker | BDEJS300C          | 24.49        | [A][b2amzn]     | Jigsaw, for cutting acrylic |
| **B3** | 1        | Black & Decker | BDEDMT             | 29.99        | [A][b3amzn]     | Electric hand drill, **drill press preferred if you have one**, for drilling acrylic |
| **B4** | 1        | XLCMSY         | EAN: 0742880496381 | 12.99        | [A][b4amzn]     | Cobalt metric drill bits, in particular - 4.5mm |
| **B5** | 1        | Porter Cable   | PC1014             | 59.59        | [A][b5amzn]     | Forstner bits, **preferred for drill press** |
| **B6** | 1        | KATA TOOLS     | KTL89020           | 19.99        | [A][b5amzn]     | Hole saws, **preferred for hand drill** |

[b1amzn]: https://www.amazon.com/Fiskars-167110-1001-Heavy-Craft-Knife/dp/B001A42CTW
[b2amzn]: https://www.amazon.com/BLACK-DECKER-Jig-Amp-BDEJS300C/dp/B00OJ72L84
[b3amzn]: https://www.amazon.com/BLACK-DECKER-BDEDMT-Matrix-Driver/dp/B0094B9BHE
[b4amzn]: https://www.amazon.com/XLCMSY-Straight-Resistant-Stainless-1-5-6-5mm/dp/B07ZQGXWGL
[b5amzn]: https://www.amazon.com/PORTER-CABLE-PC1014-Forstner-Bit-14-Piece/dp/B004TSZEB4
[b6amzn]: https://www.amazon.com/KATA-19-152mm-Mandrels-Installation-Plywood/dp/B08JCHY2DM

### Custom component manufacture

#### Tubing adapter (C1)

To 3d print this component see the [general 3d printing guidelines](../../../3d_printing).

|For 3D printing: [tubing_adapter.stl](CAD/exports/tubing_adapter.stl)| Inventor source: [tubing_adapter.ipt](CAD/tubing_adapter.ipt)|
|:---:|:---:|
|![](images/tubing_adapter_rendering.jpg)|[![](CAD/exports/tubing_adapter.jpg)](CAD/exports/tubing_adapter.pdf)|

#### Gaskets

Use craft knife (**B1**) to cut these from the gasket material (**A8**).

|     | Blower gasket (C9)         | Tubing adapter gasket (C10) |
|----:|:--------------------------:|:---------------------------:|
| PNG | ![](CAD/exports/blower_gasket.png) | ![](CAD/exports/tubing_adapter_gasket.png) |
| DXF |[blower_gasket.dxf](CAD/exports/blower_gasket.dxf)|[tubing_adapter_gasket.dxf](CAD/exports/tubing_adapter_gasket.dxf)|
| Inventor |[blower_gasket.ipt](CAD/blower_gasket.ipt)|[tubing_adapter_gasket.ipt](CAD/tubing_adapter_gasket.ipt)|

#### Acrylic plates

You will have to cut the following mounting and holding plates from an acrylic sheet (**A4**).

If you have access to an adequately powerful laser cutter, it should be able to perform all perimeter and hole cuts.

Otherwise you will need some combination of tools (**B2-6**) listed in the [Tools 2](#tools-2) section above.

Out of the drill sets, you will need:
- 4.5mm (11/64") drill bit (from **B4**) for fastener holes (make them slightly larger than specified in drawings)
- 30mm (1-1/4") Forstner bit or hole cutter (from either **B5** or **B6**)
- 37mm (1-1/2") Forstner bit or hole cutter (from either **B5** or **B6**)

You might also want to use some 200 grit sandpaper to clean up the edges of the pieces you cut and drill.

|     | Driver mounting plate (C5) | Blower holding plate (C6) | Tubing adapter holding plate (C7) |
|:-----:|:--------------------------:|:-------------------------:|:---------------------------------:|
|  |![](CAD/exports/driver_mounting_plate.jpg)|![](CAD/exports/blower_holding_plate.jpg)|![](CAD/exports/tubing_adapter_holding_plate.jpg)|
| Drawings |[Driver Mounting Plate](CAD/exports/driver_mounting_plate.pdf)|[Blower Holding Plate](CAD/exports/blower_holding_plate.pdf)|[Tubing Adapter Holding Plate](CAD/exports/tubing_adapter_holding_plate.pdf)|
| DXF |[driver_mounting_plate.dxf](CAD/exports/driver_mounting_plate.dxf)|[blower_holding_plate.dxf](CAD/exports/blower_holding_plate.dxf)|[tubing_adapter_holding_plate.dxf](CAD/exports/tubing_adapter_holding_plate.dxf)|
| Inventor | [driver_mounting_plate.ipt](CAD/driver_mounting_plate.ipt)|[blower_holding_plate.ipt](CAD/blower_holding_plate.ipt)| [tubing adapter_holding_plate.ipt](CAD/tubing_adapter_holding_plate.ipt)|


You can use one of these plates as a template to drill the appropriate holes in the main acrylic plate and the metal
frame that supports it.

The above manufactured components (excluding gaskets) should appear as follows:

![](images/assembly_parts.jpg)

### Assembly instructions

#### Mount the blower driver

Use the following hardware:
* 4x M3 14mm screws (**A13**)
* 4x M3 lock nuts (**A17**)

Mount the blower driver (**A2**) to the Driver mounting plate (**A5**). It is advisable to run the screws from the
bottom up, because the screw heads may be too wide to fit next to the circuit board.

![](images/blower_driver_mounting.jpg)

#### Make blower sandwich

|                            |                             |
|:--------------------------:|:---------------------------:|
|Take the mounted blower driver from step above, and run four 85mm screws (**A14**) with m4 washers (**A12**) downwards. | Flip this assembly upside down. Put four spacers (**A15**) on the screws. Run the wiring of blower (**A1**) through hole in blower holding plate (**A6**)|
|![](images/blower_mounting.jpg) | ![](images/blower_mounting2.jpg) |

|                            |                             |
|:--------------------------:|:---------------------------:|
| Slide the blower holding plate over the screws so that wiring comes out through the space between the two plates, with blower on top. Place blower gasket (**A9**) on main enclosed ventilator assembly. | Carefully to not lose the screws, flip the blower sandwich and install it onto the main assembly. Fasten with m4 washers (**A12**) and regular non-locking nuts (**A16**).|
|![](images/blower_mounting3.jpg) | ![](images/blower_mounting4.jpg) |

Tighten carefully one nut at a time, ensuring a tight gasket seal. Do not over-tighten. If you see the acrylic holding plate begin to bend, relax the fasteners somewhat.

#### Install tubing adapter

You will likely need some sort of small clamps for this step, to ensure that components stay in place while you fasten them.

* Make sure the air filter assembly plate is (temporarily) fitted to the main ventilator assembly.
* Flip the ventilator assembly face-down, so that the bottom of the main assembly plate faces you vertically.
* Insert tubing adapter (**A3**) through the tubing adapter holding plate (**A7**) and place this assembly on the bottom side of the main mounting plate.
* Now insert the tubing adapter gasket (**A10**) in the gap, adjust its position to be centered on the blower inlet and apply pressure with the tubing adapter to hold it in place.
* Turn the tubing adapter so that the tubing barb faces the general direction of the air filter assembly that it will connect to.
* Temporarily clamp the entire blower assembly in place.
* Fasten with m4 washers (**A12**) and lock nuts (**A11**).

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](images/tubing_adapter_holding_plate.jpg) | ![](images/integrated.jpg) |

## System integration

Integration into the pneumatic system is covered in either [pizza build](../../../pizza_build) or
[enclosed build pneumatics](..) pages, depending on which version you are building.

Integration into the electrical/controls system is covered in [wiring page](../../../electrical/wiring).

## Price estimate

**#TODO:** out of date (used old BOMs), need to update this

**Basic assembly:**

65 (A1) + 0.70 (A3) + 1.20 (A4) + 0.22 (A5) + 0.18 (A6) = 67.30 USD

**Complete assembly for enclosure:**

67.30 (basic assembly) + 1 (C1) + 3 (C3-5) + 2.5 (C7-8) + 0.48 (C9) + 0.3 (C10) + 0.4 (C11) + 6.18 (C12) + 4.84 (C13)  +
0.26 (C14) + 1.2 (C15) + 0.28 (C16) + 1.2 (C18) + 1.3 (C19) + 0.22 (A5) + 0.18 (A6) = 90.64 USD
