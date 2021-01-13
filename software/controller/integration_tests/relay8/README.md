# 8-channel relay

The test server has an 8-Channel relay switch from SainSmart. Two of the channels, 1 and 3 are wired to control
two power outlets. Currently, channel 1 has a powerful LED shop light connected to it.

| Relay contraption  | Lighting setup          |
|:------------------:|:-----------------------:|
| ![](relay_rig.jpg) | ![](lighting_setup.jpg) |


## Operating the relay

A command line utility `sainsmartrelay` is available at `/usr/local/bin` on `bigcheese` server.

The control Commands of the Relay switch are:

```shell script
	sainsmartrelay --on 1
	sainsmartrelay --off 1
```

You can use the `--help` option to find out all available options of `sainsmartrelay`.

## Background

The following describes how the above script was made to work.

The relay was purchased from [here](https://www.amazon.com/SainSmart-Eight-Channel-Relay-Automation/dp/B0093Y89DE).

The relay is hooked up to `ttyUSB0`.

Here are instructions to build the `sainsmartrelay` command line utility:

- please see the [labbots/SainsmartUsbRelay](https://github.com/labbots/SainsmartUsbRelay.git) project on github.
- clone the source: `git clone https://github.com/labbots/SainsmartUsbRelay.git`
- install any dependent libraries as listed on the github project page.
- Change the code in following files:
  - in the `makefile`, add `-fPIE` option to `CFLAGS`
  - in `sainsmartrelay.h`, update the value of `MAX_NUM_RELAYS` to `8`
- run `make`
