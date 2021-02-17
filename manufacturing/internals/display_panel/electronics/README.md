# Electrical system assembly

This build constitutes the electrical, computing and digital user interface components.

**#TODO:** add pictures

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part Number        | Price (USD) | Sources[*][ppg]         | Notes |
| ----  |---------:| --------------| ------------------ | -----------:|:-----------------------:| ----- |
|**A1** | 1        | RespiraWorks  | PCB                |             | [Rw][a1rw]              | RespiraWorks custom PCB |
|**A2** | (1)      | RespiraWorks  | PCB_interim_board  |             | [Rw][a2rw]              | RespiraWorks Rev1 interim board **OPTIONAL** |
|**A3** | (1)      | Duracell      | 2016 3V            | 1.67        | [Z][a3amzn]             | CR2016 battery, for interim board **OPTIONAL** |
|**A4** | 1        | ST Micro      | NUCLEO-L452-RE     | 14.43       | [M][a4mous] [R][a4rs]   | STM32 Nucleo dev board, **DO NOT get the -P** version |
|**A5** | 2        | Digikey       | X-NUCLEO-IHM03A1   | 10.64       | [K][a5key] [M][a5mous]  | Stepper driver board |
|**A6** | (1)      | ST            | X-NUCLEO-IHM02A1   | 15.00       | [S][a6st] [R][a6rs]     | Two-axis stepper driver board, **Alternative to 2x`[A5]`**  |
|**A7** | 1        | Raspberry Pi  | RPI4-MODBP-2GB     | 53.23       | [M][a7mous]             | Raspberry Pi 4 |
|**A8** | 1        | Pastall       | PT2019120501       | 9.99 / 40   | [Z][a8amzn]             | Heat sink for Rpi |
|**A9** | 1        | SanDisk       | SDSQXAF-032G-GN6MA | 10.73       | [Z][a9amzn]             | UHS-1 Class 10 micro SD, memory for Raspberry Pi |
|**A10**| 1        | UPerfect      | -                  | 49.00       | [A][a10ali]             | 7" capacitive touchscreen with speakers |
|**A11**| 1        | UGREEN        | B0773N757H         | 6.99        | [Z][a11amzn]            | USB-A to micro-USB cable, 1.5ft |
|**A12**| 1        | Seadream      | 26121600           | 7.00        | [Z][a12amzn]            | microHDMI adapter |
|**A13**| 1        | C2G           | 56781              | 7.99        | [Z][a13amzn]            | HDMI cable, 1ft |
|**A14**| 4        | McMaster-Carr | 92095A458          | 4.97 / 100  | [C][a14mcmc]            | M2.5 screw, 6mm |
|**A15**| 4        | DigiKey       | 732-10395-ND       | 48.40/100   | [K][a15key]             | M2.5 standoff, 9mm tall |
|**A16**| 4        | DigiKey       | 732-10435-ND       | 50.82/100   | [K][a16key]             | M2.5 standoff, 20mm tall |

>**TODO:** add miniUSB cable for embedded deployment

>**TODO:** where do standoffs go?

[a1rw]:    ../../../../pcb
[a2rw]:    https://github.com/inceptionev/VentilatorRev1InterimDaughtercard
[a3amzn]:  https://www.amazon.com/Duracell-Lithium-Battery-lasting-battery/dp/B00006JPGV
[a4mous]:  https://www.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-L452RE?qs=sGAEpiMZZMtw0nEwywcFgEEYp888DlnM1Y5kGes2rJIHvcJjT1ZDkw%3D%3D
[a4rs]:    https://export.rsdelivers.com/product/stmicroelectronics/nucleo-l452re/stmicroelectronics-stm32-nucleo-64-mcu/1261775
[a5key]:   https://www.digikey.com/short/z442qt
[a5mous]:  https://www.mouser.com/ProductDetail/511-X-NUCLEO-IHM03A1
[a6st]:    https://www.st.com/en/ecosystems/x-nucleo-ihm02a1.html#sample-and-buy
[a6rs]:    https://export.rsdelivers.com/product/stmicroelectronics/x-nucleo-ihm02a1/stmicroelectronics-x-nucleo-ihm02a1-two-axis/1646982
[a7mous]:  https://www.mouser.com/ProductDetail/Raspberry-Pi/RPI4-MODBP-2GB-BULK?qs=%2Fha2pyFaduiq9oc0d1uK569Mu3%252BsSMVa9bhYkyZbjQ1oNl8pHrdrS2f8pDbixKgb
[a8amzn]:  https://www.amazon.com/Raspberry-Heatsink-Conductive-Adhesive-Aluminum/dp/B082RKKQ2D
[a9amzn]:  https://www.amazon.com/dp/B06XWMQ81P
[a10ali]:  https://www.aliexpress.com/item/4000747984746.html
[a11amzn]: https://www.amazon.com/UGREEN-Braided-Charger-Charging-Controller/dp/B01MTXZ3U8
[a12amzn]: https://www.amazon.com/Seadream-Degree-Down-toward-Adapter-Connector/dp/B01EQC345A/
[a13amzn]: https://www.amazon.com/C2G-56781-Ethernet-Devices-Chromebooks/dp/B00J4LAKQA
[a14mcmc]: https://www.mcmaster.com/92095A458/
[a15key]:  https://www.digikey.com/en/products/detail/w-rth-elektronik/971090151/6174614
[a16key]:  https://www.digikey.com/en/products/detail/würth-elektronik/971200151/6174654


