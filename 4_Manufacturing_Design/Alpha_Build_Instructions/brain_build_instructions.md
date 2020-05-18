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