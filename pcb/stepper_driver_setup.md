# Setting up to drive stepper motors

When the PCB was originally designed, the ability to add a stepper driver was essentially a hedge, a backup *just in
case* we wanted to try something that required a stepper motor, so if some of this looks hackey, that's why.  Now that
the stepper-driven valve concept is moving towards the main design, we need to support it.  This is the beginning of a
guide to using stepper motors with the PCB, which will be completed in the following few days.

* This will assume some knowledge and practice with electrical tinkering.
* For bakground on stepper motors in general, the always-excellent Great Scott provides a good
  [intro to stepper motors](https://youtu.be/bkqoKWP4Oy4)
* For information on the powerSTEP01, the specific driver for X-NUCLEO-IHM03A1 we are using in Rev 1.0 of the PCB,
  [there is a video here](https://youtu.be/_Arx5CMr_mk).  Keep in mind that while this driver will stay for the duration
  of the Beta phase, we may move to something different in the final product, as powerSTEP01 is likely to be **way**
  overkill for this application, but it does offer a lot of convenient features.
* We will need to solder some wires to power the stepper driver as it takes a power input for the motor that is separate
  from the logic power it gets from the Nucleo below. We will piggyback off the blower driver power socket.  Solder two
  13cm wires of 22awg or thicker to the backside of J10, the blower power connector. The square pin pad is negative.

![solder power connection](readme_photos/stepper_driver_setup/IMG_9372.jpg)
* If you are using the X-NUCLEO-IHM02A1, before going forward, you have to change the board configuration to make it
compatible with the PCB setup, using your trusty soldering iron:
    * Move the 0 Ohm resistor from SB34 to SB12 to connect PA5 to the clock
    * Move the 0 Ohm resistor from SB23 to SB8 to connect PB6 to CS
![X-NUCLEO-IHM02A1](readme_photos/stepper_driver_setup/x-nucleo-ihm02a1.jpeg)

* While we could put the wires of the stepper directly into the terminal block on the driver, making a little doodad
  like this will make it quick and easy to swap out different steppers that use this common 4-pin dupont 0.100" pitch
  connector.

![terminal block wires](readme_photos/stepper_driver_setup/IMG_9371.jpg)

* Insert the wires in to the terminal block and connect them like so (note the example below is for the
  X-NUCLEO-IHM03A1, IHM02A1 is slightly different).  Screw them down to ensure a secure connection.

![screw down wires](readme_photos/stepper_driver_setup/IMG_9373.jpg)
* Connect the stepper motor to this connector.

![connect stepper](readme_photos/stepper_driver_setup/IMG_9374.jpg)
* Note pin naming conventions and color codes vary from manufacturer to manufacturer.
    * If it does nothing and draws no current, swap one wire from the A pair with one wire from the B pair.
    * If you send a CW command and it turns CCW, then swap the polarity of either the A or B pair, but not both.
* Set the correct chip select for the powerSTEP01 (see
  [Hardware ICD](https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit?usp=sharing)
  for pin assignments)
* Send SPI commands to the powerSTEP01 from the NUCLEO to setup the motor settings.
  [sample code: pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup)
* Send SPI commands to the powerSTEP01 to command motions.
  [sample code: pcbreathe-bringup](https://github.com/inceptionev/pcbreathe-bringup)
* Once you get one stepper motor working, and you are ready to use multiple stepper drivers, for X-NUCLEO-IHM02A1, you
  only have to connect the second stepper motor, but for the X-NUCLEO-IHM03A1, there are some more steps:
    * Consult the [X-NUCLEO-IHM03A1 user manual](https://www.st.com/resource/en/user_manual/dm00206777-getting-started-with-the-high-power-stepper-motor-driver-expansion-board-based-on-powerstep01-for-stm32-nucleo-stmicroelectronics.pdf)
      for information about how to stack the boards.  A brief overview of the process follows, but read the manual first.
    * Make the same motor connector doodad for the second board and install it.  Also prepare two short red and black
      power wires to piggy-back the motor power input.
    * You will need to move some resistors.  The manual link above will list which ones.

    * BOTTOM BOARD IN THE STACK (of two-board stack):

    ![bottom board](readme_photos/stepper_driver_setup/IMG_9493.jpg)

    * TOP BOARD IN THE STACK (of two-board stack):

    ![top board](readme_photos/stepper_driver_setup/IMG_9494.jpg)

    * Put the two short power wires into the bottom board's power inputs along with the existing wires and screw then
      down.
    * Stack the boards, making sure that the long pins go into all their intended sockets.  They tend to get bent and
      might need some straightening.
    * Screw the two power wires into the upper board.

    ![power stack](readme_photos/stepper_driver_setup/IMG_9497.jpg)

    * You're done!  Plug in your motors and get moving!

    ![dual motors moving](readme_photos/stepper_driver_setup/IMG_9505.GIF)
