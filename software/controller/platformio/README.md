# Platformio configuration

The [build_config](build_config) directory contains the build configuration.

What follows is the configuration for the deployment scripts.

## Modifying STLink script

In order to deploy to multiple STM32 devices on the same system, a file in your local platformio configuration found here:
`$HOME/.platformio/packages/tool-openocd/scripts/interface/stlink.cfg`
must be replaced with the [`stlink.cfg`](stlink.cfg) provided here.

This patching is now handled by running `controller.sh patch_ocd`.

This solution is based on a post on the platformio forums
[here](https://community.platformio.org/t/choosing-stlink-v2-programmer/10716).

The post is rather old, so the TCL syntax had to be modified to make it work.

### Finding your Nucleo's serial number

Association of STM32 HLA serial number to prototype alias ID is now managed by the python and bash utility scripts. Use `./controller.sh devices` or use the `device` command in the debug interface for more details. The manifest is maintained on Google Drive in a spreadsheet under `Testing`.

The following are instructions for how to do this manually.

To deploy to a particular STM32 boards connected to your system, first find out the serial numbers of your connected Nucleos.

On Unix machines, running something like `udevadm info /dev/ttyACM* | grep SERIAL_SHORT` should produce:
```
E: ID_SERIAL_SHORT=0668FF303435554157105440
E: ID_SERIAL_SHORT=066FFF303435554157105014
E: ID_SERIAL_SHORT=066BFF303435554157104916
```

On Windows, you can use the STLink utility provided by ST Micro. The serial number shows up in the log when you connect. Possibly you might need to use this to update the firmware too. Close the utility after you have what you need.

With the STLink script modified, you can now deploy firmware like this:
```
SERIAL=066BFF303435554157104916 TEST=TEST_IDLE pio run -e integration-test -t upload
```
