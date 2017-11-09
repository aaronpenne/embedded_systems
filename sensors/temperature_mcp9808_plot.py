# temperature_mcp9808_plot.py

import smbus        # Subset of I2C controls to communicate with the sensor
import time         # Needed to loop on a time basis
import datetime     # Allows capturing real date and time info
import plotly.plotly as py      # Plotly library and objects
from plotly.graph_objs import Scatter, Layout, Figure
from config import configPlot   # Brings in my config.py file. See config.py.sample

def get_temperature_c():
    value = bus.read_i2c_block_data(address, temperature_reg)
    regTa = (value[0] << 8) + value[1]
    temperature = (regTa & 0x0FFF) / 16.0
    if (regTa & 0x1000):
        temperature -= 256
    temperature = round(temperature,2)
    return temperature

def get_temperature_f(tempC):
    tempF = tempC * (9.0/5.0) + 32
    tempF = round(tempF,2)
    return tempF

def log_value(file):
    file = open(file,'a')
    now = datetime.datetime.now()
    currentTime = now.strftime("%Y/%m/%d %H:%M:%S")
    currentTempC = get_temperature_c()
    currentTempF = get_temperature_f(currentTempC)
    currentLog = str(currentTime)+" | C: "+str(currentTempC)+" | F: "+str(currentTempF)+"\n"
    stream.write({'x': currentTime, 'y': currentTempF})
    file.write(currentLog)
    file.close()

bus = smbus.SMBus(1)
address = 0x18          # Address of the sensor (found using i2cdetect)
temperature_reg = 0x05  # Address of the temperature register on sensor (found using datasheet)
log_path = configPlot['log_path']   # Update with your desired log path and log file!
period = 600            # 10 minutes

## Start plotly stuff
username = configPlot['username']       # Update with your username!
api_key = configPlot['api_key']         # Update with your API key!
stream_token = configPlot['stream_token']   # Update with your stream token!
py.sign_in(username, api_key)
trace1 = Scatter(
    x=[],
    y=[],
    mode='lines+markers',
    stream=dict(
        token=stream_token,
        maxpoints=200
    )
)
layout = Layout(
    title=configPlot['title']
)
fig = Figure(data=[trace1], layout=layout)
print py.plot(fig, filename=configPlot['filename']
stream = py.Stream(stream_token)
stream.open()
## End Plotly stuff

while True:
    log_value(log_path)
    time.sleep(period)
