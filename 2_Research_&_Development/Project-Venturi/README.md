# Venturi Flow Sensor

![Rendering](assets/screenshot.png)

The venturi is used to measure flow by measuring a change in pressure as the flow accelerates through the throat. Relative to other pressure sensor types, a venturi flow meter produces the largest signal pressure at the smallest pressure loss. This allows significantly cheaper and more widely available pressure sensors to be used, because a signal at peak flow can be 5 to 15 times larger than the flow resistance. 

The change in pressure is measured between the two ports, and this is correlated to the flow rate through the orifice. It is important to note that the as the flow expands, almost all of this pressure is recovered to the original flow; the pressure signal measured is not the same as the pressure resistance to flow. 

# Mechanical Implementation

The venturi is available in 3/4" or 5/8" ID tubing.

The Alpha version is designed to insert into 5/8" ID or 16mm ID tubing.

The [beta](/../../4_Manufacturing_Design/Beta_Build_Instructions/README.md) version of the ventilator uses 3/4"ID or 19mm ID tubing throughout. 

The pressure sensor we are using, see below, has 2.5 mm or 3/32" barbs. Note that this is not sufficiently close in size to use more common 3mm or 1/8" tubing. This model has printed threads to accept an adapter from #10-32 pipe to 3/32" adapter. Other adapters could be incorporated eventually by changing the interface design.

You can use a zip tie or hose clamp to improve the seal by tightening it behind the barb.

There is an arrow printed onto the body between the two ports to indicate the direction of flow. The venturi is asymetrical, with a tighter constriction on entrance than exit. 

## Transfer Function

To go from voltage to flow rate, you must correlate the flow rate to a change in pressure, and the change in pressure to a voltage. 

The fundamental equation for a ventiru flow meter is:

![Equation](https://wikimedia.org/api/rest_v1/media/math/render/svg/164d293785917d5d5bb07cdb01d96bafbc603a61)

You then need to convert from dP to voltage, if using an analog sensor, or from dP to serial, if using an I2C sensor. 

You can use [this short spreadsheet](https://docs.google.com/spreadsheets/d/1G9Kb-ImlluK8MOx-ce2rlHUBnTOtAFQvKjjs1bEhlpM/edit#gid=963553579) to return the transfer function.

**#TODO:** make spreadsheet contents visible here

## Pressure Sensor

This venturi is designed to be paired with this sensor and a 14-bit ADC. 

[Data Sheet MPXV5004DP](https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf)

Adjust values in the spreadsheet above accordingly to see how it will perform in your setup. 

## 3D Printing

### Files

* Venturi - for use with 5/8" ID tubing
    * [.step format](assets/venturi-5-8.step)
    * [.iges format](assets/venturi-5-8.iges)
    * [.f3d format](assets/venturi-5-8.f3d)
* Venturi - for use with 3/4" ID tubing
    * [.iges format](assets/venturi-3-4.iges)
    * [.f3d format](assets/venturi-3-4.f3d)

### Guidelines

These should not be printed with FDM. Instead, a leaktight method like SLA or MJF should be used. Also, this model contains printed threads and is designed to interface with the referenced mcmaster barbed fitting (item 2 below). 

Note that the flow measurement is very sensitive to the exact geometry, particularly the thread geometry. 3D printing can have variable shrinkage that results in uncertainty in the throat size, and thus the flow rates. Check the diameter of the printed venturi; a good way to do this is to put a drill into the venturi that just barely fits, and then measure the diameter of that drill bit. (Any round thing will work.)

## Parts List (BOM)

### Purchasing Source Key 

* **C** = McMaster-Carr
* **Z** = Amazon
* **3D** = 3D printed

### Parts

| Item | Quantity | Manufacturer  | Part #         | Price (USD)     | Action | Sources    | Notes |
| ---- |---------:| ------------- | ------------------- | ------------:| ------- |-------------| ----- |
| 1    |        1 | RespiraWorks  |                     |              | **ask** | **3D**      | 3D-printed venturi |
| 2    |        2 | McMaster-Carr | 5463K33             | 3.93 / 10    | buy     | [C][2mcmc]  | Barbed fitting 3/32" AKA nipple |
| 3    |        1 | McMaster-Carr | 5463K44             | 7.39 / 10    | buy     | [C][3mcmc]  | sensing tube 3/23" t-junction |
| 4    |        - | uxcell        | a16031400ux0163     | 6.49 / 16ft  | buy     | [Z][4amzn]  | 2.5 mm tubing for connecting to sensors |
| 5    |        - | McMaster-Carr | 50315K68            | 5.75 / 25ft  | buy     | [C][5mcmc]  | 2.5 mm tubing, alternative to *3* |

[2mcmc]:   https://www.mcmaster.com/5463K33
[3mcmc]:    https://www.mcmaster.com/5463K44
[4amzn]:   https://www.amazon.com/gp/product/B01F4BJ7PI
[5mcmc]:   https://www.mcmaster.com/50315K68

### Tools

We try to make few assumptions about what tools you have. Here are recommended tools you might need. You might already have some of these.

| Item | Quantity | Manufacturer  | Part #         | Price (USD) | Sources         | Notes |
| ------ |---------:| ------------- | ------------------- | --------:|-----------------| ----- |
| a1     |        1 | McMaster-Carr | 26955A86            |     6.76 | [C][a1mcmc]     | Tap for 10-32 threading nipple ports. |
| a2     |        1 | Kaufhof       | KF-ATW-0053         |    14.11 | [Z][a2amzn]     | Hand-operated tap wrenches, for use with a1 above | 

[a1mcmc]:https://www.mcmaster.com/26955A86
[a2amzn]:https://www.amazon.com/gp/product/B003GKJYKI

## Assembly

![Nipples](assets/venturi-nipples.jpg)

Screw the nipples into the 3d-printed venturi.
There are threads in the print but they don’t always come out right if resin gets trapped in the grooves while it prints.
In such a case you might need a tap (listed in tooling BOM above) to clean the threads. You do not need a bottoming tap, since there is enough depth in the holes and the threads are short.

In the latest tested version, no hand-tapping was required to insert nipples. With good enough 3d printing, this step should not be necessary. 

**#TODO:** instructions on connecting tubes to sensors

## Testing

Check for leaks by plugging the holes and pulling some vacuum. 

* Plug one large end, plug both nipples, suck on the remaining end.  If you have a vacuum pump handy do a regular vacuum hold leak test.
* Or, blow into each port with your fingers over the other three.

