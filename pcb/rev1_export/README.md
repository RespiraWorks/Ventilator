# RespiraWorks Ventilator Mainboard Rev 1.0 

This directory contains the PCB design files exported from Altium and instructions for setting up the PCB for operation. 

The snapshot for this file is **20200424v2-RELEASE-CANDIDATE-2**.

Previous versions of this iteration are found in the repository up through
[commit 2513e5d](https://github.com/RespiraWorks/Ventilator/commit/2513e5da6fe3e8d02f5f1c38cb00bee6b4fabf3e).

## Contents:

The complete turn-key manufacturing files for Rev 1.0 are provided here. This package includes PCB artwork files, NC 
drill data, fabrication and assembly drawings, Bill of Materials, and machine-assembly centroid (pick-and-place) files.
Any standard PCB fabrication and assembly vendor will be able to manufacture these boards using the provided files.

[Schematic](20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-DWG-SCH.PDF)

[3D STEP file](20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-3D-STEP.step)

[Manufacturing Package](20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-PKG-TURNKEY.zip)

**TODO:** identify the remaining files.

## Requesting hardware for development

If you are a RespiraWorks contributor, request dev hardware by making an entry
[here](https://docs.google.com/spreadsheets/d/1rf-cOTlQL_LnzrZq-64n7_R_pFyFMdCAxsPC65YqKUg/edit?usp=sharing)

## Important Rev 1.0 PCB Errata
* [#739](https://github.com/RespiraWorks/Ventilator/issues/739): DNP resistors populated on board, typing 3.3V to the 5V
  rail (bad) - if you have SN 01, 02, 03, or 05 - don't continue using the board without executing the fix in this issue
  ticket.  See ticket for rework instructions and photos.  All other serial number boards are shipping with this issue
  fixed.
* [#738](https://github.com/RespiraWorks/Ventilator/issues/738): Yellow and Green alarm LEDs don't work due to a pin use
  conflict on the nucleo
* [#745](https://github.com/RespiraWorks/Ventilator/issues/745): Blower command is 5V but nucleo outputs 3.3V.  All
  boards are currently shipping with a level translator fitted which addresses this problem, however the blower will
  turn on full blast briefly at reset ([#740](https://github.com/RespiraWorks/Ventilator/issues/740) - to be fixed in
  next rev)
* [#736](https://github.com/RespiraWorks/Ventilator/issues/736) I2C sensor connectors labeled A, C, B, D. Should be A,
  B, C, D :facepalm:  To be clear, what this means is, the I2C multiplexer assignments, as labeled on the board in Rev
  1.0 are: MUX0: SENSOR A; MUX1: SENSOR C; MUX2: SENSOR B, MUX3: SENSOR D
* [#846](https://github.com/RespiraWorks/Ventilator/issues/846): anti-miller caps C33 and C34 filter out PWM signal and
  prevent proper operation.  The Miller effect is only present at startup and doesn't cause a problem with permanently
  connected loads like the PSOL.  Remove C33 and C34.

## Rev 1.0 PCB Quick Start Guide
* If you have SN 01, 02, 03, or 05, make sure you check the errata above before you proceed or you may damage the Rpi.
* If you intend to use the stepper motor-based pinch valves, you will need to solder some wires to power the stepper
  driver as it takes a power input for the motor that is separate  from the logic power it gets from the Nucleo below.
  We will piggyback off the blower driver power socket.  Solder two 20cm wires of 22awg or thicker to the backside of
  J10, the blower power connector. The square pin pad is negative.

![solder power connection](readme_photos/stepper_power.jpg)
* Verify that you have an STM32-L452RE module (not an STM32-L452RE-P module, they have different pinouts - see #51)

![verify model](readme_photos/IMG_9305.jpg)
* Plug the Nucleo into the PCB, observing the silkscreen (white outline on the board) for correct orientation.

![plug in the nucleo](readme_photos/IMG_9306.jpg)
* Move the JP5 jumper in the upper-middle-center of the Nucleo board to the E5V position.  This tells the board to
  expect external power from the PCB.  This will avoid programming problems.  If you wish to remove the Nucleo board
  and work with it on its own without the PCB, move the jumper back to the U5V position to power it from USB.

![plug in the nucleo](readme_photos/IMG_9289.jpg)
* If you have a X-NUCLEO-IHM03A1 or X-NUCLEO-IHM02A1 stepper driver accesory, plug that in on top of the Nucleo.  The
  long gold pins of this module can get bent so make sure they are straight and they all go into their respective
  sockets successfully.  The stepper driver green connectors go in the direction of the white connectors on the
  mainboard PCB.  For setting up multiple stepper boards and/or connecting actual steppers, see
  [this guide](../../manufacturing/internals/display_panel/electronics/stepper_drivers).

![plug in the stepper driver](readme_photos/IMG_9307.jpg)
* If you are using an Rpi 3B+ or 4 with this PCB, plug this into the RPI socket.  Note that if you have the standoffs
  fitted, do not overtighen them as this can damage the Pi.

![plug in the Rpi](readme_photos/IMG_9308.jpg)
* If you are using Touchscreen w/ Speakers with your Rpi, connect an HDMI cable (included with Touchscreen package)
  between the Pi and the Touchscreen.  Similarly, connect a USB micro cable (included with Touchscreen package) between
  the Pi and the TOUCH+5V connector on the Touchscreen.

![plug in the Touchscreen](readme_photos/IMG_9309.jpg)
* If you are connecting a blower, take the 6-pin to 6-pin JST XH cable and connect it between the BLOWER CTRL connector
  at the top of the PCB with corresponding connector on the blower driver.  Likewise, take the 2-pin to 2-pin JST VH
  cable and connect it between BLOWER POWER connector at the top of the PCB and the corresponding connector on the
  blower driver.

![plug in the nucleo](readme_photos/IMG_9311.jpg)
* The PCB must be powered in order for the Nucleo to program correctly.  A power cable which plugs into the upper left
  corner of the PCB is provided with the board and allows it to be powered from any 12V, 1.2A or greater power adapter
  with a 5.5x2.1mm center-positive barrel jack.  You will want 5A or more if you are also powering solenoids, blowers,
  and heaters from the PCB.  Plug this in to the power the board.  A green POWER light in the upper left of the PCB
  should come on, and the various status lights of the Rpi and Nucleo will probably also come on.

![plug in the nucleo](readme_photos/IMG_9312.jpg)
* When programming over the Nucleo USB interface, the board emulates an ST-Link programming the MCU over SWD.  Use these
  settings in the toolchain of your choice.
* The bringup code was written in the Arduino IDE environment with STM32duino installed and can be found here along with
  basic instructions for setting up STM32dino and programming the STM32. (disclaimer: this code is not part of the
  project and is pretty much unsupported, use at your own risk.  The listing of hardware pin #defines at the top might
  be useful): [pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup)
    * for more test code with a state machine for running a ventilator setup in closed-loop control mode,
      see this repo: [FMLtest](https://github.com/inceptionev/FMLtest)
* The main reference for pin and GPIO assignments on this board are in the
  [Electrical Hardware Requirements and PCB Interface Control Document](https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit?usp=sharing)
* The large power connectors on the PCB are JST VH type.
* The smaller peripheral connectors on the PCB are JST XH type.
* The buzzer requires an oscillating signal to make sound, it is designed for peak output at 2.4kHz.  Take off the
  kapton cover at your own hearing risk.
* On the MPXV5004DP differential pressure sensors, the upper port is the positive measurement, and the lower port is the
  negative.  The transfer function, assuming a 12-bit ADC, is P[kpa] = ADCcounts/819 - 1
* The switched heater and solenoid outputs can support a resistive or inductive load up to 2A.  Refer to schematic for
  pinouts.
* The i2c sensor connectors are initially configured for providing 3.3V supply power to the sensors, but can be
  configured for 5V.  Each sensor can have its own voltage; they do not need to be the same.  If you need to switch to
  5V, see the schematic for the resistors to change.  Refer to schematic for pinouts.
* The analog pressure sensors have an anti-aliasing filter value of 100Hz.

## Stepper driver setup

When the PCB was originally designed, the ability to add a stepper driver was essentially a hedge, a backup *just in
case* we wanted to try something that required a stepper motor.

To use stepper motors, you must add stepper driver boards as described in
[this guide](../../manufacturing/internals/display_panel/electronics/stepper_drivers).

## System integration

The PCB and its daughter-boards are part of the
[electronics sub-assembly](../../manufacturing/internals/display_panel/electronics).

