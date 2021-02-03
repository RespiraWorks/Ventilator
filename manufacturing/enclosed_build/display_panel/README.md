# Display panel

![](images/rendering.jpg)

## Upper Face Plate

**#TODO: explain design rationale and link to CAD files here. Make it as good or better as the other pages!**

The sizing for the mounting holes was based on [this](https://www.pemnet.com/fastening_products/pdf/sidata.pdf) catalog
for heat set insert.
M4 screws were chosen to mount the acrylic face plate to the sheet metal enclosure.
For corresponding M4 nuts (refer page 6, metric table, row 6), length code 1 was chosen since acrylic thickness
(6.35 mm) was more than minimum hole depth specification (5.72 mm).
Hence the corresponding outer diameter for insertion i.e. acrylic mounting diameter would be 5.94 mm, which is
reflected in the CAD.

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
|**A4** | (1)       | UPerfect      | -                        | 49.00        | [A][a4ali]  | 7" capacitive touchscreen, **DUPLICATE in [electrical assembly][elec]** |
|**A5** | 4         | McMaster-Carr | 93625A101                | 7.70 / 10    | [C][a5mcmc] | M2 locknut |
|**A6** | 8         | McMaster-Carr | 93475A195                | 1.06 / 100   | [C][a6mcmc] | M2 washer, 5mm OD |
|**A7** | (8)       | McMaster-Carr | 91116A240                | 2.40 / 100   | [C][a7mcmc] | M2 washer, 7mm OD, **alternate to A6** |
|**A8** | 4         | McMaster-Carr | 92000A019                | 6.75 / 100   | [C][a8mcmc] | M2 screw 12mm, phillips drive |
|**A9** | (4)       | McMaster-Carr | 92095A455                | 7.44 / 24    | [C][a9mcmc] | M2 screw 12mm, hex drive, **alternate to A8** |
|**A10**| 4         | McMaster-Carr | 94180A351                | 15.47 / 100  | [C][a10mcmc]| Heat-set inserts for m4 screws |

[a1rw]:    #upper-face-plate
[a2mcmc]:  https://www.mcmaster.com/7768K22/
[a3mcmc]:  https://www.mcmaster.com/50785K273/
[a4ali]:   https://www.aliexpress.com/item/4000747984746.html
[a5mcmc]:  https://www.mcmaster.com/93625A101/
[a6mcmc]:  https://www.mcmaster.com/93475A195/
[a7mcmc]:  https://www.mcmaster.com/91116A240/
[a8mcmc]:  https://www.mcmaster.com/92000A019/
[a9mcmc]:  https://www.mcmaster.com/92095A455/
[a10mcmc]: https://www.mcmaster.com/94180A351/

## Assembly Instructions

The brass threaded check valve (**A2**) interfaces with a through-wall straight connector (**A3**) that will be on the
enclosed ventilator's front panel.

On the opposite end of the fitting, a 1/4"ID tubing adapter (in pneumatics BOM) should be attached. Together with this adapter,
the oxygen input port should appear like this:

|  Front    |  Back   |
:------------------:|:-----------------:|
![](images/through-fitting_1.jpg)  |  ![](images/through-fitting_2.jpg)  |

**#TODO: Improve this, explain screen mounting**
