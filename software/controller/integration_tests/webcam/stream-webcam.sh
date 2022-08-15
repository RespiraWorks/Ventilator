#! /bin/bash

# Copyright 2020-2022, RespiraWorks
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

print_help() {
    cat <<EOF
Starts a video stream from webcam.

Syntax:
  VIDEO_SOURCE="?" AUDIO_SOURCE="??" stream-webcam.sh <api-key>

Examples:
  VIDEO_SOURCE="/dev/video0" AUDIO_SOURCE="hw:CARD=WEBCAM,DEV=0" ./stream-webcam.sh abab-1234-cdcd-9876-nxnx
  VIDEO_SOURCE="/dev/video2" AUDIO_SOURCE="hw:CARD=WEBCAM_1,DEV=0"  abab-1234-cdcd-9876-nxnx
EOF
}

# \todo syntax check and print help
KEY=""
if [ "$1" != "" ]; then
	KEY="$1"                               # Key
fi

VBR="2500k"                                    # Video bitrate
FPS="24"                                       # FPS
QUAL="1280x720"                                # Quality
YOUTUBE_URL="rtmp://a.rtmp.youtube.com/live2"  # URL

ffmpeg -f alsa -ac 1 -ar 16000 -i $AUDIO_SOURCE \
       -f v4l2 -s $QUAL -r 10 \
       -i $VIDEO_SOURCE -vcodec libx264 -pix_fmt yuv420p \
       -preset ultrafast -strict experimental \
       -r $FPS -g 20 -b:v $VBR -codec:a libmp3lame \
       -ar 44100 -b:a 11025 \
       -bufsize 512k -f flv $YOUTUBE_URL/$KEY
