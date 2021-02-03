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
- If you are feeling stingy, don't get the touchscreen (**A3**). You can connect the RasPi to any monitor and use
a mouse and keyboard to control it.

[ppg]: ../../manufacturing/purchasing_guidelines.md

| Item  | Qty | Manufacturer  | Part Number        | Price (USD) | Sources[*][ppg]         | Notes |
| ----  |----:| --------------| ------------------ | -----------:|:-----------------------:| ----- |
|**A1** | 1   | ST Micro      | NUCLEO-L452-RE     | 14.43       | [M][a1mouser] [R][a1rs] | STM32 Nucleo dev board, make sure to **NOT get the -P** version |
|**A2** | 1   | Monoprice     | 3896               | 0.83        | [Z][a2amzn] [O][a2mono] | USB-A to miniUSB-B cable, power/data for Nucleo |
|**A3** | 1   | UPerfect      | -                  | 49.00       | [A][a3ali]              | 7" capacitive touchscreen with speakers, HDMI/USB |
|**A4** | 1   | Raspberry Pi  | RPI4-MODBP-2GB     | 53.23       | [M][a4mouser]           | Raspberry PI 4 |
|**A5** | 1   | Monoprice     | 31201              | 8.39        | [Z][a5amzn] [O][a5mono] | USB-A to USB-C cable, 3ft |
|**A6** | 1   | Seadream      | 26121600           | 7.00        | [Z][a6amzn]             | microHDMI adapter |
|**A7** | 1   | SanDisk       | SDSQXAF-032G-GN6MA | 10.73       | [Z][a7amzn]             | UHS-1 Class 10 micro SD, memory for Raspberry Pi |
|**A8** | 1   | RespiraWorks  | PCB                |             | [Rw][a8rw]              | RespiraWorks custom PCB |
|**A9** | 1   | Mean Well     | GSM160A12-R7B      | 63.99       | [K][a9key]              | 12v Power supply adapter, medical grade, 11.5A |
|**A10**| 2   | Digikey       | X-NUCLEO-IHM03A1   | 10.64       | [K][a10key] [M][a10mau] | Stepper driver dev board |
|**A11**| (1) | ST            | X-NUCLEO-IHM02A1   | 15.00       | [S][a11st] [R][a11rs]   | Two-axis stepper driver dev board, **Alternative for item 10:**  |

[a1mouser]: https://www.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=sGAEpiMZZMtw0nEwywcFgEEYp888DlnM1Y5kGes2rJIHvcJjT1ZDkw%3D%3D
[a1rs]:     https://export.rsdelivers.com/product/stmicroelectronics/nucleo-l452re/stmicroelectronics-stm32-nucleo-64-mcu/1261775
[a2amzn]:   https://www.amazon.com/AmazonBasics-USB-2-0-Cable-Male/dp/B00NH13S44/
[a2mono]:   https://www.monoprice.com/product?p_id=3896
[a3ali]:    https://www.aliexpress.com/item/4000747984746.html
[a4mouser]: https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-2GB-BULK?qs=%2Fha2pyFaduiq9oc0d1uK569Mu3%252BsSMVa9bhYkyZbjQ1oNl8pHrdrS2f8pDbixKgb
[a5amzn]:   https://www.amazon.com/JSAUX-Charger-Braided-Compatible-Samsung/dp/B076FPGWNZ/ref=sxin_7_ac_d_rm
[a5mono]:   https://www.monoprice.com/product?p_id=31201
[a6amzn]:   https://www.amazon.com/Seadream-Degree-Down-toward-Adapter-Connector/dp/B01EQC345A/
[a7amzn]:   https://www.amazon.com/dp/B06XWMQ81P
[a8rw]:    ../../pcb
[a9key]:  https://www.digikey.com/en/products/detail/mean-well-usa-inc/GSM160A12-R7B/7703457
[a20key]:  https://www.digikey.com/short/z442qt
[a20mau]:  https://www.mouser.com/ProductDetail/511-X-NUCLEO-IHM03A1
[a11st]:   https://www.st.com/en/ecosystems/x-nucleo-ihm02a1.html#sample-and-buy
[a11rs]:   https://export.rsdelivers.com/product/stmicroelectronics/x-nucleo-ihm02a1/stmicroelectronics-x-nucleo-ihm02a1-two-axis/1646982

### Tools

| Item | Manufacturer  | Part number        | Price (USD) | Sources[*][ppg] | Notes |
| ---- |--------------:| ------------------ | -----------:|:---------------:|------ |
|**B1**| Anker         | 68ANREADER-B2A     | 12.99       | [Z][b1amzn]     | microSD-USB adapter, for flashing RasPi images |

[b1amzn]:https://www.amazon.com/Anker-Portable-Reader-RS-MMC-Micro/dp/B006T9B6R2

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
