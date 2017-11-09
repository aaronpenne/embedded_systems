# gyro_mpu6050.py

import smbus    # Subset of I2C controls to communicate with the sensor
import math     # Needed to calculate rotation values
import sys      # Allows writing to stdout
import time     # Needed to loop on a time basis

# A word is two bytes, and makes up 1 whole register on this sensor
def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

# The info is stored as 2's complement to allow for negative, so we must convert it
def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def dist(a,b):
    return math.sqrt((a*a)+(b*b))

def get_y_rotation(x,y,z):
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def get_x_rotation(x,y,z):
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

bus = smbus.SMBus(1)
address = 0x68
power_mgmt_1 = 0x6b

# Scaling numbers given in datasheet
scale_gyro = 131
scale_accel = 16384.0

# How many seconds the sensor data will take to refresh
period = 0.5

# Turn on the sensor
bus.write_byte_data(address, power_mgmt_1, 0)

# Run this loop 
while True:
    # Grab the gyro data for each axis using registers provided in datasheet
    gyro_xout = read_word_2c(0x43)
    gyro_yout = read_word_2c(0x45)
    gyro_zout = read_word_2c(0x47)

    # Scale the gyro data using the value provided in datasheet
    gyro_xout_scaled = gyro_xout / scale_gyro
    gyro_yout_scaled = gyro_yout / scale_gyro
    gyro_zout_scaled = gyro_zout / scale_gyro

    # Grab the accel data for each axis using registers provided in datasheet
    accel_xout = read_word_2c(0x3b)
    accel_yout = read_word_2c(0x3d)
    accel_zout = read_word_2c(0x3f)

    # Scale the accel data using the value provided in datasheet
    accel_xout_scaled = accel_xout / scale_accel
    accel_yout_scaled = accel_yout / scale_accel
    accel_zout_scaled = accel_zout / scale_accel

    # Gets the rotation in degrees about the X and Y axes
    x_rotation = get_x_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled)
    y_rotation = get_y_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled)
    
    # Print the rotation values to the terminal 
    sys.stdout.write("\rX rotation: " + str(x_rotation) + " | Y rotation: " + str(y_rotation))
    sys.stdout.flush()
    time.sleep(period)     # Would like to see how small this sleep number can get before breaking
