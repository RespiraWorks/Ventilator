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

### Source files

The custom design and assembly have been carried out in Autodesk Inventor, and the following "source" files are provided:

* [Filter assembly.iam](assets/Filter%20assembly.iam) - Inventor assembly file for a single filter holder
* [Filter housing.ipt](assets/Filter%20housing.ipt) - Inventor part file for filter housing/panel mount (pictured in purple above).
* [Filter cover.ipt](assets/Filter%20cover.ipt) - Inventor part file for filter cover (pictured in blue above), which has two configurations.
* [Filter gasket.ipt](assets/Filter%20gasket.ipt) - Inventor part file for filter gasket design
* [Filter stand-in.ipt](assets/Filter%20stand-in.ipt) - Stand-in for the actual HEPA filter.

Previous versions of these parts were designed in OnShape, and the history of those previous iterations can be found
[here](https://cad.onshape.com/documents/3fe0c1f79c482144c267173d/w/2ad1c08071a25185f9c78c68/e/c3b817c7fd516ff612068657).

### 3d printing

The following STL files have been exported for 3d printing:
* [Filter housing.stl](assets/Filter%20housing.stl) - should need 4 of these
* [Filter cover - patient connector.stl](assets/Filter%20cover%20-%20patient%20connector.stl) - should need 2 of these
* [Filter cover - air vent.stl](assets/Filter%20cover%20-%20air%20vent.stl) - should need 2 of these

Quantities identified above assume a full set of 4 filter assemblies for a single ventilator.

Please see the [3d printing guidelines](../3d_printing) for general advice.

![](assets/printed_set.jpg)

## Parts List (BOM)

**Note: If you are a member of the RespiraWorks team, review the
[part purchasing guidelines](../../manufacturing/README.md#part-purchasing-guidelines)
BEFORE purchasing any parts.**

### Purchasing Source Key

* **C** = McMaster-Carr
* **B** = Built by RespiraWorks
* **G** = Grainger
* **Z** = Amazon

### Parts

**Note:** The below quantities are for building 4 complete sets which are needed for one Ventilator.

| Item | Quantity  | Manufacturer  | Part                                | Price (USD)         | Sources    | Notes |
| ---- |----------:| ------------- | ----------------------------------- | -------------------:|:-----------|:------|
| 1    |         4 | RespiraWorks  | Filter housing                      | 1 (material)        | [B][1rw]   | 3D printed from STL in link |
| 2    |         2 | RespiraWorks  | Filter cover with patient connector | 1 (material)        | [B][2rw]   | 3D printed from STL in link |
| 3    |         2 | RespiraWorks  | Filter cover with vent              | 1 (material)        | [B][3rw]   | 3D printed from STL in link |
| 4    |         4 | Amyehouse     | 20160819-1a                         | 8/6pk               | [Z][4amzn] | Roomba 700-compatible HEPA filters |
| 5    |         8 | RespiraWorks  | Filter gasket                       | 0.10 (material)     | B          | make using item **6** below, using **a1+a2** |
| 6    | ~16 sq in | McMaster-Carr | 8785K82                             | 22.22/ 12x12" sheet | [C][6mcmc] | Gasket material, cut with jigs(**a1+a2**) to make item **5** |
| 7    |         8 | McMaster-Carr | 96016A556                           | 5.50/10pk           | [C][7mcmc] | m3 thumbscrews |
| 8    |         8 | McMaster-Carr | 94180A333                           | 16.69/100pk         | [C][8mcmc] | heat-set inserts for m3 screws |


[1rw]: assets/Filter%20housing.stl
[2rw]: assets/Filter%20cover%20-%20patient%20connector.stl
[3rw]: assets/Filter%20cover%20-%20air%20vent.stl
[4amzn]: https://www.amazon.com/gp/product/B01KNZCW8E
[6mcmc]: https://www.mcmaster.com/8785K82-8785K822/
[7mcmc]: https://www.mcmaster.com/96016A556-96016A831/
[8mcmc]: https://www.mcmaster.com/94180A333/

**Note:** items 5 is made in this procedure from the material in item 6

## Tools

| Item | Quantity | Manufacturer  | Part #                  | Price (USD)  | Sources     | Notes |
| ---- |---------:| ------------- | ------------------------| ------------:|:------------|:------|
| a1   |        1 | RespiraWorks  | Filter inside cut jig   | 1 (material) | [B][a1rw]   | 3D printed from STL in link |
| a2   |        1 | RespiraWorks  | Filter outside cut jig  | 1 (material) | [B][a2rw]   | 3D printed from STL in link |
| a3   |        1 | Fiskars       | 167110-1001             | 5.49         | [A][a3amzn] | Craft knife, for cutting gaskets |
| a4   |        1 | General       | 707475                  | 31.75        | [G][a4grai] | Needle file set (might be needed, depending on quality of 3d prints) |
| a5   |        1 | Walnut Hollow | 38283                   | 32.01        | [A][a5amzn] | Wood burning tool, or soldering iron if you have one, for installing inserts |

[a1rw]:   assets/filter-gasket-jig-inside.stl
[a2rw]:   assets/filter-gasket-jig-outside.stl
[a3amzn]: https://www.amazon.com/Fiskars-167110-1001-Heavy-Craft-Knife/dp/B001A42CTW
[a4grai]: https://www.grainger.com/product/23N602
[a5amzn]: https://www.amazon.com/Walnut-Hollow-Versa-Temp-Temperature-Woodburning/dp/B005P1TRAS

## Assembly Instructions

- Step 1: 3D print the parts and tools called out in the BOM.

- Step 2: Remove the roomba filters from the housings that they arrive in from the manufacturer.

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/roomba1.jpg) | ![](assets/roomba2.jpg) |


- Step 3:  Start installing the heat-set inserts (Item 8), pushing them halfway into filter housing (Item 1).
Now let the the plastic/resin cool off before pushing the inserts in all the way. This goes best if you get the inserts
halfway into each of the 4 units in turn and then come back to them.

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/insert1.jpg) | ![](assets/insert2.jpg) |


- Step 4: Cut the inner edge of the gasket in the gasket material using the inside cut jig (item a1).

|                            |                             |
|:--------------------------:|:---------------------------:|
|![](assets/MakeGasket2.jpg) | ![](assets/MakeGasket3.jpg) |

- Step 5: Place the outer cut jig (item a2) into the hole you just made.

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
