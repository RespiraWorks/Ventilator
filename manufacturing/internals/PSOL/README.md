# Proportional solenoid

Supply of oxygen is controlled by a proportional solenoid (PSOL). We are currently testing with 2 different solenoids.
Parts for both variants are listed below.

>**TODO: add more details**

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer   | Part #          | Price (USD) | Sources[*][ppg]| Notes |
| ----- |---------:| -------------- | --------------- | -----------:|:--------------:| ----- |
|**A1** | 1        | SMC Pneumatics | PVQ31-6G-40-01N | 96.10       | [Sm][a1smc]    | Proportional solenoid |
|**A2** | 2        | McMaster-Carr  | 5121K391        | 0.56        | [C][a2mcmc]    | 1/8 NPT x 1/4" ID barbed adapter **with `[A1]` only** |
|**A3** |(1)       | Uflow          | IBV19M          | 87.00       | [U][a3uflow]   | Proportional solenoid **alternate for `[A1]`** |
|**A4** |(2)       | McMaster-Carr  | 1786N249        | 8.16        | [C][a4mcmc]    | 1/8 BSPT x 1/4 NTP brass threaded pipe fitting **with `[A3]` only** |
|**A5** |(2)       | McMaster-Carr  | 5121K451        | 0.58        | [C][a5mcmc]    | 1/4 NPT x 1/4" ID barbed adapter **with [A3] only** |
|**A6** | 2        | McMaster-Carr  | 92095A182       | 0.08        | [C][a6mcmc]    | M3 screws, 10mm: for mounting to tray |
|**A7** | 2        | McMaster-Carr  | 91116A120       | 0.03        | [C][a7mcmc]    | M3 washers, 9mm OD: for mounting to tray |

**Total assembly price:** USD 97.44

[a1smc]:   https://www.smcpneumatics.com/PVQ31-6G-40-01N.html
[a2mcmc]:  https://www.mcmaster.com/5121K391/
[a3uflow]: https://www.uflowvalve.com/solenoid-proportional-flow-control-valve/product/2-2-way-miniature-high-flow-proportional-valve
[a4mcmc]:  https://www.mcmaster.com/1786N249/
[a5mcmc]:  https://www.mcmaster.com/5121K451/
[a6mcmc]:  https://www.mcmaster.com/92095A182/
[a7mcmc]: https://www.mcmaster.com/91116A120/

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../methods).**

Sub-assembly steps will differ depending on which proportional solenoid is used.

### Uflow IBV19M

Follow this section only if you are using the Uflow IBV19M `[A3]` solenoid.

You will need to create two M3 holes in opposite corners of the aluminium manifold as pictured:

| Marked | Tapped |
|:-------------------------:|:-------------------------:|
| ![](images/uflow_tapping_1.jpg) | ![](images/uflow_tapping_2.jpg) |

You will first want to drill 2.5mm holes about 5mm deep. Mark out the positions with a scribe and punch holes to help
guide the drill.

Drill with the PSOL secured in a drill press at 90 degrees, and with depth stop set precisely so as not to bore a hole
into the pneumatics channel thus creating a leak. If you do not have a 2.5mm drill bit, the closest SAE bits are
3/32" or 7/64".

Next you will have to tap M3x0.5 threads into those holes. Do so slowly, by hand. You can probably use the bottoming
tap from a kit like [this](https://www.amazon.com/uxcell-Metric-Straight-Flutes-Bottoming/dp/B00VGEG5GM). However, for
a professional tapping experience, [this kit](https://www.mcmaster.com/2608A13/) would be preferable, which  includes
both the drill bits and the proper taps.

You will need to drill 3.2mm holes in the appropriate positions on the pneumatics tray acrylic sheet, as those
provided in CAD drawings are positioned for the SMC solenoid.

You will also need to install the BSPT pipe adapters `[A4]` and the NPT tubing adapters `[A5]`. Remember to use
PTFE tape on the threads.

### General mounting instructions

Mounts to acrylic with 2x:
* M3x10 screws
* M3 washers 9mm OD

PSOL sub-assembly CAD: [.iam](PSOL.iam)

>**TODO: make CAD subassembly of Uflow PSOL and add more details**
