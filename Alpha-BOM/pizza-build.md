# Pizza Build

**DISCLAIMER: These are live documents.
There are duplicate and alternative items for undecided aspects of the physical design. 
RespiraWorks may or may not compensate you for any of these parts if you choose to buy them. Please connect with Edwin or Ethan first.
If you decide to buy them, do so at your own risk... oh, and thank you ;).**

This build consists of a functional ventilator pneumatic assembly with controller and user interface. This build is adequate for integration and systems testing.

It lacks heating, oxygen mixing, humidification, battery backup and air filtering.
It is also not enclosed like the final product and is meant to be operated in an open layout on your table, like a pizza. **This is not intended to be used on patients and lacks a number of safety features.**

This build depends on already having a Brain build, which has its own BOM [here](brain-build.md).

![Draft Pizzabuild Drawing](20200509-draft-pizzabuild-drawing.jpg)

## Purchasing source abbreviations

* **A** = Ali Express
* **Z** = Amazon
* **E** = ebay
* **C** = McMaster-Carr
* **K** = Digikey
* **W** = Wonsmart
* **B** = Built by RespiraWorks

## Parts BOM

| RefDes | Quantity | Manufacturer  | Part #              | Price($)     | Do what?  | Sources         | Notes |
| ------ |---------:| ------------- | ------------------- | ------------:|:---------:|-----------------| ----- |
| 1      |        - | McMaster-Carr | 53415K217           | 12.89 / 10   | buy       | [C][1mcmc]      | Barbed fitting 3/4" |
| 2      |        - | McMaster-Carr | 5463K565            | 10.53 / 10   | buy       | [C][2mcmc]      | Barbed tee-fitting 3/4" |
| 3      |        - | McMaster-Carr | 5463K33             | 3.93 / 10    | buy       | [C][3mcmc]      | Barbed fitting 3/32" for venturis |
| 4      |        - | McMaster-Carr | 5463K37             | 4.74 / 10    | buy       | [C][4mcmc]      | Barbed fitting 3/32" for venturis |
| 5      |        2 |               |                     | 12.95 / 2    | buy       | [Z][5amzn]      | 2pac of ball valves 1/2", diameter of current design but may not be for long |
| 5a     |        2 |               |                     | 13.45 / 2    | buy       | [Z][5a-amzn]    | 2pac of ball valves 5/8", alternative for uncertain design changes |
| 5b     |        2 |               |                     | 13.45 / 2    | buy       | [Z][5b-amzn]    | 2pac of ball valves 3/4", alternative for uncertain design changes |
| 6      |        - |               |                     | 14.26 / 10ft | buy       | [Z][6amzn]      | 5/8" tubing, fits current venturi design |
| 6a     |        - |               |                     | 42.99 / 10ft | buy       | [Z][6a-amzn] [C][6a-mcmc]   | 3/4" tubing, alternative because we are probably switching to this diameter |
| 7      |        - |               |                     | 6.49 / 16ft  | buy       | [Z][7amzn] [C][7mcmc]     | 2.5 mm tubing (for sensors?) |
| 8      |        1 |               |                     | 19.99        | buy       | [Z][8amzn] [C][8mcmc]     | 12v Power supply adapter (obsolete? opt for 8a?) |
| 8a     |        1 |               |                     | 13.99        | buy       | [Z][8a-amzn] [C][8a-mcmc]   | 12v Power supply adapter, better than 8 above |
| 9      |        2 |               |                     | 16.99 / 2    | buy       | [Z][9amzn]      | CPAP tubing |
| 10     |        1 |               |                     | 15.00        |*ask first*| [E][10ebay]     | test "lung", ask Edward if he has extra? |
| 11     |        1 |               |                     | 6.98         | buy       | [Z][11amzn]     | use these to simulate lung compliance |
| 12     |        1 | RespiraWorks  | PCB1                |              | **ask**   | [B][1rw]        | RespiraWorks Ventilator Mainboard Rev 1 PCB |
| 13     |        1 | RespiraWorks  | Power input 1       |              | **ask**   | B               | Power Input cable |
| 14     |        1 | Digikey       | X-NUCLEO-IHM03A1    |        10.64 |*ask first*| [K][14key]      | Stepper driver dev board |
| 15     |        1 | RespiraWorks  | PinchValve1.4       |              | **ask**   | B               | Marc-designed stepper pinch valve rev 1.4 |
| 16     |        2 | RespiraWorks  | Venturi2            |              | **ask**   | B               | Ethan-designed 10-32 threaded venturis Rev 2 |
| 17     |        1 |               | WS7040-12-X200N     |        52.65 |*ask first*| [A][17ali]      | 12v blower and driver. Make sure to choose w/driver when ordering. |
| 17x    |        1 |               | WS1208DYY01V03      |        24.00 |*ask first*| [W][17x-won]    | blower driver, in case blower above did not come with driver. |
| 18     |        1 | RespiraWorks  | blower control cable|              | **ask**   | B               | blower control cable |
| 19     |        1 | RespiraWorks  | blower power cable  |              | **ask**   | B               | blower power cable |

