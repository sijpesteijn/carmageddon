#/bin/bash

mjpg_streamer -i "/usr/lib/mjpg-streamer/input_uvc.so -d /dev/video0 -n -f 15 -r 320x240" -o "/usr/lib/mjpg-streamer/output_http.so -p 8090 -n -w ./www"

# http://192.168.88.100:8090/?action=stream