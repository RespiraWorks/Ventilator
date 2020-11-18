# Brain Build

This build constitutes the computing and digital user interface components.
It is enough to fully deploy and run all developed software, to test the GUI interface and
possibly the communications protocol between controller and UI. Since it contains none of the sensors
or drivers, system integration testing with this bare build is not possible.
For an upgrade, check the [Pizza build](../pizza_build).

**#TODO:** add pictures

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

Additional notes:
- Take note of the specifically RPI3+ or RPI4 components. Don't get the wrong ones.
- If you are not (yet) able to get a PCB, get **A18 & A19**
- If you are getting a PCB, and a power cable is included, you will not need **A15-17**, nor **B2**
- If you are feeling stingy, don't get the touchscreen (**A3**). You can connect the RasPi to any monitor and use
a mouse and keyboard to control it.

[ppg]: ../../manufacturing/purchasing_guidelines.md

| Item  | Qty | Manufacturer  | Part Number        | Price (USD) | Sources[*][ppg]         | Notes |
| ----  |----:| --------------| ------------------ | -----------:|:-----------------------:| ----- |
|**A1** | 1   | ST Micro      | NUCLEO-L452-RE     | 14.43       | [M][a1mouser] [R][a1rs] | STM32 Nucleo dev board, make sure to **NOT get the -P** version |
|**A2** | 1   | Monoprice     | 3896               | 0.83        | [Z][a2amzn] [O][a2mono] | USB-A to miniUSB-B cable, power/data for Nucleo |
|**A3** | 1   | UPerfect      | -                  | 49.00       | [A][a3ali]              | 7" capacitive touchscreen with speakers, HDMI/USB |
|**A4** | 1   | Raspberry Pi  | RPI4-MODBP-2GB     | 53.23       | [M][a4mouser]           | Raspberry PI 4 |
|**A5** | 1   | Raspberry Pi  | RPI3-MODBP         | 35.00       | If you have one already | Raspberry PI 3B+ as an alternative to PI 4, **optional** substitute if you have one already|
|**A6** | 1   | Monoprice     | 31201              | 8.39        | [Z][a6amzn] [O][a6mono] | USB-A to USB-C cable, 3ft, **for RPI4** |
|**A7** | 1   | Monoprice     | 13924              | 1.05        | [Z][a7amzn] [O][a7mono] | USB-A to microUSB-B cable, 3ft, **only for RPI3B+** |
|**A8** | 1   | Seadream      | 26121600           | 7.00        | [Z][a8amzn]             | microHDMI adapter, **for RPI4** |
|**A9** | 1   | SanDisk       | SDSQXAF-032G-GN6MA | 10.73       | [Z][a9amzn]             | UHS-1 Class 10 micro SD, memory for Raspberry Pi |
|**A10**| 1   | RespiraWorks  | PCB                |             | [B][a10rw]              | RespiraWorks custom PCB |
|**A11**| 1   | McMaster Carr | 3791N024           | 85.03       | [C][a11mcmc]            | 12v Power supply adapter, medical grade, 9A, for **PCB** |
|**A12**| 1   | BINZET        | FBA_LTC0487-3      | 19.99       | [Z][a12amzn]            | 12v Power supply adapter, 10A, alternative for A11, for **PCB** |
|**A13**| 1   | HISVISION     | AY60WDC            | 13.99       | [Z][a13amzn]            | 12v Power supply adapter, 5A, alternative for A11-12, for **PCB** |
|**A14**| 1   | McMaster Carr | 3824N132           | 27.00       | [C][a14mcmc]            | 12v Power supply adapter, 5A, alternative for A11-13, for **PCB** |
|**A15**| 1   | AIMHD         | AHD-DC20-F-PT      | 9.99 / 20   | [Z][a15amzn]            | Female Power Pigtail DC 5.5mm x 2.1mm Connectors, for main power, for **PCB** |
|**A16**| 6   | JST           | SVH-21T-P1.1       | 0.11        | [K][a16digi] [R][a16rs] | VH crimps, for power connectors, for **PCB** |
|**A17**| 1   | JST           | VHR-3N             | 0.14        | [K][a17digi] [R][a17rs] | VH housing 3 pin, for main power, for **PCB** |
|**A18**| 1   | GenBasic      | 4330127279         | 6.00        | [Z][a18amzn]            | Jumper cables. for **non-PCB** build
|**A19**| 1   | Poyiccot      | AF-90degree_Type-C | 9.00        | [Z][a19amzn]            | USB-C elbow adapter. for **non-PCB** build |

