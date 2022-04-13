# Venturi Flow Sensor

| Cross-section                     | Solid                    |
|:---------------------------------:|:------------------------:|
| ![](images/v1_cross_section.jpg)  | ![](images/v1_solid.jpg) |

## Design basis

The venturi is used to measure flow by measuring a change in pressure as the flow accelerates through the throat.
Relative to other pressure sensor types, a venturi flow meter produces the largest signal pressure at the smallest
pressure loss. This allows significantly cheaper and more widely available pressure sensors to be used, because a
signal at peak flow can be 5 to 15 times larger than the flow resistance. For a resistance type flow sensor, the ratio
is 1:1. The change in pressure is measured between the two ports, and this is correlated to the flow rate through the
orifice. It is important to note that as the flow expands, almost all of this pressure is recovered to the original
flow; the pressure signal measured is not the same as the pressure resistance to flow.

### Mechanical Implementation

Currently the prototype parts are 3D printed using an SLA printer in ABS-like resin. Ultimately this design requires
some modification to make it amenable to injection molding, which is the intended method for at-scale production.
Venturis like these are commonly injection molded, so there is precedent for successful injection molding of parts with
the required internal profile. However, the team did not identify any off-the-shelf parts that met the orifice sizing
requirements dictated by the ventilatory flow rates required, leading to a custom design for this part. This part
incorporates molded-in hose barbs, which are compatible with either 3/4"ID or 19mm ID flexible tubing depending on
supply chain availability.

The venturi outer diameter is defined by the tubing standard. The current design is made for 3/4" ID tubing.

The pressure sensor we are using, the [MPXV5004DP](https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf)
has 2.5 mm or 3/32" barbs. Note that this is not sufficiently close in size to use more common 3mm or 1/8" tubing.
This model has printed threads to accept an adapter from #10-32 pipe to 3/32" adapter. Other adapters could be
incorporated eventually by changing the interface design.

## Version history

### Alpha

This was the first proof of concept, tested with arduino code. It was made for 5/8"(~16mm) ID tubing.

### Beta

It was decided that the next iteration of the ventilator will use 3/4"(~19mm) ID tubing throughout. The venturi was
adjusted to match.

Relative to the 16mm sensor, it has a slightly larger orifice (design 5.50 mm) and a more aggressive chamfer at the
inlet to improve the aerodynamic flow.

An arrow was added to indicate the direction of flow.

The venturi was made asymmetrical, with a tighter constriction on entrance than exit.

### Version 1

It was decided to consolidate all CAD for better configuration and BOM management. To this end, the venturi has been
redesigned in AutoDesk Inventor. This marks the start of formal design tracking for this component.

Tube length has been moderately increased to decrease turbulence (user parameter-controlled).

Tubing interface barbs have been improved for better clamping.

Barb interfaces can easily be modified to interface with tubing of various diameters. Most recent prototypes have
shown that variation in tubing diameter could not be avoided and some of these transitions may have to be integrated
into manifolds or components such as the venturi to simplify the manufacturing process and make the ventilator more
compact.

The CAD includes optional features that can be enabled/disabled, such as integrated U-turn manifolds required for
the latest enclosed build. A number of different configurations have been exported below.

## Parametric model

