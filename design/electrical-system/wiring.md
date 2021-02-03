# Wiring

**DISCLAIMER: These are live documents.
There are duplicate and alternative items for undecided aspects of the physical design.
RespiraWorks may or may not compensate you for any of these parts if you choose to buy them. Please connect with Edwin or Ethan first.
If you decide to buy them, do so at your own risk... oh, and thank you ;).**

**TODO:** explain purpose and contents

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../manufacturing/purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #              | Price (USD)  | Sources[*][ppg]       | Notes |
| ----- |---------:| ------------- | ------------------- | ------------:|:---------------------:| ----- |
|**A1** |       12 | JST           | SXH-001T-P0.6       |         0.10 | [K][a1digi]           | XH crimps, for sensor & actuator connections |
|**A2** |        - | JST           | XHP-2               |         0.10 | [K][a2digi]           | XH 2-pin housings, for solenoid |
|**A3** |        - | JST           | XHP-3               |         0.10 | [K][a3digi]           | XH 3-pin housings, for heater |
|**A4** |        - | JST           | XHP-5               |         0.12 | [K][a4digi]           | XH 5-pin housings, for sensors |
|**A5** |        2 | JST           | XHP-6               |         0.12 | [K][a5digi]           | XH 6-pin housings, for blower control |
|**A6** |        - | WGCD          | 4330589508          |         8.99 | [Z][a6amzn]           | XH connector kit for 2/3/4/5 pins, alternative for 1+2+3+4 above |
|**A7** |        - | CQRobot       | CQRJST254-BGB       |         9.99 | [Z][a7amzn]           | XH connector kit for 6/7/8 pins, alternative for 1+5 above |
|**A8** |        6 | JST           | SVH-21T-P1.1        |         0.11 | [K][a8digi] [R][a8rs] | VH crimps, for main power, for blower power |
|**A9** |        2 | TE            | 1-1123722-2         |         0.18 | [K][a9digi] [R][a9rs] | VH housing 2 pin, for blower power |
|**A10**|        1 | JST           | VHR-3N              |         0.14 | [K][a10digi]          | VH housing 3 pin, for main power |
|**A11**|        1 | Kycon         | KPJX-PM-4S          |         2.89 | [Z][a11key]          | Female Power Pigtail DC 5.5mm x 2.1mm Connectors, for main power |
|**A12**|        - | Remington     | 16UL1007STRKIT      |        29.70 | [Z][a12amzn]          | 16 AWG wire, for blower power cable |
|**A13**|        - | Remington     | 26UL1007STRKIT10COLOR |      31.60 | [Z][a13amzn]          | 26 AWG wire, for signal/control cables |
|**A14**|        - | Remington     | 22UL1015STRKIT10COLOR |      37.75 | [Z][a14amzn]          | 22 AWG wire, for signal/control cables |

[a1digi]:   https://www.digikey.com/short/z44f8d
[a2digi]:   https://www.digikey.com/short/z44f8f
[a3digi]:   https://www.digikey.com/short/z44fb9
[a4digi]:   https://www.digikey.com/short/z44r0b
[a5digi]:   https://www.digikey.com/short/z44ff8
[a6amzn]:  https://www.amazon.com/gp/product/B06ZZ45G7G
[a7amzn]:  https://www.amazon.com/gp/product/B079MJ1RYN
[a8digi]:  https://www.digikey.com/short/z44fjr
[a8rs]:    https://export.rsdelivers.com/product/jst/svh-41t-p11/jst-nv-vh-female-crimp-terminal-contact-16awg-svh/7620692
[a9digi]:  https://www.digikey.com/short/z44fwj
[a9rs]:    https://export.rsdelivers.com/product/jst/vhr-2n/jst-vhr-female-connector-housing-396mm-pitch-2/8201172
[a10digi]: https://www.digikey.com/short/z44fwp
[a11key]: https://www.digikey.com/en/products/detail/kycon-inc/KPJX-PM-4S/9990081
[a12amzn]: https://www.amazon.com/gp/product/B00N51OJJ4
[a13amzn]: https://www.amazon.com/gp/product/B011JC76OA
[a14amzn]: https://www.amazon.com/gp/product/B073SDGNKW

### Tools

We try to make few assumptions about what tools you have. Here are recommended tools you might need.

| Item | Quantity | Manufacturer  | Part number         | Price($) | Sources         | Notes |
| ---- |---------:| ------------- | ------------------- | --------:|-----------------| ----- |
| B1   |        1 | Preciva         | PR-3254 (PR102001)             |    39.99 | [Z][b1amzn]     | Crimping tool for HX and HV connectors |

[b1amzn]:https://www.amazon.com/gp/product/B07R1H3Z8X

**Crimper Tips:**

* Position 1 on the crimper will crimp XH contacts
* Position 2 on the crimper will crimp VH contacts.  However, with certain gauges of wire, this crimper may bend the contact into a shrimp-like shape.  If this happens, bend the contact back straight (it should be pretty obvious as it blocks insertion)
* It comes with a lot of XH contacts, but not any VH contacts.

## Assembly

As much as possible we should try to keep the cables one-to-one
(for example the wiring of the PCB to blower driver cables are all pin 1 to pin 1,
pin 2 to pin 2, etc. with the same connector style on both ends.

![Pizza pneumatics](images/wires.jpg)

### Blower wiring

For pizza builds: You will also have to crimp connectors (**A8 + A9**) to the second end of the wiring (using **B1**) that connects the
blower driver board to the [PCB](../../pcb).

The power and signal cables that come included with the blower are too short to reach the PCB, so new ones must be
manufactured using **A1, A5, A7-A9, A12-A13**. Please follow the old cables included with blower as examples.

The new cables should each be about 42cm in length.

**#TODO:** Some sort of shrink tubing should also be used

**#TODO:** illustrate this

### Stepper wiring

**#TODO:** make pigtails

Each pinch valve must also include some additional wiring and a driver board for controlling the stepper motor. A
pigtail with male dupont connectors must be manufactured for interfacing to the driver board. Furthermore, since a
single ventilator contains two pinch valves, each must come with its own driver board configured with 0-ohm resistors
soldered appropriately. All of this is documented in a subset of the PCB documentation
[here](../../pcb/rev1_export/stepper_driver_setup.md).

### Proportional solenoid valve

To connect the solenoid to the PCB, you will need to crimp a 2-pin XH connector to the wires.

**#TODO:** expand on this and illustrate this

### Oxygen sensor

**#TODO:** missing info

## Pricing totals

**#TODO:** missing info
