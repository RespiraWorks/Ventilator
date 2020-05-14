## Ansible Files (ansible 2.5.1)
### rpi-primer.yml
This is the Ansible script that installs Docker on the target system and also configures the system to automatically boot into OpenBox. Either a QEMU instance or a Raspberry Pi accessible via SSH must be running for it to work.

The command used to run the script is as follows:

`$ ansible-playbook -i [inventory] --ask-become-pass rpi-primer.yml`

The terminal will ask for the SUDO password for Raspbian. By default, this is "raspberry" without the quotes.

The `inventory` parameter specifies a file containing a list of hosts to provision. For the default `hosts` file, this will be a virtual machine. However, you can also use the `hosts.raspberrypi` file to deploy to a Raspberry Pi on your network (having the default hostname `raspberrypi.local`.) This will be useful for development testing.

### hosts
This is Ansible's inventory file that keeps track of the targets of playbooks. Place this file at `/etc/ansible/` or replace the contents of the preexisting `hosts` file

## QEMU Files (QEMU emulator version 2.11.1)

Both of these files are from [this](https://github.com/dhruvvyas90/qemu-rpi-kernel) repo with RPi kernels
### kernel-qemu-4.19.50-buster
Kernel used to spin up Raspbian via QEMU.

### versatile-pb-buster.dtb
Compiled device tree used to spin up Raspbian via QEMU

Ensure both of these files are in the same directory as the raspbian image.
