# Pizza build: Binary Solenoid Valve

A solenoid valve can be used as a valve in the pneumatic circuit.
A solenoid is a common car component, relatively cheap and easy to obtain.

We refer to this as a binary solenoid because it has only two states - open or closed, as opposed to a proportional solenoid, which would have other degrees in between.  

**#TODO:** unit price analysis

## Parts List (BOM)

### Purchasing Source Key

* **Z** = Amazon
* **C** = McMaster-Carr
* **K** = Digikey

### Parts

| Item | Quantity | Manufacturer  | Part number         | Price (USD)     | Sources        | Notes |
| ---- |---------:| ------------- | ------------------- | ------------:|----------------| ----- |
| 1    |        1 | Beduan        | 40141600            | 17.99        | [Z][1amzn]     | 1/2NTP 12v solenoid |
| 2    |        2 | McMaster-Carr | 5372K128            | 4.89 / 10    | [C][2mcmc]     | 1/2 NTP <-> 5/8"ID male multibarbed |
| 3    |        2 | McMaster-Carr | 53415K216           | 10.00 / 10   | [C][3mcmc]     | 1/2 NTP <-> 3/4"ID male single barb, **alternative to item 2 for 3/4"ID prototypes** |
| 4    |        2 | JST           | SXH-001T-P0.6       |         0.10 | [K][4digi]     | XH crimps, for connection to PCB |
| 5    |        1 | JST           | XHP-2               |         0.10 | [K][5digi]     | XH 2-pin housing, for connection to PCB |

[1amzn]:  https://www.amazon.com/gp/product/B07N6246YB
[2mcmc]:  https://www.mcmaster.com/5372K128
[3mcmc]:  https://www.mcmaster.com/53415K216
[4digi]:  https://www.digikey.com/short/z44f8d
[5digi]:  https://www.digikey.com/short/z44f8f

## Tools

| Item | Quantity | Manufacturer  | Part number         | Price($) | Sources         | Notes |
| ---- |---------:| ------------- | ------------------- | --------:|-----------------| ----- |
| a1   |        1 | Preciva       | PR-3254 (PR102001)  |    39.99 | [Z][a1amzn]     | Crimping tool for HX and HV connectors |

[a1amzn]:https://www.amazon.com/gp/product/B07R1H3Z8X

**NOTE: Items 3, 4 and a1 above are also duplicated in the [wiring BOM](../../../1_Ventilator_System_Design/Electrical_System/wiring.md)**

## Assembly Instructions

Screw tubing adapters into the solenoid - either item 2 or 3, depending on the diameter of the tubing in your prototype.

To connect it electrically to the PCB you will need to crimp a 2-pin XH connector on the end.
Depending on your table-top setup, you may want to extend the included wires.

![Lung-with-new-tubes](binary_solenoid.jpg)



