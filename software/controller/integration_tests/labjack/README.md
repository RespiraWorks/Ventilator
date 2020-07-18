# Labjack code

This is code for the [Labjack T7](https://labjack.com/products/t7).

Reusing some Labjack sample Python code from [here](https://labjack.com/support/software/examples/ljm/python).

Using this [datasheet](https://media.ncd.io/sites/2/20170721134812/ams5915-5.pdf) for pressure sensor.

## Current setup

![Current setup](setup.jpg)

Pressure sensor is hooked up to the inflow venturi just downstream of the closed off oxygen inlet.
You can deploy code controller that will drive the blower, and if the i2c code works,
you should be able to observe flow with the sensor.

There is also a DAC line connected to ADC input.
