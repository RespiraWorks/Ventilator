# RespiraWorks Ventilator Mainboard Rev 1.0

## Important Rev 1.0 PCB Errata
* [#63](https://github.com/RespiraWorks/pcbreathe/issues/63): DNP resistors populated on board, typing 3.3V to the 5V rail (bad) - if you have SN 01, 02, 03, or 05 - don't continue using the board without executing the fix in this issue ticket.  See ticket for rework instructions and photos.  All other serial number boards are shipping with this issue fixed.
* [#64](https://github.com/RespiraWorks/pcbreathe/issues/64): Yellow and Green alarm LEDs don't work due to a pin use conflict on the nucleo
* [#56](https://github.com/RespiraWorks/pcbreathe/issues/56): Blower command is 5V but nucleo outputs 3.3V.  All boards are currently shipping with a level translator fitted which addresses this problem, however the blower will turn on full blast briefly at reset ([#62](https://github.com/RespiraWorks/pcbreathe/issues/62) - to be fixed in next rev)
* [#66](https://github.com/RespiraWorks/pcbreathe/issues/66) I2C sensor connectors labeled A, C, B, D. Should be A, B, C, D :facepalm:  To be clear, what this means is, the I2C multiplexer assignments, as labeled on the board in Rev 1.0 are: MUX0: SENSOR A; MUX1: SENSOR C; MUX2: SENSOR B, MUX3: SENSOR D 

## Rev 1.0 PCB Quick Start Guide (visual guide pictures to be uploaded soon)
* If you have SN 01, 02, 03, or 05, make sure you check the errata above before you proceed or you may damange the Rpi.
* Verify that you have an STM32-L452RE module (not an STM32-L452RE-P module, they have different pinouts - see #51)

![verify model](/ReadmePhotos/IMG_9305.jpg)
* Plug the Nucleo into the PCB, observing the silkscreen (white outline on the board) for correct orientation.

![plug in the nucleo](/ReadmePhotos/IMG_9306.jpg)
* Move the JP5 jumper in the upper-middle-center of the Nucleo board to the E5V position.  This tells the board to expect external power from the PCB.  This will avoid programming problems.  If you wish to remove the Nucleo board and work with it on its own without the PCB, move the jumper back to the U5V position to power it from USB.

![plug in the nucleo](/ReadmePhotos/IMG_9289.jpg)
* If you have a X-NUCLEO-IHM03A1 or X-NUCLEO-IHM02A1 stepper driver accesory, plug that in on top of the Nucleo.  The long gold pins of this module can get bent so make sure they are straight and they all go into their respective sockets successfully.  The stepper driver green connectors go in the direction of the white connectors on the mainboard PCB.  For setting up multiple stepper boards and/or connecting actual steppers, see "Setting Up To Drive Steppers" lower down in this readme.

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
* The bringup code was written in the Arduino IDE environment with STM32duino installed and can be found here along with basic instructions for setting up STM32dino and programming the STM32. (disclaimer: this code is not part of the project and is pretty much unsupported, use at your own risk.  The listing of hardware pin #defines at the top might be useful): [pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup) 
    * for more test code with a state machine for running a ventilator setup in closed-loop control mode, see: [FMLtest] (https://github.com/inceptionev/FMLtest)
* The main reference for pin and GPIO assignments on this board are in the [Electrical Hardware Requirements and PCB Interface Control Document](https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit?usp=sharing)
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

## Setting up to drive stepper motors
When the PCB was originally designed, the ability to add a stepper driver was essentially a hedge, a backup *just in case* we wanted to try something that required a stepper motor, so if some of this looks hackey, that's why.  Now that the stepper-driven valve concept is moving towards the main design, we need to support it.  This is the beginning of a guide to using stepper motors with the PCB, which will be completed in the following few days.

* This will assume some knowledge and practice with electrical tinkering.  
* For bakground on stepper motors in general, the always-excellent Great Scott provides a good [intro to stepper motors](https://youtu.be/bkqoKWP4Oy4)
* For information on the powerSTEP01, the specific driver for X-NUCLEO-IHM03A1 we are using in Rev 1.0 of the PCB, [there is a video here](https://youtu.be/_Arx5CMr_mk).  Keep in mind that while this driver will stay for the duration of the Beta phase, we may move to something different in the final product, as powerSTEP01 is likely to be **way** overkill for this application, but it does offer a lot of convenient features.
* We will need to solder some wires to power the stepper driver as it takes a power input for the motor that is separate from the logic power it gets from the Nucleo below. We will piggyback off the blower driver power socket.  Solder two 13cm wires of 22awg or thicker to the backside of J10, the blower power connector. The square pin pad is negative. 

![solder power connection](/ReadmePhotos/IMG_9372.jpg)
* If you are using the X-NUCLEO-IHM02A1, before going forward, you have to change the board configuration to make it compatible with the PCB setup, using your trusty soldering iron:
    * Move the 0 Ohm resistor from SB34 to SB12 to connect PA5 to the clock
    * Move the 0 Ohm resistor from SB23 to SB8 to connect PB6 to CS
![X-NUCLEO-IHM02A1](/ReadmePhotos/x-nucleo-ihm02a1.jpeg)

* While we could put the wires of the stepper directly into the terminal block on the driver, making a little doodad like this will make it quick and easy to swap out different steppers that use this common 4-pin dupont 0.100" pitch connector.

![terminal block wires](/ReadmePhotos/IMG_9371.jpg)

* Insert the wires in to the terminal block and connect them like so (note the example below is for the X-NUCLEO-IHM03A1, IHM02A1 is slightly different).  Screw them down to ensure a secure connection.

![screw down wires](/ReadmePhotos/IMG_9373.jpg)
* Connect the stepper motor to this connector. 

![connect stepper](/ReadmePhotos/IMG_9374.jpg)
* Note pin naming conventions and color codes vary from manufacturer to manufacturer.   
    * If it does nothing and draws no current, swap one wire from the A pair with one wire from the B pair.  
    * If you send a CW command and it turns CCW, then swap the polarity of either the A or B pair, but not both.
* Set the correct chip select for the powerSTEP01 (see [Hardware ICD](https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit?usp=sharing) for pin assignments)
* Send SPI commands to the powerSTEP01 from the NUCLEO to setup the motor settings. [sample code: pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup)
* Send SPI commands to the powerSTEP01 to command motions. [sample code: pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup)
* Once you get one stepper motor working, and you are ready to use multiple stepper drivers, for X-NUCLEO-IHM02A1, you only have to connect the second stepper motor, but for the X-NUCLEO-IHM02A1, there are some more steps:
    * Consult the [X-NUCLEO-IHM03A1 user manual](https://www.st.com/resource/en/user_manual/dm00206777-getting-started-with-the-high-power-stepper-motor-driver-expansion-board-based-on-powerstep01-for-stm32-nucleo-stmicroelectronics.pdf) for information about how to stack the boards.  A brief overview of the process follows, but read the manual first.
    * Make the same motor connector doodad for the second board and install it.  Also prepare two short red and black power wires to piggy-back the motor power input.
    * You will need to move some resistors.  The manual link above will list which ones.
    
    * BOTTOM BOARD IN THE STACK (of two-board stack):
    
    ![bottom board](/ReadmePhotos/IMG_9493.jpg)
    
    * TOP BOARD IN THE STACK (of two-board stack):
    
    ![top board](/ReadmePhotos/IMG_9494.jpg)
    
    * Put the two short power wires into the bottom board's power inputs along with the existing wires and screw then down.
    * Stack the boards, making sure that the long pins go into all their intended sockets.  They tend to get bent and might need some straightening.
    * Screw the two power wires into the upper board.
    
    ![power stack](/ReadmePhotos/IMG_9497.jpg)
    
    * You're done!  Plug in your motors and get moving!
    
    ![dual motors moving](/ReadmePhotos/IMG_9505.GIF)
    

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
