# temperature_mcp9808.py

import smbus    # Subset of I2C controls to communicate with the sensor
import time     # Needed to loop on a time basis
import sys      # Needed to output to terminal via sys.stdout.write()
import datetime # Allows capturing real date and time info
from config import configTemp   # Brings in my config.py file. See config.py.sample

def get_temperature_c():
    value = bus.read_i2c_block_data(address, temperature_reg)
    regTa = (value[0] << 8) + value[1]
    temperature = (regTa & 0x0FFF) / 16.0
    
    # Watch out for negative values
    if (regTa & 0x1000):
        temperature -= 256
    return temperature

def get_temperature_f(tempC):
    tempF = tempC * (9.0/5.0) + 32
    return tempF

bus = smbus.SMBus(1)
address = 0x18          # Address of the sensor (found using i2cdetect)
temperature_reg = 0x05  # Address of the temperature register on sensor (found using datasheet)
period = 0.5            # 500 ms
log_path = configTemp['log_path']

# Runs forever, but only collect data every 60 seconds
while True:
    
    # Gets the log file
    file = open(log_path,'a')
    
    # Gets the date, time, and temperatures ready for printing
    now = datetime.datetime.now()
    currentTime = now.strftime("%Y/%m/%d | %H:%M:%S")
    currentTempC = get_temperature_c()
    currentTempF = get_temperature_f(currentTempC)
    currentLog = str(currentTime) + " | C: " + str(currentTempC) + " | F: " + str(currentTempF)
    
    # Writes to log file
    file.write(currentLog)
    file.close()
    
    # Writes to terminal, removing old output and putting new data in its place
    sys.stdout.write("\r" + currentLog)
    sys.stdout.flush()
    
    # Does this again every 'period' seconds
    time.sleep(period)
