# Ventilator GUI

Here resides the code for the ventilator graphical user interface.

## QtCreator

We're using QtCreator to create the qml layout file (main.qml) that is used to
generate the graphical user interface. Note that any ressources (E.g. Images,
other qml files) need to be referenced in the qml.qrc file.

## Installing Qt Dependencies

To install the necessary Qt dependencies, use:

```
sudo apt install build-essential qml-module-qtquick-controls2 qml-module-qtquick-layouts qtdeclarative5-dev qtquickcontrols2-5-dev libqt5quick* libqt5charts5-dev qt5-qmake qml-module-qtquick2 qt5-default qml-module-qtcharts
```

**Please note** that on the Rpi, we are currently using version 5.11.3 of Qt.

## Installing Qtcreator

To install Qtcreator, use:
```
sudo apt-get install qtcreator
```

## Building

To build the GUI code, move the the gui directory and type:
```
./build.sh
```

Upon a successful compilation, the executable file is accessible as:
```
./build/ProjectVentilatorGUI
```

If you are running the application remotely, via ssh for example, don't forget to set
the display environment variable:
```
export DISPLAY=:0
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
