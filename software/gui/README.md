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

If you are not actively working on the code for this application but simply need it for prototype testing, you will probably want to use the [automated deployment scripts](../utils/rpi_config).

## Building the GUI

You may want to build and run the GUI application natively, either on your primary machine or on the *RPi*. Most recently, we have been building and testing on the [Ubuntu MATE](https://ubuntu-mate.org/download/arm64/focal/) distribution, more specifically `20.04.3 Focal Fossa` release.

The [gui.sh](gui.sh) script does almost everything you need:
* installs dependencies
* builds the app
* runs the app
* runs unit tests

The script should work for most debian systems using `apt` as the package manager. It should also work on macOS. For a comprehensive list of commands and options you can always run:

```
./gui.sh help
```

Here is a quick summary of some of those options (which may not be up to date).

To install dependencies, run:
```
./gui.sh install
```

To build the application, run:

```
./gui.sh build
```

You can run this latest step with `-j` option to make it faster, but when compiling on *RPi* you may run out of virtual memory.

You can also compile with `--debug` option. You will see additional information on the screen, and logs will be more detailed (`debug` and `trace` level messages will be written), and will output to both console and log file.

### Mac OS X

You may also build and run on MacOS, but you may have to install some dependencies manually.
This section may be out of date.

Go to https://www.qt.io/download-open-source and download the Qt Online Installer.
When asked to select a version, check "Archive", click "Filter", and select 5.15.2.
Unselect all modules except core QT and Charts.

After installation completes, add `qmake` to your PATH, e.g.:

```
export PATH=/Users/$USER/Qt/5.15.2/clang_64/bin:$PATH
```

### Installing Qt Creator (optional)

It is possible to develop the GUI without using Qt Creator, but it is convenient to use Creator for editing the qml layout files.

Note that any resources (e.g. images, all qml files) need to be referenced in the qml.qrc file.

To install Qt Creator on Linux, use:
```
sudo apt-get install qtcreator
```

### Running

Upon a successful compilation, you can run the app like so:

```
./gui.sh run --serial-port /dev/ttyS0
```

Where you may specify whatever serial port the ventilator controller is for the GUI to communicate with. The above has been the correct port for all prototypes tested to date.

If you do not specify a serial-port, the app will run with pre-recorded data.

If you are running the application remotely, via ssh for example, don't forget to set the display environment variable:
```
export DISPLAY=:0
```

### Testing

To run unit tests, type:

```
./gui.sh test
```

### Logs & debugging

Timestamped logs written by the app should be found locally at:

```
$HOME/.local/share/RespiraWorks/VentilatorUI/gui.log
```

Upon start, the application will log the git hash of the commit it was built from.

If app was built in release mode, messages down to `info` level should be seen. In debug mode, `debug` as well as `trace` level messages will also be written to log.

If you want to add additional debug messages, refer to [logger.h](src/logger.h). This is a thread-safe logger. Log macros accept [fmt](https://github.com/fmtlib/fmt) syntax.

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
