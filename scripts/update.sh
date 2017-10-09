#!/bin/bash

git pull
make
echo Start carmageddon. Check /var/log/syslog
./dist/carmageddon > /dev/null 2>&1