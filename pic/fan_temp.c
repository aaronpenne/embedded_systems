// fan_temp.c

#include <xc.h>

// Configuration
#pragma config FOSC = INTOSC 		// INTOSC oscillator I/O function on CLKIN pin
#pragma config WDTE = OFF 		// WDT disabled
#pragma config PWRTE = OFF 		// PWRT disabled
#pragma config MCLRE = ON 		// MCLR/VPP pin function is digital input
#pragma config CP = OFF 		// Program memory code protection is disabled
#pragma config CPD = OFF 		// Data memory code protection is disabled
#pragma config BOREN = ON 		// Brown-out Reset enabled
#pragma config CLKOUTEN = OFF 		// CLKOUT function is disabled
#pragma config IESO = OFF 		// Internal/External Switchover mode disabled
#pragma config FCMEN = ON 		// Fail-Safe Clock Monitor is enabled
#pragma config WRT = OFF 		// Write protection off
#pragma config PLLEN = ON 		// 4x PLL enabled
#pragma config STVREN = ON 		// Stack Overflow or Underflow will reset
#pragma config BORV = LO 		// Brown-out Reset Voltage (Vbor) trip set low
#pragma config LVP = OFF 		// Low-voltage programming enabled

// Global variables
bit statusIntr = 0; 		// Flag showing that interrupt has happened
bit statusFan = 0; 		// Flag showing program has just begun
unsigned char valADC = 0; 	// Holds value of ADRESH, 8 bits = register = char
unsigned char index = 0; 	// Counter

// Decimal values to compare against ADRESH (valADC)
unsigned char val01 = 40; 	// Temperature ~ 85F
unsigned char val02 = 64; 	// Temperature ~ 89F
unsigned char val03 = 83; 	// Temperature ~ 93F
unsigned char val04 = 102; 	// Temperature ~ 97F
unsigned char val05 = 121; 	// Temperature ~ 101F
unsigned char val06 = 140; 	// Temperature ~ 105F
unsigned char val07 = 159; 	// Temperature ~ 109F
unsigned char val08 = 178; 	// Temperature ~ 113F
unsigned char val09 = 198; 	// Temperature ~ 117F
unsigned char val10 = 218; 	// Temperature ~ 121F
unsigned char val11 = 240; 	// Temperature ~ 125F

void main(void){
	// Setup registers
	OSCCON = 0b01101010; 		// Clock is 4Mhz with internal oscillator
	TRISA = 0b00111011; 		// Output on P5 (RA2) and input on rest
	INTCON = 0b10100000; 		// Global interrupts, Timer0 overflow flag on
	OPTION_REG = 0b00000111; 	// Pull-ups on, interrupt on falling edge, and Timer0 uses prescaler of 1:256

	// PWM setup (per datasheet section 24.3.2)
	PR2 = 0xFF; 		// Sets Timer 2 to maximum period
	CCP1CON = 0b00001100; 	// Active high on all PWM pins
	T2CON = 0b00000100; 	// Turn Timer2 on, no pre/postscalers

	// ADC setup (per datasheet section 16.2.6)
	ANSELA = 0b00000001;	// Analog on P7 (RA0) and digital on rest
	ADCON0 = 0b00000001; 	// ADC on P7 (RA0) and ADC on
	ADCON1 = 0b01010000; 	// ADRESH left justified, Fosc/16, Vref=Vdd

	while(1){
		if(statusIntr == 1){
			if(statusFan == 1){
				if(valADC < val01){
					CCPR1L = 103; 	// Duty cycle = 40.0%
				} else if(valADC < val02){
					CCPR1L = 117; 	// Duty cycle = 45.5%
				} else if(valADC < val03){
					CCPR1L = 131; 	// Duty cycle = 51.0%
				} else if(valADC < val04){
					CCPR1L = 145; 	// Duty cycle = 56.5%
				} else if(valADC < val05){
					CCPR1L = 159; 	// Duty cycle = 62.0%
				} else if(valADC < val06){
					CCPR1L = 173; 	// Duty cycle = 67.5%
				} else if(valADC < val07){
					CCPR1L = 187; 	// Duty cycle = 73.0%
				} else if(valADC < val08){
					CCPR1L = 201; 	// Duty cycle = 78.5%
				} else if(valADC < val09){
					CCPR1L = 215; 	// Duty cycle = 84.0%
				} else if(valADC < val10){
					CCPR1L = 229; 	// Duty cycle = 89.5%
				} else if(valADC < val11){
					CCPR1L = 244; 	// Duty cycle = 95.0%
				} else {
					CCPR1L = 255; 	// Duty cycle = 100%
				}
			} else {
				CCPR1L = 255;	// Burst fan for a bit when powered on
				index++;
				if (index == 100){
					statusFan = 1;
				}
			}
			statusIntr = 0;		// Wait for next interrupt
		}
	}
}

// Interrupt service routine
void interrupt interruptRoutine(void){
	if (TMR0IE && TMR0IF){ 		// Check that source of interrupt is Timer0
		valADC = ADRESH; 	// Gets current ADC value from register
		statusIntr = 1; 	// Flag that the interrupt has happened
		GO_nDONE = 1; 		// Starts ADC process
		TMR0IF = 0; 		// Clears interrupt flag to go back to main
		
		// Testing the PWM output
		//CCPR1L = 0xFF; 
		
		// Testing the PWM output and ADC input
		//CCPR1L = valADC; 
	}
}
