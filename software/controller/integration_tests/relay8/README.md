# Relay code

The Relay switch is 8-Channel from the SainSmart vendor.
The command line utility sainsmartrelay is available in /usr/local/bin on bigcheese server.
The control Commands of the Relay swtich are:

```shell script
	sainsmartrelay --on 1
	sainsmartrelay --off 1
```

You can use --help option to find out all available options of sainsmartrelay.

Please do NOT use relay8.py for now, currently this script is not working.


## Current setup

![Current setup](setup.jpg)

Relay is hooked up to ttyUSB0. The power line runs to an incandescen bulb.
The bulb has been confirmed to work.
Also, the relay briefly switched it on when powering up.
