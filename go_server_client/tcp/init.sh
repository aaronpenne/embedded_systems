#! /bin/bash
set -x

# Turning off USR0 LED
echo none > /sys/class/leds/beaglebone\:green\:usr0/trigger

# Setting default gateway
/sbin/route add default gw 192.168.7.1

# Updating nameserver
echo "nameserver 8.8.8.8" >> /etc/resolv.conf

