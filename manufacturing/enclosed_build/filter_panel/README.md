# Air filter panel

![](images/filter_panel_assembly_rendering.jpg)

## Lower Face Plate

**#TODO: present CAD links nicely**

This is a clear acrylic sheet that houses the filter assembly. It is mounted using heat inserts. Link to the part file
is given in [Source files](#Source-files) below. Some other useful files for the same part are
[.idw 2D drawing](CAD/LowerAcrylicFacePlate.idw), [.pdf 2D drawing](CAD/LowerAcrylicFacePlate.pdf),
[.dxf of the front face for laser cutting](CAD/LowerAcrylicFacePlate_FrontFace.dxf).

![](images/filter_panel_acrylic_plate_rendering.jpg)

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

| Item | Quantity  | Manufacturer  | Part #                              | Price (USD)         | Sources[*][ppg]| Notes |
| ---- |----------:| ------------- | ----------------------------------- | -------------------:|:----------:|:------|
|**A1**| 1         | RespiraWorks  | lower_acrylic_face_panel            |                     | [Rw][a1rw]  | Lower acrylic face panel |
|**A2**| 2         | RespiraWorks  | patient_tubing_filter_assembly      |                     | [Rw][a2rw]  | Filter housing assembly, variant with patient tubing |
|**A3**| 2         | RespiraWorks  | outlet_filter_assembly              |                     | [Rw][a3rw]  | Filter housing assembly, variant with outlet vent |
|**A4**| 4         | McMaster-Carr | 94180A351                           | 15.47 / 100         | [C][a4mcmc] | Heat-set inserts for m4 screws |
|**A5**| 16        | McMaster-Carr | 93625A150                           | 6.04 / 100          | [C][a5mcmc] | M4 lock nut |
|**A6**| 16        | McMaster-Carr | 93475A230                           | 1.86 / 100          | [C][a6mcmc] | M4 washer |
|**A7**| 16        | McMaster-Carr | 92000A224                           | 9.60 / 100          | [C][a7mcmc] | M4 14mm screw, phillips drive |
|**A8**| (16)      | McMaster-Carr | 92095A193                           | 8.95 / 100          | [C][a8mcmc] | M4 14mm screw, hex drive, **alternate to A7** |

[a1rw]: #lower-face-plate
[a2rw]: filter_holder
[a3rw]: filter_holder
[a4mcmc]: https://www.mcmaster.com/94180A351/
[a5mcmc]: https://www.mcmaster.com/93625A150/
[a6mcmc]: https://www.mcmaster.com/93475A230/
[a7mcmc]: https://www.mcmaster.com/92000A224/
[a8mcmc]: https://www.mcmaster.com/92095A193/

## Assembly Instructions

Disassemble the individual holders and integrate the housings with the acrylic panel. For latest
enclosed build, this requires M4x14mm screws (assuming 1/4" or 6.5mm acrylic panel).

![](images/panel1.jpg)

![](images/panel2.jpg)
