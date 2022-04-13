# Back panel

|                           |                         |
|:-------------------------:|:-----------------------:|
| ![](images/rendering.jpg) | ![](images/drawing.jpg) |


## Design rationale

> TODO

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

> TODO Add USB port

> TODO Add Ethernet port

> TODO Add alarm battery access port

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item   |  Quantity | Manufacturer  | Part #                     | Price (USD) | Sources[*][ppg] | Notes                                                         |
|--------|----------:|---------------|----------------------------|------------:|:---------------:|:--------------------------------------------------------------|
| **A1** | (~250cm²) | McMaster-Carr | 8560K357                   |    150 / m² |   [C][a1mcmc]   | 1/4" (6.35mm) thick clear acrylic sheet, to make `[A2]` below |
| **A2** |         1 | RespiraWorks  | [Back panel acrylic][a2rw] |        3.75 |       Rw        | Back panel acrylic plate, cut from acrylic `[A1]`             |
| **A3** |         1 | McMaster-Carr | 7880T345                   |       11.80 |   [C][a3mcmc]   | Push-to-Connect Tube Fitting                                  |
| **A4** |         6 | McMaster-Carr | 94180A351                  |        0.15 |   [C][a4mcmc]   | Heat-set inserts for M4 screws                                |
| **A5** |         1 | RespiraWorks  | [Main power harness][a5rw] |     8.61(?) |       Rw        | Main power wire harness, with 4pin DIN female plug            |
| **A6** |         2 | McMaster-Carr | 94500A298                  |        0.14 |   [C][a6mcmc]   | M2.5 screw, 14mm                                              |
| **A7** |         2 | McMaster-Carr | 94205A101                  |        0.25 |   [C][a7mcmc]   | M2.5 locknut                                                  |

**Total assembly price:** USD ~25.84

[a1mcmc]:  https://www.mcmaster.com/8560K357/
[a2rw]:    #custom-parts
[a3mcmc]:  https://www.mcmaster.com/7880T345/
[a4mcmc]: https://www.mcmaster.com/94180A351/
[a5rw]:    ../../wiring/README.md#main-power-input
[a6mcmc]:  https://www.mcmaster.com/94500A298/
[a7mcmc]:  https://www.mcmaster.com/94205A101/


## Custom Parts

All Inventor part models, assembly models and drawings are within this repo directory.

### Back Panel Acrylic Plate

Recently exported files for cutting acrylic can be found on our Google Drive at
[Manufacturing/Output files v0.4/Acrylic](https://tinyurl.com/4czevhxs).

## Assembly

Recently exported drawings for sub-assemblies can be found on our Google Drive at
[Manufacturing/Output files v0.4/Subassemblies](https://tinyurl.com/4d3s7zmw).

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

* You should first install the heat-set inserts `[A4]`, while the acrylic panel is flat and no electronics or fitting
  are in the way of your and your soldering iron.

* Install the through-fitting `[A3]` such that the push-to-connect port is to the inside of the panel.

* The main power wire harness `[A5]` terminates with a female 4-pin DIN connector. Mount this so that the receptacle faces to the outside of the panel. Orient the DIN socket with the locking pin facing up. Use M2.5 screws `[A6]` and locknuts `[A7]`.
