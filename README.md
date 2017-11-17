# Embedded Systems

## ARM

C - two_track_recorder.c

Designed for the (STM32F0 Discovery Board)[http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF253215?sc=internet/evalboard/product/253215.jsp]. Uses a keypad and speaker to make Nintendo sounding songs on the fly. Allows for recording and looping while playing. This program has 4 modes, Control, Play, Record, Playback. Control waits for input to switch to another mode. You get to control mode from any other mode by pushing the B key. To access the other modes from control push:
	1 - Play
	2 - Record
	3 - Playback
Play is a free play note mode. Record will record up to 100 notes. Playback allows you to play the song that was recorded back, plus lets you play other notes on top of it live.


## Dining Philosophers

Go

The classic CS problem in go. This algorithm avoids deadlock by switching the primary fork for one of the philosophers. It tracks wait/think/eat times and outputs them for each philosopher for each loop. The algorithm has not deadlocked in testing, and no philosophers starve. Busy waiting was avoided to increase power savings. There are some inefficiences, in that one of the philosophers tends to eat less than the others, but he still lives so it is acceptable. 


## Energy Efficient Scheduler

C

Low energy multi thread scheduler. Used custom kernel module to measure timing directly from registers.


## Engine Control Unit

C

Implementing injection/spark timer for optimal engine performance. Algorithm adapts to provide spark at optimal engine location. Demonstration using 2 Raspberry Pis and various sensors


## Server/Client

Go

UDP and TCP server/client developed on 2 Beaglebone Black boards to sync clocks, etc.

## PIC

C

The purpose of this design is to use a PIC microcontroller to control the speed of a fan based on temperature. There is no sense running a fan at full voltage and wasting power, so this design allows the fan to run at a low idle speed and increase speed only as the temperature increase.

The fan has 12 speeds, including an idle speed and maximum speed. The speeds are determined by the [PWM duty cycle](https://en.wikipedia.org/wiki/Pulse-width_modulation#Principle). The idle speed is set to a duty cycle of 40%, and max speed is set to 99.6%. The speed control follows this logic: If the register value created by the [ADC](https://en.wikipedia.org/wiki/Analog-to-digital_converter) is within a certain range, then set the duty cycle register to a certain value. To conserve power and processing time, an [interrupt service routine (ISR)](https://en.wikipedia.org/wiki/Interrupt_handler) is implemented based on an internal timer (Timer0). This interrupt occurs about every 65 ms when the Timer0 register overflows. Within the ISR, the program gets the result of the ADC out of the ADRESH register (the 8 MSBs of the conversion) and puts the 8 bit value into a variable (valADC). A flag (statusIntr) is set so the main program knows that an interrupt happened and can now do PWM operations. At the end of the ISR, the GO/DONE bit is set to start the next ADC conversion. The main program will only change the PWM value after the statusIntr flag is set. It does this using an if-else-if statement to determine the range that valADC fits into. The range values were determined experimentally and are represented as 8 bit decimal values in the program. Once the correct range is found, a specific value is put into the PWM duty cycle register (CCPR1L). A conversion ratio between duty cycle and the PWM register value was found experimentally to be: Duty / CCPR1L = Conversion Factor. For example: 100 / 255 = 0.39. This means that any desired duty cycle can be divided by 0.39 to get the appropriate CCPR1L value. The PWM output modulates the fan’s power and changes the speed.


## Sensors

### Gyroscope - MPU6050

Python

Reads 6 registers via I2C and computes the angles of rotation about the X and Y axes. Current version has a 'live' readout on the terminal that [flushes the buffer](http://en.wikipedia.org/wiki/Data_buffer) and writes the current value every 500 milliseconds. The datasheet and register map for the MPU-6050 are in two seperate PDFs and can be found at the [manufacturer's webpage](http://www.invensense.com/mems/gyro/mpu6050.html). The breakout board used here is called GY-521 and can be found 
[on Amazon](http://www.amazon.com/s/ref=nb_sb_noss?url=search-alias%3Delectronics&field-keywords=mpu6050+gy-521&rh=n%3A172282%2Ck%3Ampu6050+gy-521) for around $5.


### Temperature - MCP9808

Python

Reads 1 register via I2C and computes the ambient temperature in [Celsius and Fahrenheit](http://en.wikipedia.org/wiki/Conversion_of_units_of_temperature). Current version whas a 'live' readout on the terminal and also uses a text file to log the date, time, and temperature. To constantly log the temperature in the background, comment out the sys.stdout functions and run the code in the background. Then to print the current temperature value you can display the last line of the log text file using the [tail command](http://linux.die.net/man/1/tail). The datasheet and register map for the MCP9808 can be found at the [manufacturer's webpage](http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en556182). The breakout board used here is sold at Adafruit for around $5.

temperature_mcp9808_plot.py works in the same way as temperature_mcp9808.py but it also streams the time and temperature data to [Plotly](https://plot.ly/), a web service for plotting live data. A similar Raspberry Pi temperature logging project is [detailed by Plotly](https://plot.ly/raspberry-pi/tmp36-temperature-tutorial/) with easy to use copy/paste code to get started. 


### Sensor Startup

C

Multiple sensors from 30-in-1 kit