| Item # | Quantity | Part #              | Price($) | Sources         | Notes |
| ------ |---------:| ------------------- | --------:|-----------------| ----- |
| 1      |        1 | 53415K217           |          | [C][1mcmc]      | Barbed fitting 3/4" |
| 2      |        1 | 5463K565            |          | [C][2mcmc]      | Barbed tee-fitting 3/4" |
| 3      |        1 | 5463K33             |          | [C][3mcmc]      | Barbed fitting 3/32" for venturis |
| 4      |        1 | 5463K37             |          | [C][4mcmc]      | Barbed fitting 3/32" for venturis |
| 5      |        2 |                     |          | [Z][5amzn]      | 2pac of ball valves 1/2", diametere of current design but may not be for long |
| 5a     |        2 |                     |          | [Z][5a-amzn]    | 2pac of ball valves 5/8", alternative for uncertain design changes |
| 5b     |        2 |                     |          | [Z][5b-amzn]    | 2pac of ball valves 3/4", alternative for uncertain design changes |
| 6      |    ???ft |                     |          | [Z][6amzn]      | 5/8" tubing, fits current venturi design |
| 9      | ft or units? |                 |          | [Z][9amzn]      | CPAP tubing |
| 10     |        1 |                     |          | [Z][10ebay]     | test "lung" |
| 11     |        1 |                     |          | [Z][11amzn]     | use these to simulate lung compliance |


[1mcmc]:   https://www.mcmaster.com/53415K217
[2mcmc]:   https://www.mcmaster.com/5463K565
[3mcmc]:   https://www.mcmaster.com/5463K33
[4mcmc]:   https://www.mcmaster.com/5463K37
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
[9amzn]:   https://www.amazon.com/gp/product/B01N14F1MV
[10ebay]:  https://www.ebay.com/itm/PORTEX-Adult-Disposable-Anesthesia-Breathing-Circuit-3L-Breathing-Bag-370924/362943456196
[11amzn]:  https://www.amazon.com/gp/product/B018WPZCSO
[14key]:   https://www.digikey.com/product-detail/en/stmicro/X-NUCLEO-IHM03A1/497-15868-ND/5325495
[17ali]:   https://www.aliexpress.com/item/32698107687.html
[17x-won]: https://wonsmart-motor.en.made-in-china.com/product/hsjxFewOppVg/China-Air-Pump12V-Brushless-Motor-12V-Blower-Fan-Driver.html
[1rw]:     https://github.com/RespiraWorks/pcbreathe/tree/master/NIGHTLY/20200424v2-RELEASE-CANDIDATE-2

## Tooling BOM

We try to make few assumptions about what tools you have. Here are recommended tools you might need.

| RefDes | Quantity | Manufacturer  | Part number         | Price($) | Sources         | Notes |
| ------ |---------:| ------------- | ------------------- | --------:|-----------------| ----- |
| T1     |        1 | McMaster-Carr | 26955A86            |     6.76 | [C][t1mcmc]     | Tap for 10-32 threads for venturis. Edwin notes that 3d printed fittings may not have good enough threads and you might need to chase them with a hand tap. |
| T2     |        1 | Kaufhof       | KF-ATW-0053         |    14.11 | [Z][t2amzn]     | Hand-operated tap wrenches, for use with T1 above |

[t1mcmc]:https://www.mcmaster.com/26955A86
[t2amzn]:https://www.amazon.com/gp/product/B003GKJYKI
