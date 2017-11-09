# Raspberry Pi
Collection of code used on a Raspberry Pi running Raspbian.
## Sensors
### Gyroscope - MPU6050
<b>gyro_mpu6050.py</b>
<br>
Reads 6 registers via I2C and computes the angles of rotation about the X and Y axes. Current version has a 'live' readout on the terminal that <a href="http://en.wikipedia.org/wiki/Data_buffer">flushes the buffer</a> and writes the current value every 500 milliseconds. The datasheet and register map for the MPU-6050 are in two seperate PDFs and can be found at the <a href="http://www.invensense.com/mems/gyro/mpu6050.html">manufacturer's webpage.</a> The breakout board used here is called GY-521 and can be found <a href="http://www.amazon.com/s/ref=nb_sb_noss?url=search-alias%3Delectronics&field-keywords=mpu6050+gy-521&rh=n%3A172282%2Ck%3Ampu6050+gy-521
">on Amazon</a> for around $5.
<br>
### Temperature - MCP9808
<b>temperature_mcp9808.py</b>
<br>
Reads 1 register via I2C and computes the ambient temperature in <a href="http://en.wikipedia.org/wiki/Conversion_of_units_of_temperature">Celsius and Fahrenheit.</a> Current version whas a 'live' readout on the terminal and also uses a text file to log the date, time, and temperature. To constantly log the temperature in the background, comment out the sys.stdout functions and run the code in the background. Then to print the current temperature value you can display the last line of the log text file using the <a href="http://linux.die.net/man/1/tail">tail command.</a> The datasheet and register map for the MCP9808 can be found at the <a href="http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en556182">manufacturer's webpage.</a> The breakout board used here is sold <a href="http://www.adafruit.com/product/1782">at Adafruit</a> for around $5.
<br><br>
<b>temperature_mcp9808_plot.py</b>
<br>
This file works in the same way as temperature_mcp9808.py but it also streams the time and temperature data to <a href="https://plot.ly/">Plotly</a>, a web service for plotting live data. A similar Raspberry Pi temperature logging project is <a href="https://plot.ly/raspberry-pi/tmp36-temperature-tutorial/">detailed by Plotly</a> with easy to use copy/paste code to get started. 
