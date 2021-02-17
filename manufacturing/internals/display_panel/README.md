# Display panel

| Rendering | Assembled |
|:-------------------------:|:-------------------------:|
| ![](images/display_panel_assembly_rendering.jpg) | ![](images/assembly_with_stack.jpg) |

## Design rationale

The display panel accommodates the LCD touchscreen display and the oxygen inlet port. The base is a clear acrylic
that also allows one to view the ventilator internals. While in the prototyping stage, it might be of advantage
in noting mechanical problems and also adds an aesthetic touch.

It is not clear whether the check valve has the correct cracking pressure and other pneumatic properties for our system.

Related issues are being tracked here:
[#653](https://github.com/RespiraWorks/Ventilator/issues/653)
[#646](https://github.com/RespiraWorks/Ventilator/issues/646)

### Bracket mounts

For serviceability, ventilator internals should be completely removable from the enclosure as a single assembly.
It was necessary to secure the display panel to the [main equipment bracket](../../enclosure) while also allowing it
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

[ppg]: ../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                   | Price (USD)  | Sources[*][ppg]| Notes |
| ----- |---------:| ------------- | ------------------------ | ------------:|:--------------:|:------|
|**A1** | ~550cm²  | McMaster-Carr | 8560K357                 | 150 / m²     | [C][a1mcmc]    | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A2]` below |
|**A2** | 1        | RespiraWorks  | upper_acrylic_face_panel |              | [Rw][a2rw]     | Upper acrylic face panel, cut from acrylic `[A1]` |
|**A3** | 2        | RespiraWorks  | electrical_assembly      |              | [Rw][a3rw]     |  |
|**A4** | 1        | McMaster-Carr | 7768K22                  | 11.62        | [C][a4mcmc]    | brass threaded check valve |
|**A5** | 1        | McMaster-Carr | 50785K273                | 8.23         | [C][a5mcmc]    | through-wall straight connector, 1/4NPT female |
|**A6** | 1        | McMaster-Carr | 5121K451                 | 5.78 / 10    | [C][a6mcmc]    | 1/4 NPT x 1/4" ID barbed adapter |
|**A7** | 4        | McMaster-Carr | 93625A101                | 7.70 / 10    | [C][a7mcmc]    | M2 lock nut |
|**A8** | 8        | McMaster-Carr | 93475A195                | 1.06 / 100   | [C][a8mcmc]    | M2 washer, 5mm OD |
|**A9** | 8        | McMaster-Carr | 91116A240                | 2.40 / 100   | [C][a9mcmc]    | M2 washer, 7mm OD |
|**A10**| 4        | McMaster-Carr | 92095A455                | 7.44 / 25    | [C][a10mcmc]   | M2 screw, 12mm |
|**A11**| 4        | McMaster-Carr | 94180A351                | 15.47 / 100  | [C][a11mcmc]   | Heat-set inserts for M4 screws |
|**A12**| 4        | McMaster-Carr | 93475A230                | 1.86 / 100   | [C][a12mcmc]   | M4 washer, 9mm OD |
|**A13**| 4        | McMaster-Carr | 92095A189                | 8.89 / 100   | [C][a13mcmc]   | M4 screw, 8mm |

[a1mcmc]:  https://www.mcmaster.com/8560K357/
[a2rw]:    #custom-parts
[a3rw]:    electronics
[a4mcmc]:  https://www.mcmaster.com/7768K22/
[a5mcmc]:  https://www.mcmaster.com/50785K273/
[a6mcmc]:  https://www.mcmaster.com/5121K451
[a7mcmc]: https://www.mcmaster.com/93625A101/
[a8mcmc]: https://www.mcmaster.com/93475A195/
[a9mcmc]: https://www.mcmaster.com/91116A240/
[a10mcmc]: https://www.mcmaster.com/92095A455/
[a11mcmc]: https://www.mcmaster.com/94180A351/
[a12mcmc]: https://www.mcmaster.com/93475A230/
[a13mcmc]: https://www.mcmaster.com/92095A189/

## Assembly

| Part  | Source |
|:------|:-------|
| Display panel assembly    | [.iam](display_panel_assembly.iam) |
| Touch-screen sub-assembly with PCB stack | [.iam](display_and_PCB_stack_assembly.iam)       |

**#TODO: Drawings**

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

* You should first install the heat-set inserts, while the acrylic panel is flat and no electronics or fitting are in
  the way of your and your soldering iron.

* Mount the touch-screen to the acrylic using M2 bolts and nuts. Use washers on both sides.

* The next logical step is probably the electronics stack, which is explained in detail on the
  [electronics assembly](electronics) page.

* Install the brass through-fitting `[A5]` with the lock-nut on the outside of of the panel.

* Install check valve `[A4]` on the outside of the panel.

|  Front    |  Back   |
:------------------:|:-----------------:|
![](images/through-fitting_1.jpg)  |  ![](images/through-fitting_2.jpg)  |
