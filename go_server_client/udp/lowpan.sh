#! /bin/bash
set -x

# Set pan_id for network
iwpan dev wpan0 set pan_id 0xbeef

# Define lowpan0 interface
ip link add link wpan0 name lowpan0 type lowpan

# Set IPv6 Address
# NOTE: The address needs to be unique for other boards
ip -6 addr add 2001:db8::7/64 dev lowpan0

# Turn on lowpan0 interface
ip link set lowpan0 up

