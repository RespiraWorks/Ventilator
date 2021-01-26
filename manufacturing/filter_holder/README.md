# Air filter assembly

![](assets/panel2.jpg)

## Design basis

The ventilator uses four mass-manufactured replaceable HEPA filter cartridges to isolate the patient from the device,
and to isolate the device gas path from the environment. These filters were selected based on research described in the
design overview document. Currently this form factor HEPA filter is available in quantities exceeding 10,000. A set of
custom panel-mount components houses this filter cartridge and serves as the connector for the inspiratory and
expiratory ends of the patient breathing circuit. A modified version of these connectors, utilizing the same filter,
is used as an additional barrier on the inspiratory and expiratory pathway to the device.

These parts are modular in design: the fixed component is identical for both patient tubing connectors and air
inlet/outlet ports. This allows the same part to be used for all four filter housings, decreasing part cost by
economies of scale and simplifying the assembly procedure. The fixed panel-mount component includes two M4 press-in or
heat-set threaded metal inserts to be used as a mounting point for the filter cartridge hold-down parts.

The replaceable filter cartridges are held in place by one of two component options - for the device inlet and outlet,
a simple screw-down tab holds the filter in place; for the patient breathing circuit connectors, a 22mm cone (male) port
is provided to interface with standard 22mm socket (female) breathing circuits.

## Implementation

- This is a panel mount filter housing, which allows the use of inexpensive vacuum HEPA filters in line between the
machine and the patient tubing.
- The filters we're using are from the Roomba 700 series. These are widely available and cost around $0.50/unit. To use
them, we're removing the actual filter from the yellow plastic housing. We expect we'll be able to order the filters
themselves this way in bulk in the future.
- Each will require two custom gaskets, made of either 3 mm EPDM foam or 1.5 mm <60 durometer rubber,
- The panel mount mounts with 4x m3 screws (length depends on mounting base thickness).
- The top and bottom connect by 2x m3x20 thumb screws, which go into threaded inserts. Right now we're using standard
McMaster heat-set inserts; for production we'll consult with the manufacturer. We might upgrade from m3 to m4.

## Custom parts

There are two types of filter assemblies to be used in our ventilator. The patient connector variety has a filter cover
with an integrated patient tubing adapter for air supplied to or returning from the patient. The air inlet/outlet
variety is for ambient air input and exhaled air exhaust.

| Patient connector |     | Air inlet/outlet  |     |
|:-----------------:|:---:|:-----------------:|:---:|
|![](assets/rendered_patient_solid.jpg) |![](assets/rendered_patient_cut.jpg) |![](assets/rendered_outlet_solid.jpg) |![](assets/rendered_outlet_cut.jpg) |

Each of the custom components are described below.

### Filter housing

The filer housing - the lower, purple part pictured in renderings above - is common to both variations of assemblies.
The housing mounts to the filter acrylic panel on the front of the ventilator assembly. Its backside contains a
single-barb adapter for 3/4" tubing internal to the ventilator pneumatic assembly. The HEPA filter holding cavity has a
platform matching the dimensions of the filter gasket. The walls of the cavity should hug the filter with no about
0.5mm of extra space on the sides.

Holes for panel mounting are made for M4 screws, with 0.1mm tolerance, spaced 52.5mm apart horizontally and 23mm vertically.

Holes for heat-set inserts are 5.5mm in diameter, adapters for the M3 inserts in the BOM below, and are spaced 51mm apart.

### Filter cover

The filter cover goes over the filter housing to squeeze the filter between the two gaskets and ensure that all air goes
through the filter. This is the upper, blue part pictured in renderings above. It also includes a cavity to hold the
gasket and filter precisely in place. The filter cover comes in two varieties - one for with adapters for patient
tubing, and the other - with vents for air in/out-flow.

Most recent changes also include thickening of the walls to make the 3d prints less fragile.

### Lower Face Plate

