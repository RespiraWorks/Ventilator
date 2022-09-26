# Display panel

|                                                  |                                       |
|:------------------------------------------------:|:-------------------------------------:|
| ![](images/display_panel_assembly_rendering.jpg) | ![](images/support_frame_drawing.jpg) |


## Design rationale

The display panel accommodates the LCD touchscreen display.  The base is a clear acrylic
that also allows one to view the ventilator internals. While in the prototyping stage, it might be of advantage
in observing mechanical problems and also adds an aesthetic touch.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item    |  Quantity | Manufacturer  | Part #                              | Price (USD) |    Sources[*][ppg]     | Notes                                                         |
|---------|----------:|---------------|-------------------------------------|------------:|:----------------------:|:--------------------------------------------------------------|
| **A1**  |         1 | RespiraWorks  | [Display panel support frame][a1rw] |   (?)150.00 |           Rw           | Display panel support frame, bent sheet metal                 |
| **A2**  |         4 | McMaster-Carr | 96439A650                           |        0.39 |      [C][a2mcmc]       | M4 self-clinching / press-fit nut                             |
| **A3**  | (~550cm²) | McMaster-Carr | 8560K357                            |  (150 / m²) |      [C][a3mcmc]       | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A4]` below |
| **A4**  |         1 | RespiraWorks  | [Display panel acrylic][a4rw]       |        8.25 |           Rw           | Upper acrylic face panel, cut from acrylic `[A3]`             |
| **A5**  |         1 | Eviciv        | DS702                               |       49.00 | [A][a5ali] [Z][a5amzn] | 7" capacitive touchscreen with speakers                       |
| **A6**  |         4 | McMaster-Carr | 93625A102                           |        0.11 |      [C][a6mcmc]       | M2.5 locknut                                                  |
| **A7**  |         4 | McMaster-Carr | 98689A111                           |        0.03 |      [C][a7mcmc]       | M2.5 washer, 5mm OD                                           |
| **A8**  |         4 | McMaster-Carr | 91116A110                           |        0.03 |      [C][a8mcmc]       | M2.5 washer, 8mm OD                                           |
| **A9**  |         4 | McMaster-Carr | 92095A504                           |        1.72 |      [C][a9mcmc]       | M2.5 screw, 14mm                                              |
| **A10** |         4 | McMaster-Carr | 94180A351                           |        0.15 |      [C][a10mcmc]      | Heat-set inserts for M4 screws                                |
| **A11** |         4 | McMaster-Carr | 93475A230                           |        0.02 |      [C][a11mcmc]      | M4 washer, 9mm OD                                             |
| **A12** |         4 | McMaster-Carr | 92095A193                           |        0.09 |      [C][a12mcmc]      | M4 screw, 14mm                                                |

**Total assembly price:** USD ~220.00

[a1rw]:   #display-panel-support-frame
[a2mcmc]: https://www.mcmaster.com/96439A650/
[a3mcmc]:  https://www.mcmaster.com/8560K357/
[a4rw]:    #display-panel-acrylic-plate
[a5ali]:   https://www.aliexpress.us/item/2255800561669994.html
[a5amzn]:  https://www.amazon.com/Eviciv-Portable-Monitor-Display-1024X600/dp/B07L6WT77H
[a6mcmc]:  https://www.mcmaster.com/93625A102/
[a7mcmc]:  https://www.mcmaster.com/98689A111/
[a8mcmc]:  https://www.mcmaster.com/91116A110/
[a9mcmc]: https://www.mcmaster.com/92095A504/
[a10mcmc]: https://www.mcmaster.com/94180A351/
[a11mcmc]: https://www.mcmaster.com/93475A230/
[a12mcmc]: https://www.mcmaster.com/92095A193/


## Custom Parts

All Inventor part models, assembly models and drawings are within this repo directory.

### Display Panel Support Frame

Recently exported files for sheet metal components can be found on our Google Drive at
[Manufacturing/Output files v0.4/Sheet metal](https://tinyurl.com/2p89pjja).

### Display Panel Acrylic Plate

Recently exported files for cutting acrylic can be found on our Google Drive at
[Manufacturing/Output files v0.4/Acrylic](https://tinyurl.com/4czevhxs).

## Assembly

Recently exported drawings for sub-assemblies can be found on our Google Drive at
[Manufacturing/Output files v0.4/Subassemblies](https://tinyurl.com/4d3s7zmw).

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

* Manufacture support frame with PEM nuts installed in appropriate locations `[A1-2]`
* Cut acrylic panel to shape `[A3-4]`.
* Install the heat-set inserts `[A10]`, while the acrylic panel is flat and no electronics are in the way of your and your soldering iron.
* Mount the acrylic to the support frame using M4 washers and screws `[A11-12]`
* Mount the touch-screen `[A5]` to the acrylic using M2.5 bolts and nuts `[A6-9]`. Use washers on both sides, bigger ones at the back.
* Connect the micro-HDMI ribbon cable included with the touch-screen. If yours did not come with one, you
  may need additional parts.
