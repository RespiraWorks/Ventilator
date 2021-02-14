# Display panel

| Rendering | Assembled |
|:-------------------------:|:-------------------------:|
| ![](images/display_panel_assembly_rendering.jpg) | ![](images/through-fitting_2.jpg) |

## Design rationale

The display panel accommodates the LCD touchscreen display and the oxygen inlet port. The base is a clear acrylic
that also allows one to view the ventilator internals. While in the prototyping stage, it might be of advantage
in noting mechanical problems and also adds an aesthetic touch.

### Bracket mounts

For serviceability, ventilator internals should be completely removable from the enclosure as a single assembly.
It was necessary to secure the display panel to the [main equipment bracket](../enclosure) while also allowing it
to lay flush with the front face of the enclosure. This means we should not have either screws or bolts some out
above the front surface of the panel. We chose to use heat-set inserts. Tapped/plastic threads are easy to overload and strip out. Metal threads provide a
reliable way of fastening plastic and acrylic parts. M4 hardware was chosen because of their prevalence in the rest of
the assembly.

The sizing for the mounting holes was based on [this](https://www.pemnet.com/fastening_products/pdf/sidata.pdf) catalog
for heat set inserts. For M4 nuts (page 6, metric table, row 6), length code 1 (5.72 mm) is small enough to fit in the
6.35mm-thick acrylic. The corresponding outer diameter for insertion i.e. acrylic mounting diameter is 5.94 mm,
which is reflected in the CAD.

### Touchscreen mounts

The touchscreen dictates that M2 screws are should be used, the standard clearance hole being 2.4mm,
which is reflected in the CAD.

### Oxygen inlet

Brass fittings are resistant to corrosion and can resist high pressures.
However:
* connector may not be compatible for oxygen supply systems
* check valve may not have the right cracking properties
* inlet may be better located at the back of the unit

These changes are likely forthcoming in future iterations of our design.

## Custom Parts

The only custom part in this assembly is the custom cut display acrylic.

| Download Type | Link   |
|:--------------|:-------|
| Source: Inventor part file | [.ipt](display_panel_acrylic_plate.ipt) |
| Drawing                    | [.idw](display_panel_acrylic_plate.idw) [.pdf](display_panel_acrylic_plate.pdf) |
| Export for laser cutting   | [.dxf](display_panel_acrylic_plate.dxf) |

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../purchasing_guidelines.md
[pneu]: ../pneumatics
[elec]: ../electrical

| Item  | Quantity | Manufacturer  | Part #                   | Price (USD)  | Sources[*][ppg]| Notes |
| ----- |---------:| ------------- | ------------------------ | ------------:|:------------:|:------|
|**A1** | 1        | RespiraWorks  | upper_acrylic_face_panel |              | [Rw][a1rw]   | Upper acrylic face panel |
|**A2** | (1)      | McMaster-Carr | 7768K22                  | 11.62        | [C][a2mcmc]  | brass threaded check valve, **DUPLICATE in [pneumatic assembly][pneu]** |
|**A3** | (1)      | McMaster-Carr | 50785K273                | 8.23         | [C][a3mcmc]  | through-wall straight connector, 1/4NPT female, **DUPLICATE in [pneumatic assembly][pneu]** |
|**A4** | 1        | McMaster-Carr | 5121K451                 | 5.78 / 10    | [C][a4mcmc]  | 1/4 NPT x 1/4" ID barbed adapter, **DUPLICATE in [pneumatic assembly][pneu]** |
|**A5** | (1)      | UPerfect      | -                        | 49.00        | [A][a5ali]   | 7" capacitive touchscreen, **DUPLICATE in [electrical assembly][elec]** |
|**A6** | 1        | RespiraWorks  | PCB                      |             | [Rw][a6rw]    | RespiraWorks custom PCB, **DUPLICATE in [electrical assembly][elec]** |
|**A7** | (1)      | RespiraWorks  | PCB_interim_board        |             | [Rw][a7rw]    | RespiraWorks Rev1 interim board **OPTIONAL**, **DUPLICATE in [electrical assembly][elec]** |
|**A8** | 1        | ST Micro      | NUCLEO-L452-RE           | 14.43       | [M][a8mous] [R][a8rs]   | STM32 Nucleo dev board, **DUPLICATE in [electrical assembly][elec]**, **DO NOT get the -P** version |
|**A9** | 2        | Digikey       | X-NUCLEO-IHM03A1         | 10.64       | [K][a9key] [M][a9mous]  | Stepper driver board, **DUPLICATE in [electrical assembly][elec]** |
|**A10** | (1)      | ST            | X-NUCLEO-IHM02A1         | 15.00       | [S][a10st] [R][a10rs]     | Two-axis stepper driver board, **Alternative to 2x`[A10]`**, **DUPLICATE in [electrical assembly][elec]**  |
|**A11** | 1        | Raspberry Pi  | RPI4-MODBP-2GB           | 53.23       | [M][a11mous]             | Raspberry Pi 4, **DUPLICATE in [electrical assembly][elec]** |
|**A12** | 4        | McMaster-Carr | 93625A101                | 7.70 / 10    | [C][a12mcmc]  | M2 locknut |
|**A13** | 8        | McMaster-Carr | 93475A195                | 1.06 / 100   | [C][a13mcmc]  | M2 washer, 5mm OD |
|**A14** | (8)      | McMaster-Carr | 91116A240                | 2.40 / 100   | [C][a14mcmc]  | M2 washer, 7mm OD, **alternate to A7** |
|**A15** | 4        | McMaster-Carr | 92000A019                | 6.75 / 100   | [C][a15mcmc]  | M2 screw 12mm, Phillips drive |
|**A16**| (4)      | McMaster-Carr | 92095A455                | 7.44 / 24    | [C][a16mcmc] | M2 screw 12mm, hex drive, **alternate to A9** |
|**A17**| 4        | McMaster-Carr | 94180A351                | 15.47 / 100  | [C][a17mcmc] | Heat-set inserts for M4 screws |
|**A18**| 4        | McMaster-Carr | 93475A230                | 1.86 / 100   | [C][a18mcmc] | M4 washer, 9mm OD |
|**A19**| 4        | McMaster-Carr | 92095A189                | 8.89 / 100   | [C][a19mcmc] | M4 8mm screw, hex socket drive |
|**A20**| 4        | McMaster-Carr | 91292A010                | 4.97 / 100   | [C][a20mcmc] | M2.5 6mm screw, hex socket drive |  

**#TODO: type of acrylic**  
**#TODO: laser cutting machine specs**

[a1rw]:    #custom-parts
[a2mcmc]:  https://www.mcmaster.com/7768K22/
[a3mcmc]:  https://www.mcmaster.com/50785K273/
[a4mcmc]:  https://www.mcmaster.com/5121K451
[a5ali]:   https://www.aliexpress.com/item/4000747984746.html
[a6rw]:    ../../pcb
[a7rw]:    https://github.com/inceptionev/VentilatorRev1InterimDaughtercard
[a8mous]:  https://www.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=sGAEpiMZZMtw0nEwywcFgEEYp888DlnM1Y5kGes2rJIHvcJjT1ZDkw%3D%3D
[a8rs]:    https://export.rsdelivers.com/product/stmicroelectronics/nucleo-l452re/stmicroelectronics-stm32-nucleo-64-mcu/1261775
[a9key]:   https://www.digikey.com/short/z442qt
[a9mous]:  https://www.mouser.com/ProductDetail/511-X-NUCLEO-IHM03A1
[a10st]:    https://www.st.com/en/ecosystems/x-nucleo-ihm02a1.html#sample-and-buy
[a10rs]:    https://export.rsdelivers.com/product/stmicroelectronics/x-nucleo-ihm02a1/stmicroelectronics-x-nucleo-ihm02a1-two-axis/1646982
[a11mous]:  https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-2GB-BULK?qs=%2Fha2pyFaduiq9oc0d1uK569Mu3%252BsSMVa9bhYkyZbjQ1oNl8pHrdrS2f8pDbixKgb
[a12mcmc]:  https://www.mcmaster.com/93625A101/
[a13mcmc]:  https://www.mcmaster.com/93475A195/
[a14mcmc]:  https://www.mcmaster.com/91116A240/
[a15mcmc]:  https://www.mcmaster.com/92000A019/
[a16mcmc]: https://www.mcmaster.com/92095A455/
[a17mcmc]: https://www.mcmaster.com/94180A351/
[a18mcmc]: https://www.mcmaster.com/93475A230/
[a19mcmc]: https://www.mcmaster.com/92095A189/
[a20mcmc]: https://www.mcmaster.com/91292A010/

## Assembly

| Part  | Source |
|:------|:-------|
| Display panel assembly    | [.iam](display_panel_assembly.iam) |
| Touch-screen sub-assembly with PCB stack | [.iam](display_and_PCB_stack_assembly.iam)       |

**#TODO: Drawings**

### Heat-set insert installation

* You should first install the heat-set inserts, while the acrylic panel is flat and no electronics or
fitting are in the way of your and your soldering iron.

1. Select a soldering iron tip large enough to provide uniform pressure on the center of the heat-set insert, but small
   enough to not touch the surrounding material.
2. Heat up your soldering iron to an operating temperature of around 343-399&deg;C (650-750&deg;F).
3. Place the heat-set insert `[A11]` into the mounting hole (one of the four corner holes on the display acrylic) with
   the smaller diameter facing down into the hole away from you.
4. Touch the tip of the soldering iron to the top of the heat-set insert and wait for a few seconds.
5. The weight of the soldering iron should gently push the heat-set insert into the now melted acrylic.
6. Press down slowly until the half the heat-set insert inserted into the acrylic. At this point only the first set of
   knurls should be inside the acrylic.
7. Allow the acrylic to cool. Then push inserts in all the way till it is flush with the face of the acrylic. Works best
   if you push each of the four halfway, let them cool and come back to them.
8. Remove heat and allow to cool and harden.

### Additional notes

* Mount the touch-screen to the acrylic using M2 bolts and nuts. Use washers on both sides.

* Install the brass through-fitting `[A3]` with the lock-nut on the outside of of the panel.

* Install check valve `[A2]` on the outside of the panel.

* Use PTFE tape on al NPT joints.

|  Front    |  Back   |
:------------------:|:-----------------:|
![](images/through-fitting_1.jpg)  |  ![](images/through-fitting_2.jpg)  |