This is a clear acrylic sheet that houses the filter assembly. It is mounted using heat inserts. Link to the part file is given in [Source files](#Source-files) below. Some other useful files for the same part are [.idw 2D drawing](CAD/LowerAcrylicFacePlate.idw), [.pdf 2D drawing](CAD/LowerAcrylicFacePlate.pdf),[.dxf of the front face for laser cutting](CAD/LowerAcrylicFacePlate_FrontFace.dxf).

![](assets/LowerAcrylicFacePlate.jpg)

The sizing for the mounting holes was based on [this](https://www.pemnet.com/fastening_products/pdf/sidata.pdf) catalog for heat set insert. 
M4 screws were chosen to mount the acrylic face plate to the sheet metal enclosure. 
For corresponding M4 nuts (refer page 6, metric table, row 6), length code 1 was chosen since acrylic thickness (6.35 mm) was more than minimum hole depth specification (5.72 mm).
Hence the corresponding outer diamater for insertion i.e. acrylic mounting diamater would be 5.94 mm, which is refelcted in the CAD.

### Source files

The custom design and assembly have been carried out in Autodesk Inventor, and the following "source" files are provided:

* [Filter assembly.iam](CAD/Filter%20assembly.iam) - Inventor assembly file for a single filter holder
* [Filter housing.ipt](CAD/Filter%20housing.ipt) - Inventor part file for filter housing/panel mount (pictured in purple above).
* [Filter cover.ipt](CAD/Filter%20cover.ipt) - Inventor part file for filter cover (pictured in blue above), which has two configurations.
* [Filter gasket.ipt](CAD/Filter%20gasket.ipt) - Inventor part file for filter gasket design
* [Filter stand-in.ipt](CAD/Filter%20stand-in.ipt) - Stand-in for the actual HEPA filter.
* [Lower acrylic face plate.ipt](CAD/LowerAcrylicFacePlate.ipt) - Inventor part file for lower acrylic face plate.

Previous versions of these parts were designed in OnShape, and the history of those previous iterations can be found
[here](https://cad.onshape.com/documents/3fe0c1f79c482144c267173d/w/2ad1c08071a25185f9c78c68/e/c3b817c7fd516ff612068657).

### 3d printing

The following STL files have been exported for 3d printing:
* [Filter housing.stl](CAD/Filter%20housing.stl) - should need 4 of these
* [Filter cover - patient connector.stl](CAD/Filter%20cover%20-%20patient%20connector.stl) - should need 2 of these
* [Filter cover - air vent.stl](CAD/Filter%20cover%20-%20air%20vent.stl) - should need 2 of these

Quantities identified above assume a full set of 4 filter assemblies for a single ventilator.

Please see the [3d printing guidelines](../3d_printing) for general advice.

![](assets/printed_set.jpg)

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../manufacturing/purchasing_guidelines.md

**Note:** The below quantities are for building a complete set of 4 filter housings needed for one ventilator assembly.

| Item | Quantity  | Manufacturer  | Part #                              | Price (USD)         | Sources[*][ppg]| Notes |
| ---- |----------:| ------------- | ----------------------------------- | -------------------:|:----------:|:------|
|**A1**| 4         | RespiraWorks  | Filter housing                      | 1 (material)        | [Rw][a1rw]  | 3D printed from STL in link |
|**A2**| 2         | RespiraWorks  | Filter cover with patient connector | 1 (material)        | [Rw][a2rw]  | 3D printed from STL in link |
|**A3**| 2         | RespiraWorks  | Filter cover with vent              | 1 (material)        | [Rw][a3rw]  | 3D printed from STL in link |
|**A4**| 4         | Amyehouse     | 20160819-1a                         | 8 / 6pk             | [Z][a4amzn] | Roomba 700-compatible HEPA filters |
|**A5**| ~16 sq in | McMaster-Carr | 8785K82                             | 22.22/ 12x12" sheet | [C][a5mcmc] | Gasket material, cut with jigs(**B1+B2**) to make **A6** |
|**A6**| 8         | RespiraWorks  | filter_gasket                       | 0.10 (material)     | Rw          | Filter gaskets, made from **A5**, using jigs **B1+B2** |
|**A7**| 8         | McMaster-Carr | 96016A556                           | 5.50 / 10           | [C][a7mcmc] | m3 thumbscrews |
|**A8**| 8         | McMaster-Carr | 94180A333                           | 16.69 / 100         | [C][a8mcmc] | heat-set inserts for m3 screws |
|**A6**| 8         | RespiraWorks  | filter_gasket                       | 0.10 (material)     | B           | Filter gaskets, made from **A5**, using jigs **B1+B2** |
|**A7**| 8         | McMaster-Carr | 96016A556                           | 5.50 / 10           | [C][a7mcmc] | M3 thumbscrews |
|**A8**| 8         | McMaster-Carr | 94180A333                           | 16.69 / 100         | [C][a8mcmc] | Heat-set inserts for m3 screws |
|**A9**| 8         | Penn Engineering | IUA-M4-1                         |                     | [C][a9penn] | Heat-set inserts for m4 screws |

[a1rw]: CAD/Filter%20housing.stl
[a2rw]: CAD/Filter%20cover%20-%20patient%20connector.stl
[a3rw]: CAD/Filter%20cover%20-%20air%20vent.stl
[a4amzn]: https://www.amazon.com/gp/product/B01KNZCW8E
[a5mcmc]: https://www.mcmaster.com/8785K82/
[a7mcmc]: https://www.mcmaster.com/96016A556/
[a8mcmc]: https://www.mcmaster.com/94180A333/
[a9penn]: https://catalog.pemnet.com/item/tapered-thru-threaded-inserts-types-iua-iub-iuc/red-thru-threaded-inserts-types-iua-iub-iuc-metric/22398

## Tools

| Item | Quantity | Manufacturer  | Part #                  | Price (USD)  | Sources[*][ppg]| Notes |
| ---- |---------:| ------------- | ------------------------| ------------:|:-----------:|:------|
|**B1**| 1        | RespiraWorks  | Filter inside cut jig   | 1 (material) | [Rw][b1rw]  | 3D printed from STL in link |
|**B2**| 1        | RespiraWorks  | Filter outside cut jig  | 1 (material) | [Rw][b2rw]  | 3D printed from STL in link |
|**B3**| 1        | Fiskars       | 167110-1001             | 5.49         | [A][b3amzn] | Craft knife, for cutting gaskets |
|**B4**| 1        | General       | 707475                  | 31.75        | [G][b4grai] | Needle file set (might be needed, depending on quality of 3d prints) |
|**B5**| 1        | Walnut Hollow | 38283                   | 32.01        | [A][b5amzn] | Wood burning tool, or soldering iron if you have one, for installing inserts |

[b1rw]:   CAD/filter-gasket-jig-inside.stl
[b2rw]:   CAD/filter-gasket-jig-outside.stl
[b3amzn]: https://www.amazon.com/Fiskars-167110-1001-Heavy-Craft-Knife/dp/B001A42CTW
[b4grai]: https://www.grainger.com/product/23N602
[b5amzn]: https://www.amazon.com/Walnut-Hollow-Versa-Temp-Temperature-Woodburning/dp/B005P1TRAS

## Assembly Instructions

- Step 1: 3D print the parts and tools called out in the [Parts list](#parts).

- Step 2: Remove the filters (**A4**) from the housings that they arrive in from the manufacturer.

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/roomba1.jpg) | ![](assets/roomba2.jpg) |


- Step 3:  Start installing the heat-set inserts (**A8**), pushing them halfway into filter housing (**A1**).
Now let the the plastic/resin cool off before pushing the inserts in all the way. This goes best if you get the inserts
halfway into each of the 4 units in turn and then come back to them.

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/insert1.jpg) | ![](assets/insert2.jpg) |


- Step 4: Cut the inner edge of the gasket in the gasket material using the inside cut jig (**B1**).

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/MakeGasket2.jpg) | ![](assets/MakeGasket3.jpg) |

- Step 5: Place the outer cut jig (**B2**) into the hole you just made.

![](assets/MakeGasket4.jpg)

- Step 6: Cut the outer edge of the gasket.

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/MakeGasket5.jpg) | ![](assets/MakeGasket6.jpg) |

- Step 7: Repeat steps above to produce a total of 8 gaskets for 4 filter assemblies

- Step 8: Test the fit of all filter assemblies with gaskets and filters. You may need to sand or file the printed
housing components to ensure that all edges and corners are clean to maintain a tight seal.

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/parts.jpg) | ![](assets/assembled.jpg) |

- Step 9: Disassemble the individual holders and instead integrate the housings with the acrylic panel. For latest
enclosed build, this requires M4x14mm screws (assuming 1/4" or 6.5mm acrylic panel).

![](assets/panel1.jpg)

![](assets/panel2.jpg)
