# Equipment tray

|         Rendering         |         Drawing         |
|:-------------------------:|:-----------------------:|
| ![](images/rendering.jpg) | ![](images/drawing.jpg) |

## Design rationale

The equipment tray is meant to provide support to all the pneumatic components of the ventilator. It must fit snugly
into fasten robustly to the enclosure assembly.

The equipment bracket is folded from a single sheet. The bracket is designed to be simple and lightweight.
Acrylic sheets are either fastened to or hung from the bracket to allow for design adjustments while utilizing the same
bracket.

The acrylic pneumatics tray provides a continuous and easily modifiable surface for mounting pneumatics equipment. This
is optimal for prototype development, because the acrylic sheet can be easily and cheaply replaced or modified on the
spot with new holes or cut-outs for alternate components being tested.

In addition to supporting the main pneumatics tray, the equipment bracket also supports the display, electronics, filter
and rear input panels. Those are detailed in other assembly pages.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item   |   Quantity | Manufacturer    | Part #                    | Price (USD) | Sources[*][ppg] | Notes                                                         |
|--------|-----------:|-----------------|---------------------------|------------:|:---------------:|---------------------------------------------------------------|
| **A1** |          1 | RespiraWorks    | [Equipment bracket][a1rw] |      149.25 |       Rw        | Ventilator equipment bracket, bent sheet metal                |
| **A2** |         21 | McMaster-Carr   | 96439A650                 |        0.39 |   [C][a2mcmc]   | M4 self-clinching / press-fit nut                             |
| **A3** | (~1300cm²) | McMaster-Carr   | 8560K357                  |    150 / m² |   [C][a3mcmc]   | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A4]` below |
| **A4** |          1 | RespiraWorks    | [Pneumatics tray][a4rw]   |        19.5 |       Rw        | Custom cut acrylic panel, cut from acrylic `[A3]`             |
| **A5** |          9 | McMaster-Carr   | 92095A193                 |        0.18 |   [C][a5mcmc]   | M4 screw, 14mm: tray fasteners                                |
| **A6** |          9 | McMaster-Carr   | 93475A230                 |        0.02 |   [C][a6mcmc]   | M4 washers, 9mm OD                                            |

**Total assembly price:** USD 172.13

>**TODO: How much bulk sheet metal?**

[a1rw]:   #equipment-bracket
[a2mcmc]: https://www.mcmaster.com/96439A650/
[a3mcmc]: https://www.mcmaster.com/8560K357/
[a4rw]:   #pneumatics-tray
[a5mcmc]: https://www.mcmaster.com/92095A193/
[a6mcmc]: https://www.mcmaster.com/93475A230/

## Custom Parts

All Inventor part models, assembly models and drawings are within this repo directory.

### Equipment Bracket

Recently exported files for sheet metal components can be found on our Google Drive at
[Manufacturing/Output files v0.4/Sheet metal](https://tinyurl.com/2p89pjja).

The equipment bracket is made out of a folded 16 gauge (1.52mm thick), stainless steel sheet.

M4 self-clinching nuts `[A2]` nuts must be driven into the 21 locations.

### Pneumatics Tray

Recently exported files for cutting acrylic can be found on our Google Drive at
[Manufacturing/Output files v0.4/Acrylic](https://tinyurl.com/4czevhxs).

|                                   |                                 |
|-----------------------------------|---------------------------------|
| ![](images/acrylic_rendering.jpg) | ![](images/acrylic_drawing.jpg) |

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../methods).**

Attach the pneumatics tray `[A4]` to the equipment bracket `[A1]` using M4 hardware `[A5-6]`.

Recently exported drawings for sub-assemblies can be found on our Google Drive at
[Manufacturing/Output files v0.4/Subassemblies](https://tinyurl.com/4d3s7zmw).
