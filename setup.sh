#!/bin/bash

nginx -s stop


read -p "Where is car running? local/usb/wifi [local]" luw
luw=${luw:-local}
if [ "$luw" == "local" ]; then
    nginx -c /usr/local/etc/nginx/nginx_car_local.conf
    sed -i -e 's/192\.168\.88\.38   carmageddon\.dev/127\.0\.0\.1\tcarmageddon.dev/g' /etc/hosts
fi
if [ "$luw" == "usb" ]; then
	nginx -c /usr/local/etc/nginx/nginx_car_bb_usb.conf
fi
if [ "$luw" == "wifi" ]; then
	nginx -c /usr/local/etc/nginx/nginx_car_bb_wifi.conf
	sed -i -e 's/127.0.0.1\tcarmageddon.dev/192.168.88.38\tcarmageddon.dev/g' /etc/hosts
fi

echo "Setting maven repo"
rm -rf ~/.m2/settings.xml
ln -s ~/.m2/settings.xml_normal ~/.m2/settings.xml

echo "Setting npm"
rm -rf ~/.npmrc
ln -s ~/.npmrc_normal ~/.npmrc