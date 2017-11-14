#include "sensor_startup.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

// Pins For Reference
// #define PIN_BUTTON	0	// Button
// #define PIN_YELLOW	1	// 2-Color LED
// #define PIN_TEMP 	4	// Temperature
// #define PIN_TRACK	5	// Tracking Sensor
// #define PIN_SHOCK	6	// Shock Sensor 
// #define PIN_RED		7	// RGB(3-Color) LED
// #define PIN_GREEN	8	// RGB(3-Color) LED
// #define PIN_BLUE		9	// RGB(3-Color) LED
// #define PIN_ALED		12	// Auto-flash Red
// #define PIN_BUZZER	13	// Buzzer

// Declaring these here to save improve performance.
// Values for triggered state are stored here.
#define TRIG_BUTTON	0	// 0=pressed, 1=idle
#define TRIG_TEMP	1	// 0=idle, 1=hot
#define TRIG_TRACK	0	// 0=object, 1=idle
#define TRIG_SHOCK	0	// 0=shock, 1=idle

int COUNTER = 0;
int VALUE_BUTTON = 1;
int CONTROL_RGB = 0;

void init_shared_variable(SharedVariable* sv) {
	// You can initialize the shared variable if needed.
	sv->bProgramExit = 0;
	sv->STATE_IDLE	 = 1;
	sv->STATE_DRIVE	 = 0;
	sv->STATE_TEMP	 = 0;
	sv->STATE_TRACK	 = 0;
	sv->STATE_SHOCK	 = 0;
}

void init_sensors(SharedVariable* sv) {
	pinMode(PIN_BUTTON,	INPUT);
	pinMode(PIN_YELLOW,	OUTPUT);
	pinMode(PIN_TEMP,	INPUT);
	pinMode(PIN_TRACK,	INPUT);
	pinMode(PIN_SHOCK,	INPUT);
	pinMode(PIN_RED,	OUTPUT);
	pinMode(PIN_GREEN,	OUTPUT);
	pinMode(PIN_BLUE,	OUTPUT);
	pinMode(PIN_ALED,	OUTPUT);
	pinMode(PIN_BUZZER,	OUTPUT);
	
	softPwmCreate(PIN_RED,  0x00, 0xff);
	softPwmCreate(PIN_BLUE, 0x00, 0xff);

	pullUpDnControl(PIN_BUTTON, PUD_UP);
}

void body_button(SharedVariable* sv) {
	COUNTER++;
	// Debounce
	if(COUNTER % 50 == 0) {
		VALUE_BUTTON = digitalRead(PIN_BUTTON);
		if((VALUE_BUTTON == TRIG_BUTTON)) {
			sv->STATE_IDLE  = !sv->STATE_IDLE;
			sv->STATE_DRIVE = !sv->STATE_DRIVE;
			COUNTER = 1;
		}
	
		if(sv->STATE_IDLE) {
			sv->STATE_DRIVE	 = 0;
			sv->STATE_TEMP	 = 0;
			sv->STATE_TRACK	 = 0;
			sv->STATE_SHOCK	 = 0;
			CONTROL_RGB = 0;
		}
	}
}

void body_twocolor(SharedVariable* sv) {
	if(sv->STATE_IDLE) {
		digitalWrite(PIN_YELLOW, LOW);
	} else if(sv->STATE_DRIVE) {
		digitalWrite(PIN_YELLOW, HIGH);
	}
}

void body_temp(SharedVariable* sv) {
	if(sv->STATE_DRIVE) {
		if(digitalRead(PIN_TEMP) == TRIG_TEMP) {
			sv->STATE_TEMP = 1;
			printf("%d\t!!! Temp  !!!\n", COUNTER);
		} else {
			sv->STATE_TEMP = 0;
		}
	}
}

void body_track(SharedVariable* sv) {
	if(sv->STATE_DRIVE) {
		if(digitalRead(PIN_TRACK) == TRIG_TRACK) {
			sv->STATE_TRACK = 1;
			printf("%d\t### Track ###\n", COUNTER);
		}
	}
}

void body_shock(SharedVariable* sv) {
	if(sv->STATE_DRIVE) {
		if(digitalRead(PIN_SHOCK) == TRIG_SHOCK) {
			sv->STATE_SHOCK = 1;
			printf("%d\t*** Shock ***\n", COUNTER);
		}
	}
}

void body_rgbcolor(SharedVariable* sv) {
	if(sv->STATE_IDLE) {
		softPwmWrite(PIN_RED,	0x00);
		digitalWrite(PIN_GREEN,	LOW);
		softPwmWrite(PIN_BLUE,	0xff);
	} else if((sv->STATE_DRIVE) && (CONTROL_RGB == 0)) {
		softPwmWrite(PIN_RED,	0x00);
		digitalWrite(PIN_GREEN,	HIGH);
		softPwmWrite(PIN_BLUE,	0x00);
		if(sv->STATE_SHOCK) {
			softPwmWrite(PIN_RED,	0xff);
			digitalWrite(PIN_GREEN,	LOW);
			softPwmWrite(PIN_BLUE,	0x00);
			CONTROL_RGB = 1;
		} else if(sv->STATE_TRACK) {
			softPwmWrite(PIN_RED,	0x76);
			digitalWrite(PIN_GREEN,	LOW);
			softPwmWrite(PIN_BLUE,	0xee);
			CONTROL_RGB = 1;
		}
	}
}

void body_aled(SharedVariable* sv) {
	if((sv->STATE_DRIVE) && (sv->STATE_TEMP)) {
		digitalWrite(PIN_ALED, HIGH);
	} else {
		digitalWrite(PIN_ALED, LOW);
	}
		
}

void body_buzzer(SharedVariable* sv) {
	if((sv->STATE_DRIVE) && (sv->STATE_TEMP)) {
		digitalWrite(PIN_BUZZER, HIGH);
	} else {
		digitalWrite(PIN_BUZZER, LOW);
	}
}
