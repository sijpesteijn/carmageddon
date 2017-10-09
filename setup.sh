#!/bin/bash

nginx -s stop

read -p "Where is car running? local/usb/wifi [local]" luw
luw=${luw:-local}
if [ "$luw" == "local" ]; then
    nginx -c /usr/local/etc/nginx/nginx_car_local.conf
fi
if [ "$luw" == "usb" ]; then
	nginx -c /usr/local/etc/nginx/nginx_car_usb.conf
fi
if [ "$luw" == "wifi" ]; then
	nginx -c /usr/local/etc/nginx/nginx_car_wifi.conf
fi

echo "Setting maven repo"
rm -rf ~/.m2/settings.xml
ln -s ~/.m2/settings.xml_normal ~/.m2/settings.xml

echo "Setting npm"
rm -rf ~/.npmrc
ln -s ~/.npmrc_normal ~/.npmrc