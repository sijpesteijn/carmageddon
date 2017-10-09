#!/bin/bash

echo Update ubuntu
apt-get update
apt-get -y upgrade

echo Mounting sd card
mkdir /media/card
sudo mount -t ext4 -o rw /dev/mmcblk0p2 /media/card

echo Moving stuff to sdcard
rm -rf /home
rm -rf /root
rm -rf /var/cache

ln -s /media/card/notouch/home /home
ln -s /media/card/notouch/root /root
ln -s /media/card/notouch/cache /var/cache

ls -l /dev/disk/by-uuid
vi /etc/fstab
UUID=b7da6e16-c9cb-4d0c-b90d-9c1c9b5badcc /media/card ext4 defaults,rw,auto,user,exec 0 0

echo Installing opencv

apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
cmake -DWITH_OPENCL=NO -DWITH_OPENCL_SVM=NO -DWITH_OPENCLAMDFFT=NO -DWITH_OPENCLAMDBLAS=NO -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
make install
ldconfig

echo Installing restbed

apt-get -y install libssl-dev
git submodule init
git submodule update
mkdir build
cd build
cmake -DBUILD_SHARED=YES -DBUILD_EXAMPLES=NO -DBUILD_TESTS=NO ..
make install

mkdir /usr/local/include/restbed
cp -R ../distribution/include/* /usr/local/include/restbed
cp ../distribution/library/librestbed.so.4.5.0 /usr/local/lib
ln -s /usr/local/lib/librest.so.4.5.0 /usr/local/lib/librestbed.so.4
ln -s /usr/local/lib/librest.so.4 /usr/local/lib/librestbed.so
ldconfig

echo Installing wifi dongle TL-WN823N

apt-get install linux-headers-$(uname -r)
git clone https://github.com/Mange/rtl8192eu-linux-driver.git
cd rtl8192eu-linux-driver
make ARCH=arm
make install

echo Finished