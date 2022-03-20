# Remote configuration via ssh 

You may want to configure a Raspberry Pi without a dedicated keyboard and mouse.

Herein we will refer to your local development machine (linux or Mac) as `Local` and the remotely accessed Raspberry Pi as `PI`.

1. Flash the operating system to your SD card.
2. Mount the SD card on your local machine.
3. Copy the 2 files in the [boot](boot) directory to the `boot` volume on the SD card.
4. Modify the [wpa_supplicant.conf](boot/wpa_supplicant.conf) file with the correct [country code](https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes) and the WiFi `network name` and `wifi password`.
5. Insert the SD card into a Raspberry Pi (a standalone will do, does not have to be a fully integrated ventilator).
6. On your `local` machine:
```shell
ssh-keygen -R raspberrypi.local
ssh pi@raspberrypi.local
```
where the password will be `raspberry`. The first command may throw an error, but that is ok. It is just to clear any old credentials.
7. Now that you are on the `PI`, run `sudo raspi-config` and set the machine's hostname to be `rw-ventilator` or something more unique if you expect to have multiple of them on your network simultaneously. When exiting the configuration utility, don't reboot it just yet.
8. Also on the `PI`:
```shell
sudo adduser admin
sudo usermod -aG sudo admin
```
where you should provide it with an appropriate password, possibly `respira`.
9. Now reboot the device.
10. Now log in to the new user and password:
```shell
ssh-keygen -R rw-ventilator.local
ssh admin@rw-ventilator.local
```
11. Remotely on the `PI`, enable ssh access for the user:
```shell
cd ~
mkdir .ssh
chmod 700 .ssh
exit
```
12. Again on `local` to transfer the public key and log in with ssh this time:
```shell
scp ~/.ssh/id_rsa.pub admin@rw-ventilator.local:.ssh/authorized_keys
ssh admin@rw-ventilator.local
```
You should no longer be asked for a password when logging in.
13. Now on the `PI`, disable plain text password access:
```shell
sudo nano /etc/ssh/sshd_config
```
and add or modify the appropriate line to read `PasswordAuthentication no`
14. also on the `PI`, run `sudo raspi-config` and set it to boot into command prompt without automatic login. Reboot when exiting.
15. Login with again using `ssh admin@rw-ventilator.local` and on the `PI`:
```shell
sudo deluser pi
```
to get rid of the old username.
16. Now run `sudo raspi-config` again and configure it to boot into Desktop **with** automatic login.

