# Display panel

| Assembly CAD Rendering               | Assembled Back View|
|:-------------------------:|:-------------------------:|
| ![](images/display_panel_assembly_rendering.jpg) | ![](images/through-fitting_2.jpg) |

|Download Type|Link|
|:------|:-------|
|Display Panel Assembly CAD|[.iam](display_panel_assembly.iam)|
|Display Assembly CAD|[.iam](display_assembly.ipt)|
|#TODO - Display Panel Assembly Drawing PDF|

## Overview

This readme contains the details, BOM, assembly instructions for one display panel assembly for the enclosed build. 

## Design Rationale
The display panel accommodates the LCD touchscreen display and the oxygen inlet port. The display panel assembly should provide an easily accessible way to the primary user interface - the touchscreen display. It accomplishes this by mounting the display assembly in a custom cut transparent acrylic sheet which also allows one to view the internals.  
### Display Panel mounting choice 
It was decided that heat set inserts be used to help screw the display panel to the sheet metal enclosure tray. This was done since the holes in the acrylic lacked any internal threads. Tapped/plastic threads are easy to overload and strip out. Metal threads provide a reliable way of fastening plastic and acrylic parts. The heat set inserts will resist pull-out and torque-out loads due to knurled external surface features designed to interface with the bulk acrylic. The sizing for the mounting holes was based on [this](https://www.pemnet.com/fastening_products/pdf/sidata.pdf) catalog for heat set insert. M4 screws were chosen to mount the acrylic face plate to the sheet metal enclosure. For corresponding M4 nuts (refer page 6, metric table, row 6), length code 1 was chosen since acrylic thickness (6.35 mm) was more than minimum hole depth specification (5.72 mm). Hence the corresponding outer diameter for insertion i.e. acrylic mounting diameter would be 5.94 mm, which is reflected in the CAD.
### LCD Display mounting choice
The heat set inserts were not deemed necessary for the holes used to mount the LCD display to the acrylic plate due to the LCD's lighter weight. M2 screws, washers and locknuts were used; details below. Standard clearance hole for an M2 screw is 2.4mm which is reflected in the CAD.
### Oxygen Inlet choice
This ventilator relies on an external source of oxygen. The inlet port consists of a brass threaded check valve which allows only unidirectional flow into the ventilator. A barbed tubing adapter connects the internal pneumatic assembly to the oxygen source. The check valve and tubing adapter are connected through the acrylic plate via a through-wall straight connector that consists of a washer and nut in addition to the connector itself. Brass fittings were chosen since they offer superb corrosion resistance, a desirable trait in a high oxygen content environment, in addition to ability to withstand high pressures.

## Custom Parts

The only custom part in this assembly is the custom cut display acrylic. For our build, we used *insert type of acrylic here* and used *insert laser cutting machine specs here* to cut the acrylic. The necessary files are presented below:

|Download Type|Link|
|:------|:-------|
|Display Panel Acrylic Plate CAD|[.ipt](display_panel_acrylic_plate.ipt)|
|Display Panel Acrylic Plate drawing|[.dwg](display_panel_acrylic_plate.dwg)|
|Display Panel Acrylic Plate drawing|[.pdf](display_panel_acrylic_plate.pdf)|
|Display Panel Acrylic Plate for laser cutting|[.dxf](display_panel_acrylic_plate.dxf)|

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md
[pneu]: ../pneumatics
[elec]: ../../electrical

| Item  | Quantity  | Manufacturer  | Part #                   | Price (USD)  | Sources[*][ppg]| Notes |
| ----- |----------:| ------------- | ------------------------ | ------------:|:----------:|:------|
|**A1** | 1         | RespiraWorks  | upper_acrylic_face_panel |              | [Rw][a1rw]  | Upper acrylic face panel |
|**A2** | (1)       | McMaster-Carr | 7768K22                  | 11.62        | [C][a2mcmc] | brass threaded check valve, **DUPLICATE in [pneumatic assembly][pneu]** |
|**A3** | (1)       | McMaster-Carr | 50785K273                | 8.23         | [C][a3mcmc] | through-wall straight connector, 1/4NPT female, **DUPLICATE in [pneumatic assembly][pneu]** |
|**A4**| 1        | McMaster-Carr  | 5121K451            | 5.78 / 10    | [C][a4mcmc]   | 1/4 NPT x 1/4" ID barbed adapter |
|**A5** | (1)       | UPerfect      | -                        | 49.00        | [A][a5ali]  | 7" capacitive touchscreen, **DUPLICATE in [electrical assembly][elec]** |
|**A6** | 4         | McMaster-Carr | 93625A101                | 7.70 / 10    | [C][a6mcmc] | M2 locknut |
|**A7** | 8         | McMaster-Carr | 93475A195                | 1.06 / 100   | [C][a7mcmc] | M2 washer, 5mm OD |
|**A8** | (8)       | McMaster-Carr | 91116A240                | 2.40 / 100   | [C][a8mcmc] | M2 washer, 7mm OD, **alternate to A6** |
|**A9** | 4         | McMaster-Carr | 92000A019                | 6.75 / 100   | [C][a9mcmc] | M2 screw 12mm, phillips drive |
|**A10** | (4)       | McMaster-Carr | 92095A455                | 7.44 / 24    | [C][a10mcmc] | M2 screw 12mm, hex drive, **alternate to A8** |
|**A11**| 4         | McMaster-Carr | 94180A351                | 15.47 / 100  | [C][a11mcmc]| Heat-set inserts for M4 screws |
 

[a1rw]:    #custom-parts
[a2mcmc]:  https://www.mcmaster.com/7768K22/
[a3mcmc]:  https://www.mcmaster.com/50785K273/
[a4mcmc]:  https://www.mcmaster.com/5121K451
[a5ali]:   https://www.aliexpress.com/item/4000747984746.html
[a6mcmc]:  https://www.mcmaster.com/93625A101/
[a7mcmc]:  https://www.mcmaster.com/93475A195/
[a8mcmc]:  https://www.mcmaster.com/91116A240/
[a9mcmc]:  https://www.mcmaster.com/92000A019/
[a10mcmc]: https://www.mcmaster.com/92095A455/
[a11mcmc]: https://www.mcmaster.com/94180A351/

## Assembly Instructions

**#TODO: Improve this, explain screen mounting**  

To install the oxygen parts of the oxygen inlet system:
1. The brass threaded check valve (**A2**) interfaces with a through-wall straight connector (**A3**) that will be on the
enclosed ventilator's front panel.
2. On the opposite end of the fitting, a 1/4"ID tubing adapter (**A4**) should be attached. Together with this adapter,
the oxygen input port should appear like this:

|  Front    |  Back   |
:------------------:|:-----------------:|
![](images/through-fitting_1.jpg)  |  ![](images/through-fitting_2.jpg)  |

To install the heat set inserts (**A11**), you will need a soldering iron.
1. Select a soldering iron tip large enough to provide uniform pressure on the center of the heat-set insert, but small enough to not touch the surrounding material.
2. Heat up yoru soldering iron to an operating temperature of around 343-399&deg;C (650-750&deg;F).
3. Place the heat-set insert (**A11**) into the mounting hole (one of the four corner holes on the display acrylic) with the smaller diamater facing down into the hole away from you.
4. Touch the tip of the soldering iron to the top of the heat-set insert and wait for a few seconds. 
5. The weight of the soldering iron should gently push the heat-set insert into the now melted acrylic.
6. Press down slowly until the top of the heat-set insert is flush with the face of the acrylic.
7. Remove heat and allow to cool and harden.  
