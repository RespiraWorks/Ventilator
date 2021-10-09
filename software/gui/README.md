# Ventilator GUI

![In-Action](in-action.png)

Here resides the code for the ventilator graphical user interface. It is based on QT and runs
on Raspberry Pi (henceforth "*RPi*").

Please also see the [Software design pages](../design/GUI_architecture.md) for more information on GUI architecture.

## Requirements

For development purposes you may build and run it locally on a Linux or macOS machine of your choice.

To deploy it as intended for the ventilator you will need:
* Rasberry Pi 4
* SD card
* Card reader USB

A more complete list of hardware components is on the
[Electrical system assembly page](../../manufacturing/internals/display_panel/electronics).

### SSH access to *RPi*

Username: `pi` Password: `respiraworks`

To access the *RPi* remotely, it must be connected to your network. You may connect it physically
with an Ethernet cable. If that is not an option, you must provide it with your WiFi credentials.

You may add those credentials to the *RPi* configuration by directly editing it on the SD card, as
described [here](https://raspberrypi.stackexchange.com/questions/66949/enable-ssh-and-connect-to-a-wifi-network-without-a-keyboard-or-a-screen).

Alternatively, if you have a keyboard that you can hook up to the *RPi*, you may escape the full screen
mode and switch to terminal with `Ctrl+Alt+F1`, and then run `sudo raspi-config`. You may have to reboot
for this toi take effect.

To find out the *RPi*'s IP address, run `ifconfig`.

Now you may ssh into the *RPi* from another machine by running `ssh pi@IP_ADDRESS`

If you have any trouble with serial communications, you may also double on check your
*RPi* configuration with `sudo raspi-config`.

## Building the GUI

You may want to build and run the GUI application natively, either on your primary machine
or on the *RPi*. Building natively on *RPi* assumes a vanilla OS image from
the [Raspberry Pi website](https://www.raspberrypi.org/) and **NOT** the RespiraWorks-generated
image referred to in the sections above.

The [gui.sh](gui.sh) script does almost everything you need:
* installs dependencies
* builds the app
* runs the app
* runs unit tests

The script should work for most debian systems using `apt` as the package manager. It should also work on macOS. For
a comprehensive list of commands and options you can always run:

```
./gui.sh --help
```

Here is a quick summary of some of those options (which may not be up to date).

To install dependencies, run:
```
./gui.sh --install
```

To build the application, run:

```
./gui.sh --build
```

You can run this latest step with `-j` option to make it faster, but when compiling on *RPi* you may run out of virtual
memory.

You can also compile with `--debug` option. You will see additional information on the screen, and logs
will be more detailed (`debug` and `trace` level messages will be written), and will output to console
in addition to file.

### Mac OS X

You may also build and run on MacOS, but you may have to install some dependencies manually.
This section may be out of date.

Go to https://www.qt.io/download-open-source and download the Qt Online Installer.
When asked to select a version, check "Archive", click "Filter", and select 5.11.3.
Unselect all modules except core QT and Charts.

After installation completes, add `qmake` to your PATH, e.g.:

**TODO:** update these instructions to use 5.11.3

```
export PATH=/Users/$USER/Qt/5.11.3/clang_64/bin:$PATH
```

### Installing Qt Creator (optional)

It is possible to develop the GUI without using Qt Creator, but it is
convenient to use Creator for editing the qml layout files.

Note that any resources (e.g. images, all qml files) need to be referenced in the qml.qrc file.

To install Qt Creator on Linux, use:
```
sudo apt-get install qtcreator
```

### Running

Upon a successful compilation, the executable file is accessible on Linux as:

```
./gui.sh --run --serial-port /dev/ttyS0
```

Where you may specify whatever serial port the ventilator controller is for the GUI to communicate
with. The above has been the correct port for all prototypes tested to date.

Depending on the OS you are running the above port could also be `/dev/ttyAMA0`.

If you do not specify a serial-port, the app will run with pre-recorded data.

If you are running the application remotely, via ssh for example, don't forget to set
the display environment variable:
```
export DISPLAY=:0
```

### Testing

To run unit tests, type:

```
./gui.sh --test
```

### Logs & debugging

Timestamped logs written by the app should be found locally at:

```
$HOME/.local/share/RespiraWorks/VentilatorUI/gui.log
```

Upon start, the application will log the git hash of the commit it was built from.

If app was built in release mode, messages down to `info` level should be seen. In debug mode, `debug` as well as
`trace` level messages will also be written to log.

If you want to add additional debug messages, refer to [logger.h](src/logger.h). This is a thread-safe logger.
Log macros accept [fmt](https://github.com/fmtlib/fmt) syntax.

### Screen resolution issues

The qml layout has been designed for a resolution of 1024x600, in case of the display
defaulting to some other setting, this messes up the GUI layout. If this randomly
happens to you (it did to me), add the following to the /boot/config.txt file on your *RPi*.
```
hdmi_cvt=1024 600 60 3 0 0 0

# uncomment to force a specific HDMI mode (this will force VGA)
hdmi_group=2
hdmi_mode=87
```
