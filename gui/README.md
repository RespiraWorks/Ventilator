# Ventilator GUI

Here resides the code for the ventilator graphical user interface.

It is based on QT.  **Please note** that on the Rpi, we are currently using version 5.11.3 of Qt.

## Installing Qt Dependencies: Linux

To install the necessary Qt dependencies, use:

```
sudo apt install build-essential qml-module-qtquick-controls2 qml-module-qtquick-layouts qtdeclarative5-dev qtquickcontrols2-5-dev libqt5quick*  libqt5serialport5-dev libqt5serialport5 qt5-qmake qml-module-qtquick2 qt5-default qml-module-qtcharts
```

(TODO update these instructions to use 5.11.3)

## Installing Qt Dependencies: Mac OS X

Go to https://www.qt.io/download-open-source and download the Qt Online Installer. When asked to select a version, check "Archive", click "Filter", and select 5.11.3. Unselect all modules except core QT and Charts.

After installation completes, add `qmake` to your PATH, e.g.:

```
export PATH=/Users/$USER/Qt/5.11.3/clang_64/bin:$PATH
```

## Installing Qt Creator (optional)

It is possible to develop the GUI without using Qt Creator, but it is
convenient to use Creator for editing the qml layout files.

Note that any resources (e.g. images, all qml files) need to be referenced in the qml.qrc file.

To install Qt Creator on Linux, use:
```
sudo apt-get install qtcreator
```

## Building

To build the GUI code, move the the gui directory and type:
```
./gui.sh --build
```

Upon a successful compilation, the executable file is accessible on Linux as:
```
./build/app/ProjectVentilatorGUI
```

and on Mac OS X as:
```
./build/app/ProjectVentilatorGUI.app/Contents/MacOS/ProjectVentilatorGUI
```

If you are running the application remotely, via ssh for example, don't forget to set
the display environment variable:
```
export DISPLAY=:0
```

## Running unit tests

To run unit tests as a developer, type:

```
./gui.sh --build && ./gui.sh --test
```

## Screen resolution issues

The qml layout has been designed for a resolution of 1024x600, in case of the display
defaulting to some other setting, this messes up the GUI layout. If this randomly
happens to you (it did to me), add the following to the /boot/config.txt file on your Rpi.
```
hdmi_cvt=1024 600 60 3 0 0 0

# uncomment to force a specific HDMI mode (this will force VGA)
hdmi_group=2
hdmi_mode=87
```
