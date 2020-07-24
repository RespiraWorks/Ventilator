# Ventilator GUI

![In-Action](in-action.png)

Here resides the code for the ventilator graphical user interface. It is based on QT and runs
on Raspberry Pi (henceforth "*RPi*").

Please also see the [Software design pages](../design) for more information on GUI architecture.

## Building and deploying

For development purposes you may build and run it locally on a Linux or macOS machine of your choice.

To deploy it as intended for the ventilator you will need:
* Rasberry Pi 4
* SD card
* Card reader USB

A more complete list of hardware components is on
[Brain build page](../../manufacturing/alpha-build-instructions/pizza-brain).

There are two ways to run the GUI on your *RPi* - by building it natively on the *RPi*
or by installing a pre-generated image of the *RPi* OS (henceforth "*RPIOS image*") that
automatically loads the GUI on bootup. The latter is the default usage for the final product.
However, if you are doing any development on the GUI, testing bleeding edge features or just
having problems with the *RPIOS image*, you might want to build and run the GUI manually.

If you are looking for how to build and use the GUI natively, you can jump ahead to the
[going native](#going-native) section below.

## Deploying *RPIOS image*

The pre-generated *RPIOS image*s are available for download here:
[http://ps3.respira.works/rpi-images/](http://ps3.respira.works/rpi-images/).

In a digital case of the Russian doll, the *RPIOS image* actually comes packaged with another image,
which it will run as virtual machine. This Docker image (henceforth "Docker image") contains
the GUI application and all necessary runtime dependencies. Docker images
are generated with every change to the master branch of the code, approximately
within 10 minutes of a merged change.

If you are interested in the details for how this image generation and deployment works,
you can read our plan for [GUI deployment strategy](deployment_strategy.md).

The Docker image contained in the *RPIOS image* is the latest Docker image at the time of building the
*RPIOS image* (which is not that often), so it may not actually contain the latest version of the GUI.
You may have to update the Docker image on the *RPi*. For this reason, and for troubleshooting
other problems, you may want to log into the *RPi* remotely.

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

### Updating Docker image

To update the Docker image on the *RPi*, run the following commands:
```
docker stop gui
docker container rm gui
docker system prune
docker pull respiraworks/gui
sudo shutdown -r now
```

To see logs from GUI running within the Docker image, try this:
```
docker logs gui
```

To find out what version of the GUI app is running, do the following:
```
docker exec -it gui /bin/bash
git log
```

## Going native

You may want to build and run the GUI application natively, either on your primary machine
or on the *RPi*. Building natively on *RPi* assumes a vanilla OS image from
the [Raspberry Pi website](https://www.raspberrypi.org/) and **NOT** the RespiraWorks-generated
image referred to in the sections above.

The [gui.sh](gui.sh) script will both install dependencies and build the application.
The script should work for most debian systems using `apt` as the package manager.

To install dependencies, run:
```
./gui.sh --install
```

To build the application, run:

```
./gui.sh --build
```

When building natively on the *RPi* you may run out of virtual memory. This is because the build
script is trying to hog too many resources. If your build fails for this reason, you should still
have the build configured correctly by the script and you can do:

```
cd build
make
```

### Mac OS X

You may also build and run on MacOS, but you will have to install dependencies manually.
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

### Running natively

Upon a successful compilation, the executable file is accessible on Linux as:
```
./build/app/ProjectVentilatorGUI --serial-port /dev/ttyS0
```
Where you may specify whatever serial port the ventilator controller is for the GUI to communicate
with. The above has been the correct port for all prototypes tested to date.

On Mac OS X, you would run it as as:
```
./build/app/ProjectVentilatorGUI.app/Contents/MacOS/ProjectVentilatorGUI
```

If you are running the application remotely, via ssh for example, don't forget to set
the display environment variable:
```
export DISPLAY=:0
```

### Testing

To run unit tests as a developer, type:

```
./gui.sh --build && ./gui.sh --test
```

**TODO:** add something about manual testing with fake data.

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
