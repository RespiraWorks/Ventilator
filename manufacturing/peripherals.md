# Peripheral components

These components are not stricly part of the ventilator assembly, but they may be necessary to get the prototype to
work. The most obvious of these would be the power supply cables. Other devices might also be needed depending on
how you intend to deploy software, test and control the prototype.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: purchasing_guidelines.md

| Item | Quantity | Manufacturer  | Part Number        | Price (USD) | Sources[*][ppg] | Notes |
| ---- |---------:| --------------| ------------------ | -----------:|:---------------:| ----- |
|**A1**| 1        | Mean Well     | GSM160A12-R7B      | 63.99       |   [K][a1key]    | 12v Power supply adapter, medical grade, 11.5A |
|**A2**| 1        | Cablelera     | ZWACPCAY-10        | 8.99        |   [Z][a2amzn]   | Hospital grade power cord |
|**A3**| (1)      | Anker         | 68ANREADER-B2A     | 12.99       |   [Z][a3amzn]   | microSD-USB adapter, for flashing RasPi images |
|**A4**| (1)      | Monoprice     | 3896               | 0.83        |  [Z][a4amzn]    | USB-A to miniUSB-B cable, for programming the nucleo |
|**A5**| (1)      | VicTsing      | IC192305US         | 23.99       |   [Z][a5amzn]   | Wireless keyboard and mouse, for setting up Rpi, **OPTIONAL** |

**Essentials (A1+A2):** USD 72.98

[a1key]:  https://www.digikey.com/en/products/detail/mean-well-usa-inc/GSM160A12-R7B/7703457
[a2amzn]: https://www.amazon.com/Cablelera-North-American-Hospital-ZWACPCAY-10/dp/B00GP6CB5A
[a3amzn]: https://www.amazon.com/Anker-Portable-Reader-RS-MMC-Micro/dp/B006T9B6R2
[a4amzn]: https://www.amazon.com/AmazonBasics-USB-2-0-Cable-Male/dp/B00NH13S44/
[a5amzn]: https://www.amazon.com/VicTsing-Keyboard-Adjustable-Independent-Indicator/dp/B07TT3VN4X


### Power

Once the ventilator is assembled, you may use `[A1]` and `[A2]` power it.

You may also want to use `[A4]` to connect the controller to your PC from which you will be flashing
controller firmware.
