#! /bin/bash
#

# Configure ffmpeg command for livestreaming ventilator prototype
# Pass 

if [ "$1" != "" ]; then
    echo "Positional parameter 1 contains something"

	VBR="2500k"                                    # Video bitrate 
	FPS="24"                                       # FPS 
	QUAL="1280x720"                                # Quality
	YOUTUBE_URL="rtmp://a.rtmp.youtube.com/live2"  # URL 
	MIC="hw:CARD=WEBCAM_1,DEV=0"
	SOURCE="/dev/video0"                           # Webcam
	KEY= "$1"                                      # Key


	ffmpeg -f alsa -ac 1 -ar 16000 -i $MIC \
	       -f v4l2 -s $QUAL -r 10 \
	       -i $SOURCE -vcodec libx264 -pix_fmt yuv420p \
	       -preset ultrafast -strict experimental \
	       -r $FPS -g 20 -b:v $VBR -codec:a libmp3lame \
	       -ar 44100 -b:a 11025 \
	       -bufsize 512k -f flv $YOUTUBE_URL/$KEY"
