# Brain build

**DISCLAIMER: These are live documents.
There are duplicate and alternative items for undecided aspects of the physical design. 
RespiraWorks MAY NOT compensate you for any of these parts.
If you decide to buy them, do so at your own risk.**

This build constitutes the computing and digital user interface components.
It is enough to fully deploy and run all developed software, to test the GUI interface and
possibly the communications protocol between controller and UI. Since it contains none of the sensors
or drivers, integration testing with this bare build is not possible. For an upgrade, check the 
[Pizza build](pizza_build.md).

## Purchasing source abbreviations

* **Z** = Amazon
* **A** = Ali Express
* **M** = Mouser
* **C** = McMaster-Carr
* **O** = Monoprice
* **R** = RS Computing ([various countries](https://www.rs-online.com/))
* **B** = Built by RespiraWorks

## Parts BOM

| Refdes       | Qty | Manufacturer | Part Number        | Price($) | Sources                          | Notes |
| ------------ |----:| -------------| ------------------ | --------:|--------------------------------- | ----- |
| BRAIN-01     |   1 | ST Micro     | NUCLEO-L452-RE     |    14.00 | [M][1mouser] [R][1rs]            | STM32 Nucleo dev board, make sure to NOT get the **-P** version |
| BRAIN-02     |   1 | Monoprice    | 3896               |     0.83 | [Z][2amzn] [O][2mono]            | USB-A to miniUSB-B cable, power/data for Nucleo |
| BRAIN-03     |   1 | UPerfect     | -                  |    49.00 | [A][3ali]                        | 7" capacitive touchscreen with speakers, HDMI/USB |
| BRAIN-11     |   1 | RespiraWorks | Carrier PCB, Rev A |      N/A | [B][12rw]                        | RespiraWorks custom PCB |
| CONSUMABLE-a |   1 | GenBasic     | 4330127279         |     6.00 | [Z][4amzn] | Jumper cables.Optional if you don't have the PCB (BRAIN-11)
| BRAIN-06     |   1 | Raspberry Pi | RPI4-MODBP-2GB     |    53.23 | [M][6mouser]                     | Raspberry PI 4 |
| BRAIN-06-alt |   1 | Raspberry Pi | RPI3-MODBP         |    35.00 | OUT OF STOCK                     | Raspberry PI 3B+ as an alternative to PI 4|
| BRAIN-07     |   1 | Monoprice    | 31201              |     8.03 | [Z][7amzn] [O][7mono]            | USB-A to USB-C cable, 3ft, for RPI4 |
| BRAIN-07-alt |   1 | Monoprice    | 13924              |     1.05 | [Z][7-alt-amzn] [O][7-alt-mono]  | USB-A to microUSB-B cable, 3ft, for RPI3B+ |
| BRAIN-08     |   1 | Seadream     | 26121600           |     7.00 | [Z][8amzn]                       | microHDMI adapter, for RPI4 |
| BRAIN-09     |   1 | SanDisk      | SDSQXAF-032G-GN6MA |    10.73 | [Z][9amzn]                       | UHS-1 Class 10 micro SD |
| CONSUMABLE-b |   1 | Poyiccot     | AF-90degree_Type-C |     9.00 | [Z][10amzn]                      | optional USB-C elbow adapter. Mounted on touch screen, in case of non-PCB build. |


## Tooling BOM

| Tools               | Price($) | Sources         | Notes |
|---------------------|---------:|-----------------|-------|
| microSD-USB adapter |    12.00 | [Z][5amzn]      | for flashing raspi images |

[1mouser]:https://www.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=sGAEpiMZZMtw0nEwywcFgEEYp888DlnM1Y5kGes2rJIHvcJjT1ZDkw%3D%3D
[1rs]:https://www.rs-particuliers.com/Product.aspx?Product=1261775
[2amzn]:https://www.amazon.com/AmazonBasics-USB-2-0-Cable-Male/dp/B00NH13S44/
[3ali]:https://www.aliexpress.com/item/4000747984746.html
[4amzn]:https://www.amazon.com/GenBasic-Piece-Female-Jumper-Wires/dp/B01L5ULRUA/
[5amzn]:https://www.amazon.com/Anker-Portable-Reader-RS-MMC-Micro/dp/B006T9B6R2
[6mouser]:https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-2GB-BULK?qs=%2Fha2pyFaduiq9oc0d1uK569Mu3%252BsSMVa9bhYkyZbjQ1oNl8pHrdrS2f8pDbixKgb
[7amzn]:https://www.amazon.com/JSAUX-Charger-Braided-Compatible-Samsung/dp/B076FPGWNZ/ref=sxin_7_ac_d_rm
[7-alt-amzn]:https://www.amazon.com/dp/B07JBN6C5C/
[8amzn]:https://www.amazon.com/Seadream-Degree-Down-toward-Adapter-Connector/dp/B01EQC345A/
[9amzn]:https://www.amazon.com/dp/B06XWMQ81P
[10amzn]:https://www.amazon.com/Poyiccot-2-Pack-Degree-Adapter-Extension/dp/B071XHQJG8/
[2mono]:https://www.monoprice.com/product?p_id=3896
[7mono]:https://www.monoprice.com/product?p_id=31201
[7-alt-mono]:https://www.monoprice.com/product?p_id=13924

## Brain build instructions
Here is the "brain build" for devs to get up and running.

NOTE: Shipping times are in flux! To the extent you can, attempt to find a similar product that can get to you faster.

Definitely get these:
- [Nucleo board with STM32 Microcontroller](https://www.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=sGAEpiMZZMtw0nEwywcFgEEYp888DlnM1Y5kGes2rJIHvcJjT1ZDkw%3D%3D)
- [mini-USB cable](https://www.amazon.com/AmazonBasics-USB-2-0-Cable-Male/dp/B00NH13S44/)
- [7" touchscreen](https://www.aliexpress.com/item/4000747984746.html)
- [jumper wires](https://www.amazon.com/GenBasic-Piece-Female-Jumper-Wires/dp/B01L5ULRUA/)
- [μSD-USB adapter](https://www.amazon.com/Anker-Portable-Reader-RS-MMC-Micro/dp/B006T9B6R2)

If you already have a working Raspberry-Pi 3b:
- [μUSB cable](https://www.amazon.com/dp/B07JBN6C5C/)

If you DO NOT have a Raspberry-Pi, we recommend getting:
- [Raspberry-Pi 4](https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-2GB-BULK?qs=%2Fha2pyFaduiq9oc0d1uK569Mu3%252BsSMVa9bhYkyZbjQ1oNl8pHrdrS2f8pDbixKgb)
- [μHDMI adapter](https://www.amazon.com/Seadream-Degree-Down-toward-Adapter-Connector/dp/B01EQC345A/)
- [μSD card](https://www.amazon.com/dp/B06XWMQ81P)
- [USB type-C cable](https://www.amazon.com/JSAUX-Charger-Braided-Compatible-Samsung/dp/B076FPGWNZ/ref=sxin_7_ac_d_rm)

Nice to haves
- Ethernet cable
- Powered USB hub
- [Elbow USB-C adapter](https://www.amazon.com/Poyiccot-2-Pack-Degree-Adapter-Extension/dp/B071XHQJG8/)
- Water (PSA: stay hydrated!)

Assembly
1. Mount touchscreen legs.
2. Mount RasPi to screen.
3. Connect HDMI flex cable from screen to RasPi. If using raspi4, use the adapter and included pigtail.
4. Power screen from powered USB hub or wall power.
5. Power RasPi 3b+/4 with μUSB/USB-C cable.
6. Connect RasPi USB to 5V_TS microUSB on screen.
7. Connect mini USB to Nucleo and USB to your laptop, or raspi, if you're feeling frisky.
8. Jumper wire time. Here is reference for [RasPi pinout](https://pinout.xyz/pinout/pin2_5v_power) and here it is for the [Nucleo](https://docs.zephyrproject.org/latest/_images/nucleo-l452re-pinout.png).
    - Raspi UART-TXD (pin 8) <--> Nucleo USART3-RX (CN10 pin 18)
    - Raspi UART-RXD (pin 10) <--> Nucleo USART3-TX (CN10 pin 25)
    - Raspi GPIO12 (pin 32) <--> Nucleo USART3-RTS (CN10 pin 30)
    - Raspi GPIO13 (pin 33) <--> Nucleo USART3-CTS (CN10 pin 28)
9. Install OS on μSD card and plug it into RasPi.
