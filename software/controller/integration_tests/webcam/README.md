# Server webcam access

The `bigcheese` server has 2 webcams attached for visual and audible confirmation of prototype operation. The
following document describes how to access the webcams.

## Live streaming scripts

Read all of the following before attempting to stream.

- Prepare Youtube for streaming
  - You may stream directly to channels on the RespiraWorks organization account on Youtube. Please request the team
	for credentials/priveleges to manage the channel.
  - Alternatively, you may set up your own youtube account to receive these streams.
	- Go to https://www.youtube.com/features?nv=1 and request "Live streaming"
	- Wait 24 hours for "Live streaming" to be enabled on your account
- Go to YouTube Dashboard: https://www.youtube.com/live_dashboard?nv=1 and start a new stream
- Change category to "Nonprofits & Activism" and privacy to "Unlisted".
- Copy "Stream name/key" and prepare to paste it as argument for script in command line
- To run local copy of script:
  - Add execute permissions to the script: `chmod +x stream-to-youtube.sh`
  - Run the script with the KEY passed as the argument: `./stream-to-youtube.sh 8yuc-2eb2-74e9-tmb6-2cm6`
	(this is an example key)
- There are two scripts called `stream-webcam1_to-youtube.sh` and `stream-webcam2_to-youtube.sh` added to
  `/usr/local/bin` one for each of the two webcams attached to the server. Just run them as sudo to stream the live on
  Youtube from a selected webcam.
- Check that on the YouTube Dashboard that the status has gone from `OFFLINE` to `STARTING` to `LIVE`.
- The URL for the stream is available at the bottom of the page
- The current setup **CANNOT** stream both webcams onto YouTube at the same time.

## Manual methods

**WARNING: these maybe obsolete**

The following are attempted low-level methods for accessing the webcams. Some of these may not be the easiest or
best choice long-term.

### Audio permissions
You will need to give yourself video and audio permissions by adding yourself to those groups:

```sudo usermod -a -G audio $(whoami)```

Restart your session. Verify your username is in the audio group with:

```fgrep -ie 'audio' /etc/group```

### motion

Tried playing with `motion` first. The conf file is included which should go into ~/.motion/
But it appears motion will not support sound, which I think we may want to have at some point.

### ffmpeg

* Uses the video4linux2 (or simply v4l2) input device to capture live input such as from a webcam.
* To list the supported, connected capture devices you can use the v4l-ctl tool: `v4l2-ctl --list-devices`

To get sound:

```ffmpeg -f alsa -ac 1 -ar 16000 -i hw:1,0 -t 10 out.wav```

To figure out parameter for hw:

```arecord -l or arecord -L will list devices```

- To survive the reboot, get the device name from the output of `sudo arecord -L`:

```
    hw:CARD=WEBCAM_1,DEV=0 <-- device name
	    GENERAL WEBCAM, USB Audio
	    Direct hardware device without any conversions
	plughw:CARD=WEBCAM_1,DEV=0  <-- device name
	    GENERAL WEBCAM, USB Audio
	    Hardware device with all software conversions
```

To get video:


```
ffmpeg -f v4l2 -framerate 15 -i /dev/video0 output3.mkv

```

but will not play in VLC. Wrong codec? Can we get it to convert?

This gets us video and sound together:

```
ffmpeg \
-f v4l2 -framerate 15 -i /dev/video0 \
-f alsa -ac 1 -ar 16000 -i hw:1,0 \
-t 10 output3.mkv
```

To stream:
```
ffmpeg \
-f v4l2 -i /dev/video0 \
-f alsa -ac 1 -ar 16000 -i hw:1,0 \
-vcodec libx264 -tune zerolatency -f mpegts udp://192.168.1.8:8081
```

Receive like this:
```
ffplay udp://192.168.1.12:8081
```
