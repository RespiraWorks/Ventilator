# Stepper driver configuration

When the PCB was originally designed, the ability to add a stepper driver was essentially a hedge, a backup *just in
case* we wanted to try something that required a stepper motor, so if some of this looks hacky, that's why.  Now that
the stepper-driven valve concept is moving towards the main design, we need to support it.  This is a guide to using
stepper motors with the Rev1 PCB.

## Background reading

* For background on stepper motors in general, the always-excellent Great Scott provides a good
  [intro to stepper motors](https://youtu.be/bkqoKWP4Oy4)
* For information on the powerSTEP01, the specific driver for X-NUCLEO-IHM03A1 we are using in Rev 1.0 of the PCB,
  [there is a video here](https://youtu.be/_Arx5CMr_mk). Keep in mind that while this driver is only selected  for R&D.
  We may move to something different in the final product, as powerSTEP01 is likely to be way overkill for this
  application, but it does offer a lot of convenient features.

## Configuring driver boards

Your steps will differ slightly depending on which type of driver boards you have.

If you are using the interim board, you will be stacking the driver board(s) on top of that. Else - directly to the
STM32 controller board.

### IHM03A1

Consult the [X-NUCLEO-IHM03A1 user manual](https://www.st.com/resource/en/user_manual/dm00206777-getting-started-with-the-high-power-stepper-motor-driver-expansion-board-based-on-powerstep01-for-stm32-nucleo-stmicroelectronics.pdf)
for information about how to stack the boards.  A brief overview of the process follows, but read the manual first.

You will need to move some resistors.  The manual link above will list which ones. You can also see in the pictures below.

| Bottom board | Top board  |
|:--------:|:--------:|
| ![bottom board](images/bottom_board.jpg) | ![top board](images/top_board.jpg) |


| Install bottom board | Stack top board  |
|:--------:|:--------:|
| Insert the wires of the "bottom" pigtail into the terminal block and connect with this color pattern.  Screw them down to ensure a secure connection. | Stack the boards, making sure that the long pins go into all their intended sockets.  They tend to get bent and might need some straightening.|
| Before you tighten the power wires, put the two short power wires into the bottom board's power inputs along with the existing wires and screw then down.| Screw the two power wires into the upper board, as well as the "TOP" pigtail using the same color pattern. |
| ![screw down wires](images/bottom_wired.jpg) | ![power stack](images/top_wired.jpg) |


### IHM02A1

If you are using the X-NUCLEO-IHM02A1, you have to change the board configuration to make it compatible with the
PCB setup:
* Move the 0 Ohm resistor from SB34 to SB12 to connect PA5 to the clock
* Move the 0 Ohm resistor from SB23 to SB8 to connect PB6 to CS

![X-NUCLEO-IHM02A1](images/x-nucleo-ihm02a1.jpeg)

For X-NUCLEO-IHM02A1, you only have to connect the second stepper motor.

**#TODO: which connector is which, how should they be labeled? @inceptionev**

## Integration notes

* Note that pin naming conventions and color codes vary from manufacturer to manufacturer.
    * If it does nothing and draws no current, swap one wire from the A pair with one wire from the B pair.
    * If you send a CW command and it turns CCW, then swap the polarity of either the A or B pair, but not both.
* Set the correct chip select for the powerSTEP01 (see
  [Hardware ICD](https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit?usp=sharing)
  for pin assignments)
* Send SPI commands to the powerSTEP01 from the NUCLEO to setup the motor settings.
  [sample code: pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup)
* Send SPI commands to the powerSTEP01 to command motions.
  [sample code: pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup)

## In action

![dual motors moving](images/two_steppers_moving.gif)