[a1mouser]: https://www.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=sGAEpiMZZMtw0nEwywcFgEEYp888DlnM1Y5kGes2rJIHvcJjT1ZDkw%3D%3D
[a1rs]:     https://export.rsdelivers.com/product/stmicroelectronics/nucleo-l452re/stmicroelectronics-stm32-nucleo-64-mcu/1261775
[a2amzn]:   https://www.amazon.com/AmazonBasics-USB-2-0-Cable-Male/dp/B00NH13S44/
[a2mono]:   https://www.monoprice.com/product?p_id=3896
[a3ali]:    https://www.aliexpress.com/item/4000747984746.html
[a4mouser]: https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-2GB-BULK?qs=%2Fha2pyFaduiq9oc0d1uK569Mu3%252BsSMVa9bhYkyZbjQ1oNl8pHrdrS2f8pDbixKgb
[a6amzn]:   https://www.amazon.com/JSAUX-Charger-Braided-Compatible-Samsung/dp/B076FPGWNZ/ref=sxin_7_ac_d_rm
[a6mono]:   https://www.monoprice.com/product?p_id=31201
[a7amzn]:   https://www.amazon.com/dp/B07JBN6C5C/
[a7mono]:   https://www.monoprice.com/product?p_id=13924
[a8amzn]:   https://www.amazon.com/Seadream-Degree-Down-toward-Adapter-Connector/dp/B01EQC345A/
[a9amzn]:   https://www.amazon.com/dp/B06XWMQ81P
[a10rw]:    ../../pcb
[a11mcmc]:  https://www.mcmaster.com/3791N024
[a12amzn]:  https://www.amazon.com/gp/product/B00Z9X4GLW
[a13amzn]:  https://www.amazon.com/gp/product/B01MYXTA6N
[a14mcmc]:  https://www.mcmaster.com/3824N132
[a15amzn]:   https://www.amazon.com/Connectors-Upgraded-Surveillance-Transformer-Connection/dp/B0768V9V5Q
[a16digi]:   https://www.digikey.com/short/z44fjr
[a16rs]:    https://export.rsdelivers.com/product/jst/svh-41t-p11/jst-nv-vh-female-crimp-terminal-contact-16awg-svh/7620692
[a17digi]:   https://www.digikey.com/short/z44fwp
[a17rs]:    https://export.rsdelivers.com/product/jst/vhr-3n/jst-vhr-female-connector-housing-396mm-pitch-3/8201175
[a18amzn]:  https://www.amazon.com/GenBasic-Piece-Female-Jumper-Wires/dp/B01L5ULRUA/
[a19amzn]:  https://www.amazon.com/Poyiccot-2-Pack-Degree-Adapter-Extension/dp/B071XHQJG8/

### Tools

| Item | Manufacturer  | Part number        | Price (USD) | Sources[*][ppg] | Notes |
| ---- |--------------:| ------------------ | -----------:|:---------------:|------ |
|**B1**| Anker         | 68ANREADER-B2A     | 12.99       | [Z][b1amzn]     | microSD-USB adapter, for flashing RasPi images |
|**B2**| Preciva       | PR-3254 (PR102001) | 39.99       | [Z][b2amzn]     | Crimping tool for HX and HV connectors |

[b1amzn]:https://www.amazon.com/Anker-Portable-Reader-RS-MMC-Micro/dp/B006T9B6R2
[b2amzn]:https://www.amazon.com/gp/product/B07R1H3Z8X

**NOTE: Items **A15-17** and **B2** above are also duplicated in the [wiring BOM](../../design/electrical-system/wiring.md)**

## Assembly Instructions

1. Mount touchscreen legs.
2. Mount RasPi to screen.
3. Connect HDMI flex cable from screen to RasPi. If using raspi4, use the adapter and included pigtail.
4. Power screen from powered USB hub or wall power.
5. Power RasPi 3b+/4 with μUSB/USB-C cable.
6. Connect RasPi USB to 5V_TS microUSB on screen.
7. Connect mini USB to Nucleo and USB to your laptop, or raspi, if you're feeling frisky.
8. Install OS on μSD card and plug it into RasPi.
9. If you got a PCB, go [here](../../pcb) for further instructions
10. If you did not get a PCB, it's jumper wire time to connect Nucleo to RasPi.
Here is reference for [RasPi pinout](https://pinout.xyz/pinout/pin2_5v_power) and here it is for the [Nucleo](https://docs.zephyrproject.org/latest/_images/nucleo_l452re_pinout.png).
    - Raspi UART-TXD (pin 8) <--> Nucleo USART3-RX (CN10 pin 18)
    - Raspi UART-RXD (pin 10) <--> Nucleo USART3-TX (CN10 pin 25)
    - Raspi GPIO12 (pin 32) <--> Nucleo USART3-RTS (CN10 pin 30)
    - Raspi GPIO13 (pin 33) <--> Nucleo USART3-CTS (CN10 pin 28)
