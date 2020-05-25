# Pizza Build

**WARNING: This is not intended to be used on patients and lacks a number of safety features.**

This is a ventilator prototype for testing and development.
The build consists of a functional ventilator pneumatic assembly with controller and user interface. This build is adequate for integration and systems testing.

It lacks heating, humidification, battery backup and air filtering.

It is also not enclosed like the final product and is meant to be operated in an open layout on your table, like a pizza.

It can also come in various configurations, depending on your testing needs:

| Solenoid valve setup  |  Pinch valve setup   | Advanced setup        |           
|:---------------------:|:--------------------:|:---------------------:|
![](photo_solenoid.jpg) |![](photo_pinch.jpg)  |![](photo_advanced.jpg)|

**DISCLAIMER: These are live documents.
There are duplicate and alternative items for undecided aspects of the physical design. 
RespiraWorks may or may not compensate you for any of these parts if you choose to buy them. Please connect with Edwin, Martin or Ethan first.
If you decide to buy them, do so at your own risk... oh, and thank you ;).**

This build depends on already having a Brain build, which has its own BOM [here](brain_build.md).

## Purchasing source abbreviations

* **A** = Ali Express
* **Z** = Amazon
* **E** = ebay
* **C** = McMaster-Carr
* **M** = Mouser
* **W** = Wonsmart
* **B** = Built by RespiraWorks

## Parts BOM

| RefDes | Quantity | Manufacturer  | Part #              | Price($)     | Do what?  | Sources         | Notes |
| ------ |---------:| ------------- | ------------------- | ------------:|:---------:|-----------------| ----- |
| 1      |        - | McMaster-Carr | 53415K217           | 12.89 / 10   | buy       | [C][1mcmc]      | Barbed fitting 3/4" |
| 2      |        - | McMaster-Carr | 5463K565            | 10.53 / 10   | buy       | [C][2mcmc]      | Barbed tee-fitting 3/4" |
| 5      |        2 |               |                     | 12.95 / 2    | buy       | [Z][5amzn]      | 2pac of ball valves 1/2", diameter of current design but may not be for long |
| 5a     |        2 |               |                     | 13.45 / 2    | buy       | [Z][5a-amzn]    | 2pac of ball valves 5/8", alternative for uncertain design changes |
| 5b     |        2 |               |                     | 13.45 / 2    | buy       | [Z][5b-amzn]    | 2pac of ball valves 3/4", alternative for uncertain design changes |
| 6      |        - |               |                     | 14.26 / 10ft | buy       | [Z][6amzn]      | 5/8" tubing, fits current venturi design |
| 6a     |        - |               |                     | 42.99 / 10ft | buy       | [Z][6a-amzn] [C][6a-mcmc]   | 3/4" tubing, alternative because we are probably switching to this diameter |
| 7      |        - |               |                     | 6.49 / 16ft  | buy       | [Z][7amzn] [C][7mcmc]     | 2.5 mm tubing (for sensors?) |
| 8      |        1 |               |                     | 19.99        | buy       | [Z][8amzn] [C][8mcmc]     | 12v Power supply adapter (obsolete? opt for 8a?) |
| 8a     |        1 |               |                     | 13.99        | buy       | [Z][8a-amzn] [C][8a-mcmc]   | 12v Power supply adapter, better than 8 above |
| 11     |        1 |               |                     | 6.98         | buy       | [Z][11amzn]     | use these to simulate lung compliance |
| 12     |        1 | RespiraWorks  | PCB1                |              | **ask**   | [B][12rw]       | RespiraWorks Ventilator Mainboard Rev 1 PCB |
| 13     |        1 | RespiraWorks  | Power input 1       |              | **ask**   | [B][13rw]               | Power Input cable |
| 16     |        2 | *you*         |                     |              | **make**  | [B][16rw]       | Venturi sensor assembly |
| 20     |        1 | McMaster-Carr | 5463K44             | 7.39 / 10    | buy       | [C][20mcmc] | sensing tube 3/23" t-junction |
| 21     |        1 | *you*         |                     |              | **make**  | [B][21rw] | Test lung assembly |
| 22     |        1 | *you*         |                     |              | **make**  | [B][22rw] | Blower assembly |

[1mcmc]:   https://www.mcmaster.com/53415K217
[2mcmc]:   https://www.mcmaster.com/5463K565
[5amzn]:   https://www.amazon.com/gp/product/B07C5B4S1S
[5a-amzn]: https://www.amazon.com/gp/product/B07C5FVN9K
[5b-amzn]: https://www.amazon.com/gp/product/B07C5FVSQB
[6amzn]:   https://www.amazon.com/gp/product/B00LX6MB6G
[6a-amzn]: https://www.amazon.com/gp/product/B079WRWH34
[6a-mcmc]: https://www.mcmaster.com/5233K71
[7amzn]:   https://www.amazon.com/gp/product/B01F4BJ7PI
[7mcmc]:   https://www.mcmaster.com/50315K68
[8mcmc]:   https://www.mcmaster.com/3791N024
[8amzn]:   https://www.amazon.com/gp/product/B00Z9X4GLW
[8a-amzn]: https://www.amazon.com/gp/product/B01MYXTA6N
[8a-mcmc]:https://www.mcmaster.com/3824N132
[11amzn]:  https://www.amazon.com/gp/product/B018WPZCSO
[12rw]:    https://github.com/RespiraWorks/pcbreathe/tree/master/NIGHTLY/20200424v2-RELEASE-CANDIDATE-2
[13rw]:    ../../1_Ventilator_System_Design/Electrical_System/wiring.md
[16rw]:    ../../2_Research_&_Development/Project-Venturi
[20mcmc]:    https://www.mcmaster.com/5463K44
[21rw]:    pizza_test_lung
[22rw]:    pizza_blower

## Pizza Toppings

**Optional additions (toppings) for your pizza build:**
These can be added or customized based on what functions you need to test in your pizza build.
* **Reduced compliance test lung:** To test basic function a ventilator check lung or a breathing bag is sufficient,
but can really only work up to 10cmH2O. To be able to test higher PIP values, you need to reduce the compliance of the
lung so you don’t just inflate it like a balloon (or pop it). View more details on reduced compliance test lung [here](pizza_compliance_test_lung).
* **Binary solenoid:** One option for inhale or exhale valve is a binary solenoid, which either opens or closes.
The sub-assembly for this component is documented [here](pizza_binary_solenoid). The PCB can only power/control one such
solenoid, so for more advanced designs you will want to (also) start using other valves.
* **Proportional pinch valve** A proportional pinch valve can control the airflow constriction with much better precision.
Multiple such pinch valves can be used in our prototype, the part is buildable from 3d-printed plastic components and
easily obtainable generic parts. Instructions are [here](../../2_Research_&_Development/Project-Pinch_Valve).
You may need anywhere between 1 and 3 such pinch valves, depending on the complexity of your system.


## Assembly instructions

![Pizza pneumatics](pizza_pneumatics.png)
[Pneumatics diagram source](pizza_pneumatics.eps)

Note: mind which tubes go into the bottom and top ports of the sensors on the PCB.

![Pizza drawing](pizza_diagram.jpg)
