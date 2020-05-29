# Brain Build

This build constitutes the computing and digital user interface components.
It is enough to fully deploy and run all developed software, to test the GUI interface and
possibly the communications protocol between controller and UI. Since it contains none of the sensors
or drivers, system integration testing with this bare build is not possible.
For an upgrade, check the [Pizza build](../pizza_build.md).

**#TODO:** add pictures

## Part List (BOM)


### Purchasing Source Key

* **Z** = Amazon
* **C** = McMaster-Carr4!@(4_Prototype_Assembly/Alpha_Build_Instructions) -
* **A** = Ali Express
* **M** = Mouser
* **O** = Monoprice
* **K** = Digikey
* **R** = RS Computing ([various countries](https://www.rs-online.com/))
* **B** = Built by RespiraWorks

### Parts

**NOTE:** Shipping times are in flux! To the extent you can, attempt to find a similar product that can get to you faster.

| Item | Qty | Manufacturer | Part Number        | Price (USD) | Sources                   | Notes |
| ---- |----:| -------------| ------------------ | --------:|-------------------------- | ----- |
| 1    |   1 | ST Micro     | NUCLEO-L452-RE     |    14.43 | [M][1mouser] [R][1rs]     | STM32 Nucleo dev board, make sure to **NOT get the -P** version |
| 2    |   1 | Monoprice    | 3896               |     0.83 | [Z][2amzn] [O][2mono]     | USB-A to miniUSB-B cable, power/data for Nucleo |
| 3    |   1 | UPerfect     | -                  |    49.00 | [A][3ali]                 | 7" capacitive touchscreen with speakers, HDMI/USB |
| 4    |   1 | Raspberry Pi | RPI4-MODBP-2GB     |    53.23 | [M][4mouser]              | Raspberry PI 4 |
| 5    |   1 | Raspberry Pi | RPI3-MODBP         |    35.00 | OUT OF STOCK              | Raspberry PI 3B+ as an alternative to PI 4, **optional** substitute if you have one already|
| 6    |   1 | Monoprice    | 31201              |     8.39 | [Z][6amzn] [O][6mono]     | USB-A to USB-C cable, 3ft, **for RPI4** |
| 7    |   1 | Monoprice    | 13924              |     1.05 | [Z][7amzn] [O][7mono]     | USB-A to microUSB-B cable, 3ft, **only for RPI3B+** |
| 8    |   1 | Seadream     | 26121600           |     7.00 | [Z][8amzn]                | microHDMI adapter, **for RPI4** |
| 9    |   1 | SanDisk      | SDSQXAF-032G-GN6MA |    10.73 | [Z][9amzn]                | UHS-1 Class 10 micro SD, memory for Raspberry Pi |
| 10   |   1 | RespiraWorks | PCB                |          | [B][10rw]                 | RespiraWorks custom PCB |
| 11   |   1 |              |                    |    85.03 | [C][11mcmc]               | 12v Power supply adapter, medical grade, 9A, for **PCB** |
| 12   |   1 |              |                    |    19.99 | [Z][12amzn]               | 12v Power supply adapter, 10A, alternative for 11, for **PCB** |
| 13   |   1 |              |                    |    13.99 | [Z][13amzn]               | 12v Power supply adapter, 5A, alternative for 11-12, for **PCB** |
| 14   |   1 |              |                    |    27.00 | [C][14mcmc]               | 12v Power supply adapter, 5A, alternative for 11-13, for **PCB** |
| 15   |   1 | AIMHD        | AHD-DC20-F-PT      | 9.99 / 20| [Z][15amzn]               | Female Power Pigtail DC 5.5mm x 2.1mm Connectors, for main power, for **PCB** |
| 16   |   6 | JST          | SVH-21T-P1.1       |     0.11 | [K][16digi]               | VH crimps, for power connectors, for **PCB** |
| 17   |   1 | JST          | VHR-3N             |     0.14 | [K][17digi]               | VH housing 3 pin, for main power, for **PCB** |
| 18   |   1 | GenBasic     | 4330127279         |     6.00 | [Z][18amzn]               | Jumper cables. for **non-PCB** build
| 19   |   1 | Poyiccot     | AF-90degree_Type-C |     9.00 | [Z][19amzn]               | USB-C elbow adapter. for **non-PCB** build |

[1mouser]: https://www.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=sGAEpiMZZMtw0nEwywcFgEEYp888DlnM1Y5kGes2rJIHvcJjT1ZDkw%3D%3D
[1rs]:     https://www.rs-particuliers.com/Product.aspx?Product=1261775
[2amzn]:   https://www.amazon.com/AmazonBasics-USB-2-0-Cable-Male/dp/B00NH13S44/
[2mono]:   https://www.monoprice.com/product?p_id=3896
[3ali]:    https://www.aliexpress.com/item/4000747984746.html
[4mouser]: https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-2GB-BULK?qs=%2Fha2pyFaduiq9oc0d1uK569Mu3%252BsSMVa9bhYkyZbjQ1oNl8pHrdrS2f8pDbixKgb
[6amzn]:   https://www.amazon.com/JSAUX-Charger-Braided-Compatible-Samsung/dp/B076FPGWNZ/ref=sxin_7_ac_d_rm
[6mono]:   https://www.monoprice.com/product?p_id=31201
[7amzn]:   https://www.amazon.com/dp/B07JBN6C5C/
[7mono]:   https://www.monoprice.com/product?p_id=13924
[8amzn]:   https://www.amazon.com/Seadream-Degree-Down-toward-Adapter-Connector/dp/B01EQC345A/
[9amzn]:   https://www.amazon.com/dp/B06XWMQ81P
[10rw]:    https://github.com/respiraworks/pcbreathe
[11mcmc]:  https://www.mcmaster.com/3791N024
[12amzn]:  https://www.amazon.com/gp/product/B00Z9X4GLW
[13amzn]:  https://www.amazon.com/gp/product/B01MYXTA6N
[14mcmc]:  https://www.mcmaster.com/3824N132
[15amzn]:   https://www.amazon.com/Connectors-Upgraded-Surveillance-Transformer-Connection/dp/B0768V9V5Q
[16digi]:   https://www.digikey.com/short/z44fjr
[17digi]:   https://www.digikey.com/short/z44fwp
[18amzn]:  https://www.amazon.com/GenBasic-Piece-Female-Jumper-Wires/dp/B01L5ULRUA/
[19amzn]:  https://www.amazon.com/Poyiccot-2-Pack-Degree-Adapter-Extension/dp/B071XHQJG8/

### Tools

| Item | Manufacturer  | Part number        | Price (USD) | Sources         | Notes |
| ---- |--------------:| ------------------ | -------- | ---------------:|------ |
| a1   |               |                    |    12.99 | [Z][a1amzn]     | microSD-USB adapter, for flashing RasPi images | 
| a2   | Preciva       | PR-3254 (PR102001) |    39.99 | [Z][a2amzn]     | Crimping tool for HX and HV connectors |

[a1amzn]:https://www.amazon.com/Anker-Portable-Reader-RS-MMC-Micro/dp/B006T9B6R2
[a2amzn]:https://www.amazon.com/gp/product/B07R1H3Z8X

**NOTE: Items 15-17 and a2 above are also duplicated in the [wiring BOM](../../../1_Ventilator_System_Design/Electrical_System/wiring.md)**

### Purchasing Guide

1. First of all, contact Edwin, Martin or Ethan to see if there are extra parts in the RespiraWorks inventory that
can be sent to you.
2. Take note of the specifically RPI3+ or RPI4 components. Don't get the wrong ones.
3. If you are not (yet) able to get a PCB, get items 18 and 19
4. If you are getting a PCB, and a power cable is included, you will not need items 15-17, nor a2
5. If you are feeling stingy, don't get the touchscreen (item 3). You can connect the RasPi to any monitor and use
a mouse and keyboard to control it.

## Assembly Instructions

1. Mount touchscreen legs.
2. Mount RasPi to screen.
3. Connect HDMI flex cable from screen to RasPi. If using raspi4, use the adapter and included pigtail.
4. Power screen from powered USB hub or wall power.
5. Power RasPi 3b+/4 with μUSB/USB-C cable.
6. Connect RasPi USB to 5V_TS microUSB on screen.
7. Connect mini USB to Nucleo and USB to your laptop, or raspi, if you're feeling frisky.
8. Install OS on μSD card and plug it into RasPi.
9. If you got a PCB, go [here](https://github.com/respiraworks/pcbreathe) for further instructions
10. If you did not get a PCB, it's jumper wire time to connect Nucleo to RasPi.
Here is reference for [RasPi pinout](https://pinout.xyz/pinout/pin2_5v_power) and here it is for the [Nucleo](https://docs.zephyrproject.org/latest/_images/nucleo-l452re-pinout.png).
    - Raspi UART-TXD (pin 8) <--> Nucleo USART3-RX (CN10 pin 18)
    - Raspi UART-RXD (pin 10) <--> Nucleo USART3-TX (CN10 pin 25)
    - Raspi GPIO12 (pin 32) <--> Nucleo USART3-RTS (CN10 pin 30)
    - Raspi GPIO13 (pin 33) <--> Nucleo USART3-CTS (CN10 pin 28)
