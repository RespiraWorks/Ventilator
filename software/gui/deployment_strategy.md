# Overview

The GUI is a C++/Qt application, meaning it is compiled to machine code. We will build native and ARM targets during regular development or when deploying to a Raspberry Pi. We're using Docker to make this easier, because it allows us to develop and deploy using the same code and dependencies without having to worry about compatibility or other issues. This also allows us to do continuous integration in the cloud without having to have a test fleet of Raspberry Pis.

The following is a description of how we intend to implement this deployment strategy. A user or developer will not need to know these details. The result of our implementation will be complete disk images deploy-able straight to a Raspberry Pi using the usual imaging process.

When deployed to a Raspberry Pi, the system will look like this:

* A Raspberry Pi running Raspbian runs the Docker daemon: for now, the daemon will always run a designated image containing the GUI. Eventually, a service will be in place to enable updates. Eventually, we will automate creating this Raspberry Pi image.
* The Docker GUI image: for now, this contains the GUI code, all of its dependencies, and the tools necessary to build the GUI. A new version of this image is built using [CircleCI](https://app.circleci.com/projects/project-dashboard/github/RespiraWorks) whenever a change is made to the code. During CI, tests are run. An introduction to the approach we're using can be found [here](https://www.docker.com/blog/getting-started-with-docker-for-arm-on-linux/).

You can get a Raspbian disk image either by following the instructions in the _Manual Raspberry Pi disk image creation_ section, or downloading the latest one from [here](http://ps3.respira.works/rpi-images/). The pi user's password is `respiraworks`.

Once you have a disk image, you need to burn it to the rPi.  Follow steps [here](https://www.raspberrypi.org/documentation/installation/installing-images/).  You will need a MicroSD adapter.

The disk image will come with the latest RespiraWorks GUI app upon generation of the disk image and be set up to start it fullscreen on boot. To build the GUI app yourself, see the _Build the GUI Locally for the rPi_ section.

## Manual Raspberry Pi disk image creation

Setting up the Raspberry Pi disk image is nontrivial. Thus, it is our goal to automate as much of it as possible. The overall approach is as follows:

1. From our desired starting point, [Raspbian Buster](https://www.raspberrypi.org/downloads/raspbian/), currently, create an SSH-enabled version of the image. (This means loading the image, probably in a VM, and running `sudo systemctl enable ssh`.)
2. Resize the filesystem to accommodate the software we will install.
3. Save this image to a place where we can easily get it.
4. Use automation to
    a. fire up a VM (e.g. using Vagrant) from the base image,
    b. install (e.g. using Ansible) all necessary libraries, tools, and services,
    c. upload the image to a public HTTPS repository.

Although it may be possible to automate all these steps, step 4 is the most important and can easily become part of our CI system.

## Raspbian Image Generation
### I. Resizing the Raspbian image
In this step, we use `qemu-img resize` to add 3GB of extra storage to the image for space to install docker and the docker GUI image.

[HOST] `$ qemu-img resize -f raw raspbian.img +3G`

Spin up a QEMU instance of the raspbian image file. See _Running Raspbian (or compatible) image in a VM_ for the command.

[QEMU] `$ sudo fdisk -l /dev/sda` (Note starting sector of Linux partition)

[QEMU] `$ sudo fdisk /dev/sda`
- Delete Linux partition (`d` -> `2`)
- Create new partition (`n` ->  _type_: primary, _start_: starting sector of deleted partition, _end_: default, _remove ext4 signature_: no)

Write changes and exit fdisk (`w`)

Shut down VM
[QEMU] `$ sudo shutdown -h now`

OR

(`Ctrl+A` -> `C` -> `system_powerdown` -> `Ctrl+A` -> `X`)

Spin up a QEMU instance of the image again.

[QEMU] `$ sudo resize2fs /dev/sda2`
### II. Enable SSH & UART
Spin up a QEMU instance of the image

[QEMU] `$ sudo systemctl enable ssh`

[QEMU] `$ sudo systemctl start ssh`

[QEMU] `$ sudo raspi-config`

Navigate to `Interfacing Options` -> `Serial`

login shell over serial: `no`

hardware enabled: `yes`

Shut down and spin up the instance to save changes. For Ansible to be able to connect to the instance, you must SSH manually at least once prior to save the host fingerprint

[HOST] `$ ssh -p5022 pi@localhost`

Enter yes to save host fingerprint

### III. Docker & Openbox (Automated)
Ansible uses sshpass with this script since it relies on an ssh connection. Make sure sshpass is installed by running:

[HOST] `$ apt-get install sshpass`

With the qemu instance running, open another terminal window and run the ansible playbook with the following command in the same directory as the hosts file:

[HOST] `$ ansible-playbook -i hosts --ask-become-pass rpi-primer.yml`

Enter the raspberry pi password when asked.

Change the default pi user's password to `respiraworks`:

[QEMU] `$ sudo raspi-config`

Navigate to `Change User Password` and use `respiraworks` when prompted.

Now shut down the QEMU instance and all the changes we made to the image are now written to the image. You can now [burn the image to an SD card](https://www.raspberrypi.org/documentation/installation/installing-images/) and boot it up in a Raspberry Pi. This script installed docker, set up openbox, and cached the latest GUI from dockerhub.

### Update the GUI Docker image after changes have been committed to `master`:

Stop the GUI Docker container if running:

[rPi] `$ docker stop gui`

Delete the GUI Docker container:

[rPi] `$ docker container rm gui`

[rPi] `$ docker system prune`

Pull the latest GUI Docker image:

[rPi] `$ docker pull respiraworks/gui`

Now after you reboot the rPi, the new GUI should start on boot.

## Running Raspbian (or compatible) image in a VM

_Note: this section is important only if you intend to develop or test without needing to use an actual Raspberry Pi 3B+._

Running Raspbian in a VM (e.g. to do CI in a non ARM environment) is fairly easy. One approach is to use [QEMU](https://www.qemu.org/) and a some kernel tweaks to facilitate easy emulation. [This project](https://github.com/dhruvvyas90/qemu-rpi-kernel) describes how to do all of this with relative ease.

During local development, I run the following command to spin up a virtual RPi.

```
qemu-system-arm \
  -nographic \
  -M versatilepb \
  -cpu arm1176 \
  -m 256 \
  -hda 2020-02-13-raspbian-buster.img \
  -net nic \
  -net user,hostfwd=tcp::5022-:22 \
  -dtb versatile-pb-buster.dtb \
  -kernel kernel-qemu-4.19.50-buster \
  -append 'root=/dev/sda2 panic=1' \
  -no-reboot
```

The appropriate kernel (kernel-qemu-4.19.50-buster) and device tree (versatile-pb-buster.dtb) must be in the same directory as the Raspbian image (2020-02-13-raspbian-buster.img)

### Preparing a Raspberry Pi and Interacting with The GUI

[This article](https://medium.com/@elango_11461/how-to-setup-raspberry-pi-3-model-b-only-using-usb-drive-and-android-phone-charger-b66048a78a7e) is useful for those who want to work with a RPi, but may not have an SD card or adaptor. This approach can also be used to set up a RPi that'll be used in a "headless" manner.

Interacting with the GUI in a development setting can be accomplished a few different ways:

1. Connect a Touchscreen to the RPi.
2. Connect a usb mouse, usb keyboard, and HDMI display to the RPi.
3. Enable SSH on the RPi using `sudo raspi-config`, then use X11 forwarding over SSH when connecting from your development machine.
4. Enable VNC on the RPi using `sudo raspi-config`, then use a VNC client from your development machine. Note - it appears only the RealVNC client will work.

### Troubleshooting Raspberry Pi Issues

If you have difficulty connecting to the RPi, hook it up to a display, mouse, and keyboard and ensure that the Wifi or Ethernet configuration is correct. When using Wifi, you may need to add an ISO "country code" to the `/etc/wpa_supplicant/wpa_supplicant.conf` file. Here is an example that works for me. Note the `update_config=1` will cause the file to be updated if you modify the configuration using the GUI.

```
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=US

network={
	ssid="<redacted>"
	psk="<redacted>"
	key_mgmt=WPA-PSK
}
```

## Build the GUI Locally for the rPi
First, burn the custom Raspbian image to a microSD card. If you don't want to make your own, you can download it [here](http://ps3.respira.works/rpi-images/). This image was based on Raspbian's May 27, 2020 release. To burn it, follow [these](https://www.raspberrypi.org/documentation/installation/installing-images/) instructions. You'll need a microSD card adapter if you don't already have one.

To recap, the following changes have been made from fresh Raspbian to the image provided:
1) Docker is installed
2) respiraworks/gui Docker image (built as of June 17th, 2020) is cached
3) Openbox is set up so the GUI Docker container is spun up on boot
4) default password is changed to `respiraworks`
5) disable first-time boot set-up wizard (deleted /etc/xdg/autostart/piwiz.desktop)

As discussed earlier, there are two layers to the GUI. The Docker image contains the Qt dependencies and built GUI. This docker image is run as a container on the rPi and the GUI executable built inside the container is run. For emphasis, the GUI executable is _not_ built to developer hardware or the rPi directly for deployment. The GUI is built inside the Docker container. Hence, to build the GUI for deployment testing on the rPi, we need to build the Docker image itself:

Update package list:

[HOST] `sudo apt-get update`

Install Docker:

[HOST] `sudo apt-get install -y docker-ce`

Because the rPi has armv7 CPU architecture, the Docker image must be built using `buildx`, Docker's cross platform builder.

Install binfmt-support:

[HOST] `sudo apt-get install -y binfmt-support`

Enable Docker Experimental mode for cross-platform builds:

[HOST] `export DOCKER_CLI_EXPERIMENTAL=enabled`

Create an image builder instance:

[HOST] `docker buildx create --name gui`

Use image builder:

[HOST] `docker buildx use gui`

Inspect builder:

[HOST] `docker buildx inspect --bootstrap`

Now from the `Ventilator/` directory, build the Docker image of GUI for armv7:

[HOST] `docker buildx build --platform linux/arm/v7 -t respiraworks/gui_local --load -f gui/Dockerfile .`

_Note: The above steps are followed by the CircleCI machine upon every push to master. See the [config file](https://github.com/RespiraWorks/Ventilator/blob/master/.circleci/config.yml) for more details._

After the Docker build is complete, we need to transfer it to the rPi. The fastest way would be to export the Docker image you built and transferring it to the rPi with a USB drive.

Save the Docker image as a tar file to the current directory:

[HOST] `docker save -o gui_local.tar respiraworks/gui_local`

Now transfer it to the rPi. This can be done manually with a USB drive or over the network with `scp`

If you are using the raspbian image we generated, then it'll try to run the GUI from the master branch on boot. To test the Docker image we just built, we'll need to first stop any running Docker containers of the GUI:

List Docker containers running:

[rPi] `docker container ls`

Now copy the container IDs of the containers running and run the following for each one:

[rPi] `docker stop <container id>`

Now load the tar of the GUI into Docker:

[rPi] `docker load -i gui_local.tar`

To run the GUI, run the following command:

[rPi] `docker run --name=gui_local --privileged -d --restart unless-stopped --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --env="DISPLAY" --net=host -i -t respiraworks/gui_local /gui/build/app/ProjectVentilatorGUI`

To stop it, you need to stop the Docker container since we have a flag to automatically restart it when it crashes:

`docker stop gui_local`