Recently exported files for 3d printing can be found [here](https://tinyurl.com/2kuwps9h).

The original model with constraints is an AutoDesk Inventor part file. Parameters may be changed to
include transitions and barbs for different diameter tubing, as described in the table below. Assembly model and drawings
are all within this repository directory as well.

**Note: the venturi configurations are saved as iPart Configurations, which can be accessed by expanding the "Tables" tab in the Feature Tree of the venturi .ipt part.**

Below are a number of variants of the flow meter as configured for integration into the pneumatic system. They
incorporate tubing diameter transitions up- and down-stream of pinch valves as well as for narrower oxygen influx
tubing. Some have also been elongated (within the limits of what the enclosure space allows). The elongation and
elimination of discrete reducers should help mitigate turbulence and thus improve sensor precision.
Empirical testing is pending.

Listed variables are the parameters that need to be configured in the CAD in order to produce this particular variation
of the part. The variables `diams_before_venturi` and `diams_after_venturi` define additional length of tubing
before and after the tapering to venturi constriction in multiples of inner tube diameter. Greater numbers should
result in less turbulence and better sensor performance.

Note that explicit clamp space may not need to be provided when tubing diameter matches the 3/4" outer diameter
of the main venturi flow sensor body. In such cases, additional clamping also adds to straight tubing length dictated
by the two variables described above.

|                               |               Generic               |             Air influx              |             Oxygen influx              |          Exhale sensor          |
|:-----------------------------:|:-----------------------------------:|:-----------------------------------:|:--------------------------------------:|:-------------------------------:|
|            **Use**            |   General testing / "Pizza build"   |           Enclosed build            |             Enclosed build             |         Enclosed build          |
|           **Image**           | ![](images/venturi_generic.jpg)     | ![](images/venturi_air_influx.jpg)  | ![](images/venturi_oxygen_influx.jpg)  | ![](images/venturi_exhale.jpg)  |
|     diams_before_venturi      |                 2.0                 |                 2.5                 |                  2.5                   |               2.0               |
|      diams_after_venturi      |                 0.5                 |                 1.0                 |                  1.0                   |               1.0               |
|     input_outer_diameter      |                3/4"                 |                3/8"                 |                  1/4"                  |              3/4"               |
|           input_thk           |                 2mm                 |                1.5mm                |                 1.5mm                  |               2mm               |
|    input_clamp_nominal_len    |                 0mm                 |                12mm                 |                  12mm                  |              12mm               |
| Straight output barb features |               enabled               |              disabled               |                disabled                |             enabled             |
|     output_outer_diameter     |                3/4"                 |                  -                  |                   -                    |              3/8"               |
|          output_thk           |                 2mm                 |                  -                  |                   -                    |              1.5mm              |
|   output_clamp_nominal_len    |                 0mm                 |                  -                  |                   -                    |              12mm               |
|     Left U-turn features      |              disabled               |               enabled               |                disabled                |            disabled             |
|     Right U-turn features     |              disabled               |              disabled               |                enabled                 |            disabled             |
|   u_turn_parallel_distance    |                  -                  |                10mm                 |                  10mm                  |                -                |
|    u_turn_output_tube_len     |                  -                  |                30mm                 |                  30mm                  |                -                |
|       **Total length**        |               106.1mm               |                143mm                |                144.6mm                 |             142.5mm             |

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item | Quantity | Manufacturer  | Part #               | Price (USD) | Sources[*][ppg] | Notes                                              |
| ---- |---------:| ------------- | -------------------- | -----------:|:---------------:|----------------------------------------------------|
|**A1**| 1        | RespiraWorks  | [Venturi body][a1rw] | ~1.00       | Rw              | 3D-printed venturi body                            |
|**A2**| 2        | McMaster-Carr | 5463K3               | 0.39        | [C][a2mcmc]     | Barbed fitting 3/32" UNF, nipple for sensor tubing |

**Total assembly price:** USD 1.78

[a1rw]:   #parametric-model
[a2mcmc]: https://www.mcmaster.com/5463K33

## Tools

These may or may not be useful.

| Item | Manufacturer  | Part #          | Price (USD) | Sources[*][ppg] | Notes                                                  |
| ---- | ------------- | --------------- | -----------:|:---------------:|--------------------------------------------------------|
|**B1**| McMaster-Carr | 2636A251        | 6.76        | [C][b1mcmc]     | 10-32 UNF bottoming tap for threading nipple ports     |
|**B2**| McMaster-Carr | 2636A251        | 6.76        | [C][b2mcmc]     | 10-32 UNF tapered tap for threading nipple ports       |
|**B3**| uxcell        | a19032000ux0738 | 6.89        | [Z][b3amzn]     | 5.5 reamer for ensuring venturi diameter, **optional** |
|**B4**| DeWalt        | DWA1205         | 2.58        | [Z][b4amzn]     | 5/64" drill bit, should optimally be 2mm, **optional** |

[b1mcmc]: https://www.mcmaster.com/26955A86/
[b2mcmc]: https://www.mcmaster.com/2636A251
[b3amzn]: https://www.amazon.com/gp/product/B07QQPJB1V
[b4amzn]: https://www.amazon.com/DEWALT-DWA1205-Pilot-Industrial-Cobalt/dp/B015J5HN2S

## Manufacturing instructions

**Note: Please see general advice in the [manufacturing methods guide](../../methods).**

An Inventor CAD Assembly Model can be found [here](venturi_assembly.iam).

### 3D Printing

These should not be printed with FDM. Instead, a leak-tight method like SLA or MJF should be used. The model
contains printed threads and is designed to interface with the referenced mcmaster barbed fitting (item 2 below).

When slicing, it is best to position the part vertically to maintain authentic tapering and orifice geometry.
No scaffolding should be required. Part has been successfully printed with Siraya Fast of various colors.

|           |             |
|:---------:|:-----------:|
|![](images/venturi_assembly_rendering.jpg) |![](images/v1_assembly_tools.jpg)|

### Post-printing

Note that the flow measurement is very sensitive to the exact geometry, particularly the thread geometry. 3D printing
may result in variable shrinkage or expansion. This may cause uncertainty in the throat size, and thus the flow rates.
Threads for sensor connection may also not be perfectly formed.

You might therefore want to use mechanical tools to ensure all orifices are of uniform geometry:
* Use 2mm (or 5/64") drill bit `[B4]` to ensure sensor port holes clearly reach the main venturi tube
* Use reamer `[B3]` above) to carefully ream the venturi constriction to be precisely 5.5mm.
* Threads should print precisely enough that you should be able to thread the nipples into them with a little effort
  and achieve an air-tight fit. However, if they did not come out as such, you may use a tapering
  tap, such as `[B1]` or `[B2]` to chase the printed threads.

Screw the nipples `[A2]` into the 3d-printed venturi `[A1]`. You may require an adjustable wrench.

**Note: reamer may be too short to reach orifice in case of modified designs for the enclosure. Geometry discrepancies
will likely have to be compensated for in software using calibration lookup tables.**

### Verification

Check for leaks by plugging the holes and pulling some vacuum.

* Plug one large end, plug both nipples, suck on the remaining end.  If you have a vacuum pump handy do a regular vacuum hold leak test.
* Or, blow into each port with your fingers over the other three.

## System integration

Connecting tubes to pressure sensors should be covered in the general ventilator prototype assembly instructions.

### Transfer Function

To go from voltage to flow rate, you must correlate the flow rate to a change in pressure, and the change in pressure to a voltage.

The fundamental equation for a venturi flow meter is:

![Equation](https://wikimedia.org/api/rest_v1/media/math/render/svg/164d293785917d5d5bb07cdb01d96bafbc603a61)

However, this assumes perfect aerodynamics. A calibration factor is required to increase the accuracy of the signal.
Unfortunately this calibration factor is going to be dependant on the quality of your printer, or eventually, the
reproducibility of the manufacturing method. We are in the process of producing a calibration factor for the 19mm and
16mm sensors.

You then need to convert from dP to voltage, if using an analog sensor, or from dP to serial, if using an I2C sensor.

You can use
[this short spreadsheet](https://docs.google.com/spreadsheets/d/1G9Kb-ImlluK8MOx-ce2rlHUBnTOtAFQvKjjs1bEhlpM/edit#gid=963553579)
to return the transfer function.

> **TODO:** make spreadsheet contents visible here

### Pressure Sensor

This venturi is designed to be paired with this sensor and a 14-bit ADC. A 12-bit ADC would work, but accuracy at the
low end is improved.

[Data Sheet MPXV5004DP](https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf)

Adjust values in the spreadsheet above accordingly to see how it will perform in your setup.

## Performance Data

### Pressure Loss curve

> **TODO:** add flow loss curve

### Flow Rate

Without calibration, using the NXP sensor above, the 16mm sensor was compared to a Fleisch Pneumotachograph.
The response curve is shown below over a varying flow signal. Note there is no calibration factor applied, so there is some offset.

![flow curve](images/pneumotachograph_compare.png)

> **TODO:** add flow accuracy curve and get improved correlation.

### Characterization tests

Characterization tests are included [here](characterization-tests).

## Fabrication at Scale

The basis for the design came from previous experience using adapted fertilizer injection venturis as flow measurement
devices. The original part is shown below.

There is no fundamental reason why the exact same design cannot be used for this application, especially if these are
being produced at scale. However, the dimensions of this part are not quite right to optimize for the flow regime of
patient inspiration. The commonly available 1/2-NPT fertilizer venturi has an entrance diameter of 12.7mm and a throat
diameter of 4.1 mm. After experimentation with the prototype venturi, the final dimensions were selected as an entrance
diameter of 15.05 mm and a throat diameter of 5.5 mm. These are significantly different from a flow measurement
standpoint, but not particularly different from a manufacturing standpoint.

The fabrication for the above process is to produce two mirror half-molds with partial thread blanks for the three
threads and alignment pins in the molds. Glue is applied to the mold halves, with the pins used to precisely align the
components. After the glue sets, additional flashing and the alignment pins are machined off, and the threads are
cleaned up with a cutting die. By using common thread fittings (1/2 NPT on other end, and 1/4 NPT on the pressure tap)
readily available adapters can be attached. For the upstream pressure tap, a 1/2 NPT tee is used, with the flow entering
the tee from upstream, and a barb adapter used to connect to the 3/32 tubing. For the throat tap, a 1/4 NPT female to
3/32 barb adapter is required.
