# RespiraWorks Ventilator Mainboard Rev 1.0

## Important Rev 1.0 PCB Errata
* [#63](https://github.com/RespiraWorks/pcbreathe/issues/63): DNP resistors populated on board, typing 3.3V to the 5V rail (bad) - if you have SN 01, 02, 03, or 05 - don't continue using the board without executing the fix in this issue ticket.  See ticket for rework instructions and photos.  All other serial number boards are shipping with this issue fixed.
* [#64](https://github.com/RespiraWorks/pcbreathe/issues/64): Yellow and Green alarm LEDs don't work due to a pin use conflict on the nucleo
* [#56](https://github.com/RespiraWorks/pcbreathe/issues/56): Blower command is 5V but nucleo outputs 3.3V.  All boards are currently shipping with a level translator fitted which addresses this problem, however the blower will turn on full blast briefly at reset ([#62](https://github.com/RespiraWorks/pcbreathe/issues/62) - to be fixed in next rev)

## Rev 1.0 PCB Quick Start Guide (visual guide pictures to be uploaded soon)
* If you have SN 01, 02, 03, or 05, make sure you check the errata above before you proceed or you may damange the Rpi.
* Verify that you have an STM32-L452RE module (not an STM32-L452RE-P module, they have different pinouts - see #51)

![verify model](/ReadmePhotos/IMG_9305.jpg)
* Plug the Nucleo into the PCB, observing the silkscreen (white outline on the board) for correct orientation.

![plug in the nucleo](/ReadmePhotos/IMG_9306.jpg)
* Move the JP5 jumper in the upper-middle-center of the Nucleo board to the E5V position.  This tells the board to expect external power from the PCB.  This will avoid programming problems.  If you wish to remove the Nucleo board and work with it on its own without the PCB, move the jumper back to the U5V position to power it from USB.

![plug in the nucleo](/ReadmePhotos/IMG_9289.jpg)
* If you have a X-NUCLEO-IHM03A1 stepper driver accesory, plug that in on top of the Nucleo.  The long gold pins of this module can get bent so make sure they are straight and they all go into their respective sockets successfully.  The stepper driver green connectors go in the direction of the white connectors on the mainboard PCB.

![plug in the stepper driver](/ReadmePhotos/IMG_9307.jpg)
* If you are using an Rpi 3B+ or 4 with this PCB, plug this into the RPI socket.  Note that if you have the standoffs fitted, do not overtighen them as this can damage the Pi.

![plug in the Rpi](/ReadmePhotos/IMG_9308.jpg)
* If you are using Touchscreen w/ Speakers with your Rpi, connect an HDMI cable (included with Touchscreen package) between the Pi and the Touchscreen.  Similarly, connect a USB micro cable (included with Touchscreen package) between the Pi and the TOUCH+5V connector on the Touchscreen.

![plug in the Touchscreen](/ReadmePhotos/IMG_9309.jpg)
* If you are connecting a blower, take the 6-pin to 6-pin JST XH cable and connect it between the BLOWER CTRL connector at the top of the PCB with corresponding connector on the blower driver.  Likewise, take the 2-pin to 2-pin JST VH cable and connect it between BLOWER POWER connector at the top of the PCB and the corresponding connector on the blower driver.

![plug in the nucleo](/ReadmePhotos/IMG_9311.jpg)
* The PCB must be powered in order for the Nucleo to program correctly.  A power cable which plugs into the upper left corner of the PCB is provided with the board and allows it to be powered from any 12V, 1.2A or greater power adapter with a 5.5x2.1mm center-positive barrel jack.  You will want 5A or more if you are also powering solenoids, blowers, and heaters from the PCB.  Plug this in to the power the board.  A green POWER light in the upper left of the PCB should come on, and the various status lights of the Rpi and Nucleo will probably also come on.

![plug in the nucleo](/ReadmePhotos/IMG_9312.jpg)
* When programming over the Nucleo USB interface, the board emulates an ST-Link programming the MCU over SWD.  Use these settings in the toolchain of your choice.
* The bringup code was written in the Arduino IDE environment with STM32duino installed and can be found here (disclaimer: this code is not part of the project and is pretty much unsupported, use at your own risk.  The listing of hardware pin #defines at the top might be useful): [pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup) 
* The large power connectors on the PCB are JST VH type.  [Purchase Link](https://www.amazon.com/gp/product/B07LBZ3LCR)
* The smaller peripheral connectors on the PCB are JST XH type. 
  * [Purchase link for 2/3/4/5-pin](https://www.amazon.com/gp/product/B06ZZ45G7G) 
  * [Purchase link for 5/6/7-pin](https://www.amazon.com/gp/product/B079MJ1RYN)
* Here is a crimper that can crimp both types of contacts.  [Purchase Link](https://www.amazon.com/gp/product/B01N4L8QMW)
* The buzzer requires an oscillating signal to make sound, it is designed for peak output at 2.4kHz.  Take off the kapton cover at your own hearing risk.
* On the MPXV5004DP differential pressure sensors, the upper port is the positive measurement, and the lower port is the negative.  The transfer function, assuming a 12-bit ADC, is P[kpa] = ADCcounts/819 - 1
* The switched heater and solenoid outputs can support a resistive or inductive load up to 2A.  Refer to schematic for pinouts.
* The i2c sensor connectors are initially configured for providing 3.3V supply power to the sensors, but can be configured for 5V.  Each sensor can have its own voltage; they do not need to be the same.  If you need to switch to 5V, see the schematic for the resistors to change.  Refer to schematic for pinouts.
* The analog pressure sensors have an anti-aliasing filter value of 100Hz.

## Rev 1 As-Built Files:
As-built directory: <br>
https://github.com/RespiraWorks/pcbreathe/tree/master/NIGHTLY/20200424v2-RELEASE-CANDIDATE-2

As-built Schematic: <br>
https://github.com/RespiraWorks/pcbreathe/blob/master/NIGHTLY/20200424v2-RELEASE-CANDIDATE-2/20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-DWG-SCH.PDF

As-built 3D STEP file: <br>
https://github.com/RespiraWorks/pcbreathe/blob/master/NIGHTLY/20200424v2-RELEASE-CANDIDATE-2/20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-3D-STEP.step

Manufacturing Pacakge: <br>
https://github.com/RespiraWorks/pcbreathe/blob/master/NIGHTLY/20200424v2-RELEASE-CANDIDATE-2/20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-PKG-TURNKEY.zip

## Requesting Hardware for Dev:
Request hardware by making an entry here: https://docs.google.com/spreadsheets/d/1rf-cOTlQL_LnzrZq-64n7_R_pFyFMdCAxsPC65YqKUg/edit?usp=sharing
