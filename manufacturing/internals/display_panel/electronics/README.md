# Electronics assembly

|   |   |
|---|---|
| ![](images/rendering_2.jpg) | ![](images/rendering_1.jpg) |

This build constitutes the electrical, computing and digital user interface components.

**Note:** The Main board (required) and Interim Board (optional) are custom components and will need to be fabricated
by a contract manufacturer before proceeding.

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../../purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part Number            | Price (USD) | Sources[*][ppg]        | Notes |
| ----  |---------:| --------------| ---------------------- | -----------:|:----------------------:| ----- |
|**A1** | 1        | RespiraWorks  | [Mainboard][a1rw]      | 300.00      | Rw                     | Custom main board |
|**A2** | (1)      | RespiraWorks  |[Interim board][a2rw]   | 87.00       | Rw                     | Rev1 interim board **OPTIONAL** |
|**A3** | (1)      | Duracell      | 2016 3V                | 1.67        | [Z][a3amzn]            | CR2016 battery, for interim board **OPTIONAL** |
|**A4** | 1        | ST Micro      | NUCLEO-L452-RE         | 14.43       | [M][a4mous] [R][a4rs]  | STM32 Nucleo dev board, **DO NOT get the -P** version |
|**A5** | 2        | Digikey       | X-NUCLEO-IHM03A1       | 10.64       | [K][a5key] [M][a5mous] | Stepper driver board |
|**A6** | (1)      | ST            | X-NUCLEO-IHM02A1       | 15.00       | [S][a6st] [R][a6rs]    | Two-axis stepper driver board, **Alternative to 2x`[A5]`**  |
|**A7** | 1        | Raspberry Pi  | RPI4-MODBP-2GB         | 53.23       | [M][a7mous]            | Raspberry Pi 4 |
|**A8** | 1        | Pastall       | PT2019120501           | 0.25        | [Z][a8amzn]            | Heat sink for Rpi |
|**A9** | 1        | SanDisk       | SDSQXAF-032G-GN6MA     | 10.73       | [Z][a9amzn]            | UHS-1 Class 10 micro SD, memory for Raspberry Pi |
|**A10**| 1        | UGREEN        | B0773N757H             | 6.99        | [Z][a10amzn]           | USB-A to micro-USB cable, 1.5ft |
|**A11**| 1        | CERRXIAN      | B01N5PHSJE             | 6.95        | [Z][a11amzn]           | USB-A to mini-USB cable, 9", right angle |
|**A12**| 4        | DigiKey       | M2117-2545-AL          | 1.09        | [K][a12key]            | M2.5 standoff, 22mm |
|**A13**| 4        | McMaster-Carr | 93475A196              | 0.02        | [C][a13mcmc]           | M2.5 washer, 6mm OD |
|**A14**| 4        | McMaster-Carr | 92095A458              | 0.05        | [C][a14mcmc]           | M2.5 screw, 6mm |
|**A15**| 2        | RespiraWorks  |[Stepper pigtail][a15rw]| 3.00        | Rw                     | Stepper driver pigtail |

**Total assembly price:** USD 513.17

[a1rw]:    ../../../../pcb/rev1_export
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
[a10amzn]: https://www.amazon.com/UGREEN-Braided-Charger-Charging-Controller/dp/B01MTXZ3U8
[a11amzn]: https://www.amazon.com/CERRXIAN-9Inch-Cable-Charge-2-Pack/dp/B01N5PHSJE
[a12key]:  https://www.digikey.com/en/products/detail/raf-electronic-hardware/M2117-2545-AL/9836050
[a13mcmc]: https://www.mcmaster.com/93475A196/
[a14mcmc]: https://www.mcmaster.com/92095A458/
[a15rw]:   ../../../wiring/README.md#stepper-wiring

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../../methods).**

### Mainboard (PCB)

- The mainboard provides the electrical control of the ventilator.  It is a custom-designed part that is most commonly fabricated by a PCB contract manufacturer.  It uses standard PCB manufacturing processes that can be handled by a wide range of vendors.

- Design and manufacturing files for the main board can be found on the [PCB Rev1 page](../../../../pcb/rev1_export).

- Revision 1 of the custom circuit board (PCB) requires some modification to work with the current prototype.

