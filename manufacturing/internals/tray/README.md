# Equipment tray

| Front | Top |
:------------------:|:-----------------:|
| ![](images/rendering_front.jpg)  | ![](images/rendering_back.jpg)  |

## Design rationale

The equipment tray is meant to provide support to all the functional components of the ventilator. It must fit snugly
into fasten robustly to the enclosure assembly.

The equipment bracket is folded from a single sheet. The bracket is designed to be simple and lightweight.
Acrylic sheets are either fastened to or hung from the bracket to allow for design adjustments while utilizing the same
bracket.

The acrylic pneumatics tray provides a continuous and easily modifiable surface for mounting pneumatics equipment. This
is optimal for prototype development, because the acrylic sheet can be easily and cheaply replaced or modified on the
spot with new holes or cut-outs for alternate components being tested.

In addition to supporting the main pneumatics tray, the equipment bracket also supports the display and filter panels
on the front face of the ventilator. Those are detailed in other assembly pages.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer   | Part #                    | Price (USD) | Sources[*][ppg]| Notes |
| ----- |---------:| -------------- | ------------------------- | -----------:|:--------------:| ----- |
|**A1** | 1        | RespiraWorks   | [Equipment bracket][a1rw] | 149.25      | Rw             | Ventilator equipment bracket, bent sheet metal |
|**A2** | 6        | McMaster-Carr  | 96439A650                 | 0.39        | [C][a2mcmc]    | M4 self-clinching / press-fit nut |
|**A3** |(~1300cm²)| McMaster-Carr  | 8560K357                  | 150 / m²    | [C][a3mcmc]    | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A4]` below |
|**A4** | 1        | RespiraWorks   | [Pneumatics tray][a4rw]   | 19.5        | Rw             | Custom cut acrylic panel, cut from acrylic `[A3]` |
|**A5** | 4        | McMaster-Carr  | 92095A193                 | 0.18        | [C][a5mcmc]    | M4 screw, 14mm: tray fasteners |
|**A6** | 4        | McMaster-Carr  | 93625A150                 | 0.06        | [C][a6mcmc]    | M4 locknut |
|**A7** | 4        | McMaster-Carr  | 93475A230                 | 0.02        | [C][a7mcmc]    | M4 washers, 9mm OD |

**Total assembly price:** USD 172.13

>**TODO: How much sheet metal?**

[a1rw]:    #custom-parts
[a2mcmc]:  https://www.mcmaster.com/96439A650/
[a3mcmc]:  https://www.mcmaster.com/8560K357/
[a4rw]:   #parts
[a5mcmc]: https://www.mcmaster.com/92095A193/
[a6mcmc]: https://www.mcmaster.com/93625A150/
[a7mcmc]: https://www.mcmaster.com/93475A230/

## Custom Parts

### Equipment Bracket

|   |   |   |
|---|---|---|
| [![](images/fab_drawing_1.jpg)](exports/equipment_bracket.pdf) | [![](images/fab_drawing_2.jpg)](exports/equipment_bracket.pdf) | [![](images/fab_drawing_3.jpg)](exports/equipment_bracket.pdf) |

| Download type | Link   |
|:--------------|:-------|
| Engineering drawing           | [.idw](equipment_bracket.idw) [.pdf](exports/equipment_bracket.pdf) |
| Source: sheet metal CAD model | [.ipt](equipment_bracket.ipt) |
| Source: assembly CAD model    | [.iam](equipment_bracket.iam) |

The equipment bracket is made out of a folded, 1.52mm thick, stainless steel sheet.

M4 self-clinching nuts `[A2]` nuts must be driven into the 6 locations on the sides.

### Pneumatics Tray

|   |   |   |
|---|---|---|
| [![](images/pneumatics_tray.jpg)](exports/pneumatics_tray.pdf) | ![](images/rendering_acrylic.jpg) |

| Download type | Link   |
|:--------------|:-------|
| Fabrication drawing      | [.idw](pneumatics_tray.idw) [.pdf](exports/pneumatics_tray.pdf) |
| Export for laser cutting | [.dxf](exports/pneumatics_tray.dxf) |
| Source: part CAD model   | [.ipt](pneumatics_tray.ipt) |


## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../methods).**

Attach the pneumatics tray `[A4]` to the equipment bracket `[A1]` using M4 hardware `[A5-7]`.
