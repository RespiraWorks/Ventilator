# Wiring

The following page describes the custom wiring components required in the electrical assembly. Manufacturing
these components will require soldering and crimping.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                | Price (USD)  | Sources[*][ppg]       | Notes |
| ----- |---------:| ------------- | --------------------- | ------------:|:---------------------:| ----- |
|**A1** | 20       | JST           | SXH-001T-P0.6         |         0.10 | [K][a1digi]           | XH crimps, for sensor & actuator connections |
|**A2** | 1        | JST           | XHP-2                 |         0.10 | [K][a2digi]           | XH 2-pin housings, for solenoid |
|**A3** | 1        | JST           | XHP-3                 |         0.10 | [K][a3digi]           | XH 3-pin housings, for oxygen sensor |
|**A4** | 1        | JST           | XHP-6                 |         0.12 | [K][a4digi]           | XH 6-pin housings, for blower control |
|**A5** | 7        | JST           | SVH-21T-P1.1          |         0.11 | [K][a5digi] [R][a5rs] | VH crimps, for main power, for blower power |
|**A6** | 2        | TE            | 1-1123722-2           |         0.18 | [K][a6digi] [R][a6rs] | VH housing 2 pin, for blower power |
|**A7** | 1        | JST           | VHR-3N                |         0.14 | [K][a7digi]           | VH housing 3 pin, for main power |
|**A9** | 1        | Kycon         | KPJX-PM-4S            |         2.89 | [Z][a9key]            | 4 pin DIN female, main power connector |
|**A10**| 8        |TE Connectivity| 102107-3              |         0.47 | [Z][a10key]           | 2.54mm male pin, for stepper interface |
|**A11**| 2        |TE Connectivity| 87499-7               |         0.73 | [Z][a11key]           | 2.54mm 4-pin housing, for stepper interface |
|**A12**|        - | Remington     | 16UL1007STRKIT        |  29.70 / 45m | [Z][a12amzn]          | 16 AWG wire, for power cables |
|**A13**|        - | Remington     | 22UL1015STRKIT10COLOR |  37.75 / 75m | [Z][a13amzn]          | 22 AWG wire, for signal/control cables |
|**A14**|        - | Remington     | 26UL1007STRKIT10COLOR |  31.60 / 75m | [Z][a14amzn]          | 26 AWG wire, for signal/control cables |
|**A15**| (1)      | WGCD          | 4330589508            |         8.99 | [Z][a15amzn]          | XH connector kit for 2/3/4/5 pins, **alternative for A1-3** |
|**A16**| (1)      | CQRobot       | CQRJST254-BGB         |         9.99 | [Z][a16amzn]          | XH connector kit for 6/7/8 pins, **alternative for A1 and A4** |
|**A17**| (1)      | Chenbo        | WJ-Z01                |        11.98 | [Z][a17amzn]          | 2.54mm connector, **alternative for A10 and A11** |

[a1digi]:   https://www.digikey.com/short/z44f8d
[a2digi]:   https://www.digikey.com/short/z44f8f
[a3digi]:   https://www.digikey.com/short/z44fb9
[a4digi]:   https://www.digikey.com/short/z44ff8
[a5digi]:  https://www.digikey.com/short/z44fjr
[a5rs]:    https://export.rsdelivers.com/product/jst/svh-41t-p11/jst-nv-vh-female-crimp-terminal-contact-16awg-svh/7620692
[a6digi]:  https://www.digikey.com/short/z44fwj
[a6rs]:    https://export.rsdelivers.com/product/jst/vhr-2n/jst-vhr-female-connector-housing-396mm-pitch-2/8201172
[a7digi]: https://www.digikey.com/short/z44fwp
[a9key]: https://www.digikey.com/en/products/detail/kycon-inc/KPJX-PM-4S/9990081
[a10key]: https://www.digikey.com/en/products/detail/te-connectivity-amp-connectors/102107-3/298993
[a11key]: https://www.digikey.com/en/products/detail/te-connectivity-amp-connectors/87499-7/29911
[a12amzn]: https://www.amazon.com/gp/product/B00N51OJJ4
[a13amzn]: https://www.amazon.com/gp/product/B073SDGNKW
[a14amzn]: https://www.amazon.com/gp/product/B011JC76OA
[a15amzn]:  https://www.amazon.com/gp/product/B06ZZ45G7G
[a16amzn]:  https://www.amazon.com/gp/product/B079MJ1RYN
[a17amzn]: https://www.amazon.com/CHENBO-Connector-Housing-Assortment-Terminal/dp/B077X8XV2J

**#TODO:** Add dupont connectors, shrink tubing

## Tools

| Item | Manufacturer | Part number         | Price($) | Sources[*][ppg] | Notes |
| ---- | ------------ | ------------------- | --------:|-----------------| ----- |
| B1   | Preciva      | PR-3254 (PR102001)  |    39.99 | [Z][b1amzn]     | Crimping tool for HX and HV connectors + connector kit |
| B2   | NovelLife    | TS100               |    74.99 | [Z][b2amzn]     | 65W Soldering iron kit |
| B3   | Chandler     | HG603D              |    24.92 | [Z][b3amzn]     | Heat gun, for shrink tubing |
| B4   | Kester       | KE1107-ND           |    58.98 | [Z][b4key]      | Solder |

[b1amzn]: https://www.amazon.com/gp/product/B07R1H3Z8X
[b2amzn]: https://www.amazon.com/NovelLife-Mini-TS100-Soldering-Digital/dp/B07D35B75T
[b3amzn]: https://www.amazon.com/Chandler-Tool-Embossing-Heat-Gun/dp/B07GC5N3QC
[b4key]: https://www.digikey.com/en/products/detail/kester-solder/24-6040-0010/265622

**Crimper Tips:**

* Position 1 on the crimper will crimp XH contacts
* Position 2 on the crimper will crimp VH contacts.  However, with certain gauges of wire, this crimper may bend the contact into a shrimp-like shape.  If this happens, bend the contact back straight (it should be pretty obvious as it blocks insertion)
* It comes with a lot of XH contacts, but not any VH contacts.

## Assembly

As much as possible we should try to keep the cables one-to-one
(for example the wiring of the PCB to blower driver cables are all pin 1 to pin 1,
pin 2 to pin 2, etc. with the same connector style on both ends.

![Pizza pneumatics](images/wires.jpg)

### Main power

**#TODO:**

### Blower wiring

For pizza builds: You will also have to crimp connectors (**A8 + A9**) to the second end of the wiring (using **B1**) that connects the
blower driver board to the [PCB](../../../pcb).

The power and signal cables that come included with the blower are too short to reach the PCB, so new ones must be
manufactured using **A1, A5, A7-A9, A12-A13**. Please follow the old cables included with blower as examples.

The new cables should each be about 42cm in length.

**#TODO:** Some sort of shrink tubing should also be used

**#TODO:** illustrate this

### Stepper wiring

**#TODO:** make pigtails

### Proportional solenoid valve

To connect the solenoid to the PCB, you will need to crimp a 2-pin XH connector to the wires.

**#TODO:** expand on this and illustrate this

### Oxygen sensor

**#TODO:** missing info

## Pricing totals

**#TODO:** missing info
