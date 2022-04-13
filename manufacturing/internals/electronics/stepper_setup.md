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
