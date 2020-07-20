# Modified stlink script for Platformio

In order to deploy to multiple controllers from the same server, we have to be able to tell Platformio on which serial
device to flash the STM32. The default stlink scripts do not provide for such a parameter. Furthermore, the only
thing that differentiates identical Nucleo boards is their Serial Number.

For the testing setup, a script in your local platformio configuration found here:
`~/.platformio/packages/tool-openocd/scripts/interface/stlink.cfg`
must be replaced with the [`stlink.cfg`](stlink.cfg) provided here.

This solution is based on a post on the platformio forums
[here](https://community.platformio.org/t/choosing-stlink-v2-programmer/10716).

The post is rather old, so the TCL syntax had to be modified to make it work.

## Using modified script

To deploy to a particular STM32 connected to your system, first find out the serial numbers of your connected Nucleos
by running something like `udevadm info /dev/ttyACM* | grep SERIAL_SHORT`

In the case of our BigCheese server, we have the following:
```
E: ID_SERIAL_SHORT=0668FF303435554157105440
E: ID_SERIAL_SHORT=066FFF303435554157105014
E: ID_SERIAL_SHORT=066BFF303435554157104916
```

Now, you can deploy controller to a particular device by doing, for example:

```
SERIAL=066BFF303435554157104916 TEST=TEST_IDLE pio run -e integration-test -t upload
```

Or, you can modify the [deploy_test.sh](../deploy_test.sh) script to include an option for your specific unit.