- Please read the errata on the page linked above and follow the instructions to make the necessary modifications, then proceed.

![](images/pcb_assembly_level1.jpg)

### Cycle controller

- The Nucleo dev board serves as the main real-time cycle controller and is to be installed onto the PCB.

- Plug the Nucleo into the PCB, observing the silkscreen (white outline on the board) for correct orientation.

- Move the JP5 jumper on the Nucleo board to the E5V position. This tells the board to expect external power from the PCB and allows the controller to be programmed in-situ.

| Cycle Controller | Setting JP5 |
|:-------------------------:|:-------------------------:|
| ![](images/pcb_assembly_level2.jpg) | ![](images/nucleo_jp5.jpg) |

### Interim board (optional)

The interim board `[A2]` is optional and not an official part of Ventilator v0.3.  However, you might want to install it into this next position in the stack if you intend to test any of the following functionality.  These features will be included on the next revision of the PCB.  If you are not running any such experiments, skip this step and stack the next board directy on top of the Cycle Controller.
* oxygen control
* EEPROM nonvolatile memory
* real-time clock

- Design and manufacturing files can be found in this [separate repository](https://github.com/inceptionev/VentilatorRev1InterimDaughtercard).

- Insert the interim controller into the sockets on the top of the Cycle Controller.  The pins will only allow one orientation of the board, so be mindful of the pin arrangement and the orientation will be correct.  Add the CR2016 battery `[A3]` for the Real-Time Clock (RTC).

![](images/pcb_assembly_level3.jpg)

### Stepper drivers

- Pinch valves require either 2 stepper driver boards `[A5]` or a single 2-axis board `[A6]`.

- Each pinch valve must also include some additional wiring and a driver board for controlling the stepper motor.
  Each stepper driver must have resistors soldered appropriately to allow them to be controlled independently.

- Before you begin, you should first manufacture the 2 pigtails of appropriate length as described on the
  [wiring page](../../../wiring).

- Stepper board resistor modification and mounting to Nucleo is documented [here](stepper_drivers).

- Remember to label the pigtails "top" and "bottom" depending on which stepper driver they are connected to.

- When completed, the stack will look like this:

![](images/pcb_assembly_level4and5.jpg)

### Mounting the PCB stack

- Now that the stack is mostly assembled, you should mount it to the back of the touch-screen on the
display panel assembly, which will already have 9mm standoffs installed to accept the PCB stack.

- Firstly, plug the microUSB cable `[A10]` into the touchscreen's `5V+Touch`. This may be harder to reach later on.

- To mount the PCB stack, align the 4 holes in the left-hand side of the mainboard with the standoffs in the display panel.

- Attach the mainboard to the display by screwing the 22mm stand-offs `[A12]` into the matching display standoffs, using 4x washers `[A13]` to protect the PCB from mechanical damage.

- The whole stack should now look like this:

![](images/pcb_no_pi.jpg)

**NOTE:** The CAD model uses 20mm standoffs, because exact models were not freely available.

### Raspberry Pi

The Raspberry Pi serves as the user interface computer for the ventilator.

Prior to installing it on the PCB, you should:
* install a heat sink `[A8]` on the processor

* load the memory card `[A9]` with the standard [RaspberryPi OS](https://www.raspberrypi.org/software/), using a microSD adapter (listed in [peripherals](../../../peripherals.md))

* install memory card into the RaspberryPi

* give it a keyboard and mouse (also listed in [peripherals](../../../peripherals.md)) or

* alternatively, you can configure it for remote ssh control as described on the [GUI page](../../../../software/gui).

| Heat sinks | SD card |
|:-------------------------:|:-------------------------:|
| ![](images/pi1.jpg) | ![](images/pi2.jpg) |

Now you can mount the Rasberry Pi onto the 20mm stand-offs on the PCB and secure them using the 6mm screws `[A14]`.

### Final wiring

Now you can make the final connections:

* RaspberryPi's `USB` to touchscreen's `5V+Touch` microUSB, using `[A10]`

* RaspberryPi's `microHDMI` to touchscreen's ribbon cable

* Use the miniUSB cable `[A11]` to connect the Nucleo to the Raspberry Pi

* It should now look like this:
![](../images/everything.jpg)
