# Display panel

| Rendering | Assembled |
|:-------------------------:|:-------------------------:|
| ![](images/display_panel_assembly_rendering.jpg) | ![](images/everything.jpg) |

## Design rationale

The display panel accommodates the LCD touchscreen display, the oxygen inlet port, and the main control electronics of the ventilator. The base is a clear acrylic
that also allows one to view the ventilator internals. While in the prototyping stage, it might be of advantage
in observing mechanical problems and also adds an aesthetic touch.

Note that the acrylic panel and circuit boards are custom components and will need to be fabricated by a contract manufacturer before the display panel is assembled.

Related issues are being tracked here:
[#653](https://github.com/RespiraWorks/Ventilator/issues/653)
[#646](https://github.com/RespiraWorks/Ventilator/issues/646)

### Oxygen inlet

Brass fittings are resistant to corrosion and high pressures.

However:
* connector may not be compatible for oxygen supply systems
* check valve may not have the right cracking pressure and other pneumatic properties
* inlet may be better located at the back of the unit

These changes are likely forthcoming in future iterations of our design.



## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                       | Price (USD) | Sources[*][ppg]| Notes |
| ----- |---------:| ------------- | ---------------------------- | -----------:|:--------------:|:------|
|**A1** |(~550cm²) | McMaster-Carr | 8560K357                     | 150 / m²    | [C][a1mcmc]    | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A2]` below |
|**A2** | 1        | RespiraWorks  | [Display panel acrylic][a2rw]| 8.25        | Rw             | Upper acrylic face panel, cut from acrylic `[A1]` |
|**A3** | 1        | McMaster-Carr | 7768K22                      | 11.62       | [C][a3mcmc]    | brass threaded check valve |
|**A4** | 1        | McMaster-Carr | 50785K273                    | 8.23        | [C][a4mcmc]    | through-wall straight connector, 1/4NPT female |
|**A5** | 1        | McMaster-Carr | 5121K451                     | 0.58        | [C][a5mcmc]    | 1/4 NPT x 1/4" ID barbed adapter |
|**A6** | 1        | Eviciv        | DS702                        | 49.00       | [A][a6ali] [Z][a6amzn]| 7" capacitive touchscreen with speakers |
|**A7** | 4        | McMaster-Carr | 93625A102                    | 0.11        | [C][a7mcmc]    | M2.5 locknut |
|**A8** | 4        | McMaster-Carr | 98689A111                    | 0.03        | [C][a8mcmc]    | M2.5 washer, 5mm OD |
|**A9** | 4        | McMaster-Carr | 91116A110                    | 0.03        | [C][a9mcmc]    | M2.5 washer, 8mm OD |
|**A10**| 4        | McMaster-Carr | 92095A504                    | 1.72        | [C][a10mcmc]   | M2.5 screw, 14mm |
|**A11**| 4        | McMaster-Carr | 94180A351                    | 0.15        | [C][a11mcmc]   | Heat-set inserts for M4 screws |
|**A12**| 4        | McMaster-Carr | 93475A230                    | 0.02        | [C][a12mcmc]   | M4 washer, 9mm OD |
|**A13**| 4        | McMaster-Carr | 92095A189                    | 0.09        | [C][a13mcmc]   | M4 screw, 8mm |
|**A14**| 4        | DigiKey       | 732-10395-ND                 | 0.59        | [K][a14key]    | M2.5 standoff, 9mm |
|**A15**| 1        | RespiraWorks  |[Electronics assembly][a15rw] | 513.17      | Rw             | Most electrical and computing components |

**Total assembly price:** USD 601.81

[a1mcmc]:  https://www.mcmaster.com/8560K357/
[a2rw]:    #custom-parts
[a3mcmc]:  https://www.mcmaster.com/7768K22/
[a4mcmc]:  https://www.mcmaster.com/50785K273/
[a5mcmc]:  https://www.mcmaster.com/5121K451
[a6ali]:   https://www.aliexpress.com/item/4000747984746.html
[a6amzn]:  https://www.amazon.com/Eviciv-Portable-Monitor-Display-1024X600/dp/B07L6WT77H
[a7mcmc]:  https://www.mcmaster.com/93625A102/
[a8mcmc]:  https://www.mcmaster.com/98689A111/
[a9mcmc]:  https://www.mcmaster.com/91116A110/
[a10mcmc]: https://www.mcmaster.com/92095A504/
[a11mcmc]: https://www.mcmaster.com/94180A351/
[a12mcmc]: https://www.mcmaster.com/93475A230/
[a13mcmc]: https://www.mcmaster.com/92095A189/
[a14key]:  https://www.digikey.com/en/products/detail/w-rth-elektronik/971090151/6174614
[a15rw]:   electronics


## Custom Parts

### Display Panel Acrylic Plate

[![](images/display_panel_acrylic_plate.jpg)](exports/display_panel_acrylic_plate.pdf)

| Download Type | Link   |
|:--------------|:-------|
| Engineering drawing      | [.idw](display_panel_acrylic_plate.idw) [.pdf](exports/display_panel_acrylic_plate.pdf) |
| Export for laser cutting | [.dxf](exports/display_panel_acrylic_plate.dxf) |
| Source: part CAD model   | [.ipt](display_panel_acrylic_plate.ipt) |

### Custom PCBs

See the **[electronics assembly](electronics)** page for turnkey manufacturing files and full details.

## Assembly

[![](images/display_panel_assembly.jpg)](exports/display_panel_assembly.pdf)

| Download type | Link   |
|:--------------|:-------|
| Engineering drawing                  | [.idw](display_panel_assembly.idw) [.pdf](exports/display_panel_assembly.pdf) |
| Source: display panel assembly model | [.iam](display_panel_assembly.iam) |
| Source: electronics assembly model   | [.iam](display_and_PCB_stack_assembly.iam) |

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

* You should first install the heat-set inserts `[A11]`, while the acrylic panel is flat and no electronics or fitting
  are in the way of your and your soldering iron.

* Install the brass through-fitting `[A5]` with the lock-nut on the outside of of the panel.

* Install check valve `[A4]` on the outside of the panel. It is best to do this before you install the touch screen
  so as to avoid potentially damaging it while working with bigger wrenches.

* Mount the touch-screen to the acrylic using M2.5 bolts and nuts. Use washers on both sides, bigger ones at the back.

* Mount the 9mm stand-offs onto the back of the touch-screen.

* Connect the micro-HDMI ribbon cable included with the touch-screen. If yours did not come with one, you
  may need additional parts.

|  Front            |  Back - with standoffs  |
:------------------:|:-----------------:|
![](images/front.jpg)  |  ![](images/back.jpg)  |

* The next step is to install the electronics stack, which is explained in detail on the
  [electronics assembly](electronics) page.
