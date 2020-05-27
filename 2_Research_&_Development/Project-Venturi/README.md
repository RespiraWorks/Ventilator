# Venturi Flow Sensor

![Rendering](screenshot.png)

The venturi is used to measure flow by measuring a change in pressure.
This version inserts within 5/8" ID tubing, though can also be used with 16mm ID tubing. 
You can use a zip tie or hose clamp to improve the seal. 

The plan is for [beta](/../../4_Manufacturing_Design/Beta_Build_Instructions/README.md) to use 3/4"ID tubing throughout.
The venturi will most likely be redesigned to have barbs that fit 3/4"ID tubing directly.

The pressure sensor we are using has 2.5 mm or 3/32" barbs, which is too small for 1/8" tubing, therefore this model has #10-32 tapped holes.

There is an arrow printed onto the body to indicate the direction of flow.

**#TODO:** link and explain STEP files

## Transfer Function

To go from voltage to flow rate, you need the following transfer functions.
For a 0.157" throat and 0.5" body using a 0-3920 Pa analog sensor, where offset is the 0 flow signal:

Q [liters/minute] = 0.75398*sqrt(1346.584669*(voltage-offset))

**#TODO:** make math pretty with Mathjax?

For an arbitrary body geometry, you can use the following short spreadsheet to return the transfer function:
https://docs.google.com/spreadsheets/d/1G9Kb-ImlluK8MOx-ce2rlHUBnTOtAFQvKjjs1bEhlpM/edit#gid=963553579

**#TODO:** make spreadsheet contents visible here

## Pressure Sensor

This venturi is designed to be paired with this sensor and a 14-bit ADC. 

[Data Sheet MPXV5004DP](https://www.nxp.com/docs/en/data-sheet/MPXV5004G.pdf)

Adjust values in the spreadsheet above accordingly to see how it will perform in your setup. 

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

![Nipples](venturi-nipples.jpg)

Screw the nipples into the 3d-printed venturi.
There are threads in the print but they don’t always come out right if resin gets trapped in the grooves while it prints.
In such a case you might need a tap (listed in tooling BOM above) to clean the threads. You do not need a bottoming tap, since there is enough depth in the holes and the threads are short.

In the latest tested version, no hand-tapping was required to insert nipples. With good enough 3d printing, this step should not be necessary. 

**#TODO:** instructions on connecting tubes to sensors

## Testing

Check for leaks by plugging the holes and pulling some vacuum. 
* Plug one large end, plug both nipples, suck on the remaining end.  If you have a vacuum pump handy do a regular vacuum hold leak test.
* Or, blow into each port with your fingers over the other three.