**Note:** If you are feeling stingy, don't get the touchscreen `[A10]`. You can connect the RasPi to any monitor and
use a mouse and keyboard to control it.

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../../methods).**

### Custom wiring

The ventilator assembly requires some custom wiring. Please use materials and instructions on the
[wiring page](../../../wiring) to prepare those custom components first.

### Main circuit board (PCB)

Design and manufacturing files for the main board can be found on the
[PCB Rev1 page](../../../../pcb/rev1_export).

Revision 1 of the custom circuit board (PCB) requires some modification to work with the current prototype.
Please follow instructions on that page to make the necessary modifications.

If you are building an enclosed ventilator prototype, you may stop following the PCB instructions right before
installation of the Raspberry Pi. Follow the sequence of steps below instead.

**#TODO:** does errata explain how to do the bodge wire?

### Cycle controller

The Nucleo dev board serves as the main cycle controller processor and is to be installed onto the PCB.

**#TODO:** move relevant parts of PCB doc and/or illustrate

### Interim board

You might want to install an interim board `[A2]` if you intend to test any of the following functionality:
* oxygen control
* EEPROM nonvolatile memory
* real-time clock

Design and manufacturing files can be found in this
[separate repository](https://github.com/inceptionev/VentilatorRev1InterimDaughtercard).

You will need to install a battery `[A3]`.

The card goes directly onto the Nucleo, **before** the stepper drivers.

You will probably also want to connect the oxygen sensor wire to this board before stacking the stepper drivers.

**#TODO:** pictures

### Stepper drivers

Pinch valves require either 2 stepper driver boards `[A5]` or a single 2-axis board `[A6]`.

Each pinch valve must also include some additional wiring and a driver board for controlling the stepper motor. A
pigtail with male dupont connectors must be manufactured for interfacing to the driver board. Furthermore, since a
single ventilator contains two pinch valves, each must come with its own driver board configured with 0-ohm resistors
soldered appropriately.

It is advisable that you first manufacture the 2 pigtails of appropriate length as described on the
[wiring page](../../../wiring).

Stepper board modification and mounting to Nucleo is documented [here](stepper_drivers).

Now you can connect the steppers to their driver pigtails as follows:
* top - inhale
* bottom - exhale

### Raspberry Pi

The Raspberry Pi serves as the user interface computer for the ventilator.

Prior to installing it on the PCB, you should:
* install a heat sink `[A8]` on the processor
* load the memory card `[A9]` with the standard [RaspberryPi OS](https://www.raspberrypi.org/software/),
  using a microSD adapter `[B1]`
* install memory card into the RaspberryPi
* give it a keyboard and mouse `[B3]` or
* alternatively, you can configure it for remote ssh control as described on our [GUI page](../../../../software/gui).

Unless you are doing a [Pizza build](../../../../quality-assurance/testing/pizza_build), you will **NOT** want to install
the Raspberry Pi onto the PCB quite yet. You will first want to mount the PCB onto the touch-screen, as described on the
[display panel sub-assembly](..) page.

### Touch screen

The touch screen needs two connections:
* RaspberryPi's `USB` to touchscreen's `5V+Touch` microUSB, using `[A11]`
* RaspberryPi's `microHDMI` to touchscreen's `HDMI IN`, using `[A12]` and `[A13]`

**#TODO:** pictures

### Final wiring

By this point you should have completed all the mechanical and pneumatic sub-assemblies of the ventilator.

* connect blower power wires to CPB
* connect blower control wires
  * to the interim board, if you have one
  * else, to the PCB
* connect proportional solenoid to PCB
* connect oxygen sensor to interim board
* connect stepper motors to the stepper driver pigtails as follows:
  * top - inhale
  * bottom - exhale
* connect power supply cable to PCB
