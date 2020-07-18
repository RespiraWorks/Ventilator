# Relay code

If not installed globally, you might need to:
```shell script
pip install pyserial
```

To get a list of available ports, you can do:
```shell script
python3 -m serial.tools.list_ports
```

Currently the code does not work.

## Current setup

![Current setup](setup.jpg)

Relay is hooked up to ttyUSB0. The power line runs to an incandescen bulb.
The bulb has been confirmed to work.
Also, the relay briefly switched it on when powering up.
