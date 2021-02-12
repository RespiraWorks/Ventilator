# Automatic deployment on Pi

## How to set up

* Prepare an SD card with a vanilla Raspberry Pi OS, the full desktop variety.
* Make sure you have a USB connection from Raspi to Nucleo if you intend to flash firmware locally
* Boot up and go through the standard Raspi configuration steps - locale, Wifi, update
* Reboot

Now, open up a browser, and come back to this page.

Just copy and paste this dangerous script into the console:

```
curl -s https://raw.githubusercontent.com/RespiraWorks/Ventilator/issue_1028_general_deployment_scripts/software/utils/rpi_config/bootstrap.sh | bash
```

Follow the onscreen directions.

When the initial install is complete, the system will reboot again.

You will need to run the "Ventilator update" app that you should find on your desktop for the first time to make the
ventilator functional.

## What you have available

On your desktop you should have:
* Ventilator update -- run this any time to update all firmware and software to the latest version
* Ventilator GUI -- graphical interface for controlling the ventilator
* Vent debug -- opens console with ventilator debug interface and connects to the controller
* Ventilator repo -- opens a browser to the RespiraWorks ventilator github page
