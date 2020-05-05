## Ansible Files (ansible 2.5.1)
### rpi-primer.yml
This is the Ansible script that installs docker and sets up the image to automatically boot into OpenBox. The QEMU instance must be running for it to work.
The command used to run the script is as follows:
$ ansible-playbook --ask-become-pass rpi-primer.yml 

### hosts
This is Ansible's inventory file that keeps track of the targets of playbooks. 

## QEMU Files (QEMU emulator version 2.11.1)
### kernel-qemu-4.19.50-buster
Kernel used to spin up Raspbian via QEMU

### versatile-pb-buster.dtb
Compiled device tree used to spin up Raspbian via QEMU
