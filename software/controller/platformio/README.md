# Platformio configuration

The [build_config](build_config) directory contains the build configuration.

What follows is the configuration for the deployment scripts.

## Deployment script

By default, controller software and integration tests will deploy to any one STM32 that is hooked up to your machine.

The [deploy.sh](deploy.sh) script provides a convenient way to deploy to multiple connected STM devices,
addressing them by user-defined aliases. In order for this to work, you need to modify the Platformio's STLink script,
find out the serial number of your Nucleo and add it to the list of aliases.

The script provides its own help on syntax.

The script is also used by integration test tools, so it is a good idea to configure it correctly.

The script is further abstracted by the [../upload.sh](../upload.sh) script, which will deploy the default release
firmware to the controller with simplified syntax.

## Modifying STLink script

For the above script to work, a file in your local platformio configuration found here:
`$HOME/.platformio/packages/tool-openocd/scripts/interface/stlink.cfg`
must be replaced with the [`stlink.cfg`](stlink.cfg) provided here.

This solution is based on a post on the platformio forums
[here](https://community.platformio.org/t/choosing-stlink-v2-programmer/10716).

The post is rather old, so the TCL syntax had to be modified to make it work.

## Finding your Nucleo's serial number

To deploy to a particular STM32 boards connected to your system, first find out the serial numbers of your connected Nucleos.

On Unix machines, running something like `udevadm info /dev/ttyACM* | grep SERIAL_SHORT` should produce:
```
E: ID_SERIAL_SHORT=0668FF303435554157105440
E: ID_SERIAL_SHORT=066FFF303435554157105014
E: ID_SERIAL_SHORT=066BFF303435554157104916
```

On Windows, you can use the STLink utility provided by ST Micro. The serial number shows up in the log when you connect.
Possibly you might need to use this to update the firmware too. Close the utility after you have what you need.

With the STLink script modified, you can now deploy firmware like this:
```
SERIAL=066BFF303435554157104916 TEST=TEST_IDLE pio run -e integration-test -t upload
```

However, this can get tedious, so we recommend...

## Making an alias

The [device_lookup_table.txt](device_lookup_table.txt) file contains a list of aliases.

The list will look something like this:
```
ALIAS  NUCLEO_SERIAL                COMMENT
p1     0668FF303435554157122853     Pizza #1 at Edwin's Workshop
p3     0668FF303435554157105440     Pizza #3 at Martin's farm
```

For the first column, make a unique label, hopefully similar to whatever designation has been agreed to by the team.
The second column is the serial number obtained by the magic above, and lastly you can have some nice description.

**PLEASE COMMIT CHANGES** after adding your Nucleo to the list. This will help keep things consistently documented as
we test software on various prototypes.
