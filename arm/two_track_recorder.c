// two_track_recorder.c

#include <STM32F0xx.h>

void initAnalogDigital(void);
void initFrequency(void);
void initGPIO (void);
void initLED(void);
void initSpeakers(void);
void greenLED(int state);
void blueLED(int state);
int  getKeypad(void);
void SysTickHandler(void);
void delay(uint32_t ms);
void modePlayback(void);
void modeRecord(void);
void modeControl(void);
void modePlay(void);
void playNoteTIM3(void);
void blinkLEDwrong(void);
void blinkLEDwait(void);
void playNote(int noteNum);
void playNoteBoth(int noteNum);
void stopNote(void);
void stopNoteBoth(void);
void clearSong(void);

int m, n, key, SCANCODE;
uint32_t MSTICKS;
uint16_t VOLUME, registerVal;
uint16_t TimerPeriod = 0, ON = 1, OFF = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0;
float frequency[17];
float period[17];
typedef struct{
	uint8_t number;
	uint32_t startTime;
	uint32_t stopTime;
} NOTE;
NOTE song[100];
enum keys{KEY1=1,  KEY2, 	KEY3, 		KEYA, 
					KEY4, 	 KEY5, 	KEY6, 		KEYB, 
					KEY7, 	 KEY8, 	KEY9, 		KEYC, 
					KEYSTAR, KEY0, 	KEYHASH, 	KEYD
};

/*------------------------------------------------------------------------------
 *  int main (void)
 *  Initialize everything and control the flow. This is where 'Control Mode' 
 *	lives. LED blinking is controlled here as well.
 *-----------------------------------------------------------------------------*/
int main(void){
	uint32_t tmp = 0;
	initAnalogDigital();
	initFrequency();
	initSpeakers();
	initLED();
	initGPIO();
	
	GPIOC->PUPDR |= ((5UL<<4*2)|(5UL<<4*3));
  GPIOC->MODER |= ((5UL<<4*0)|(5UL<<4*1));

  TimerPeriod = (SystemCoreClock / 17570 ) - 1;

  Channel1Pulse = ((50 * (TimerPeriod - 1)) / 100);
  Channel2Pulse = ((50 * (TimerPeriod - 1)) / 100);
	TIM1->ARR = 0;
	TIM3->ARR = 0;
	TIM1->CCR1 = Channel1Pulse;
	TIM3->CCR1 = Channel2Pulse;

	while(1){
		if(getKeypad() == KEY1){
			while(getKeypad() != 0)
				;
			modePlay();
			TIM1->CCR1 = 0;
			TIM3->CCR1 = 0;
		}
		if(getKeypad() == KEY2){
			while(getKeypad() != 0)
				;
			modeRecord();
			TIM1->CCR1 = 0;
			TIM3->CCR1 = 0;
		}
		if(getKeypad() == KEY3){
			while(getKeypad() != 0)
				;
			modePlayback();
			TIM1->CCR1 = 0;
			TIM3->CCR1 = 0;
		}
		tmp = getKeypad();
		if(tmp != 0 && tmp != KEY1 && tmp != KEY2 && tmp != KEY3){
			blinkLEDwrong();
		}
		blinkLEDwait();
		TIM1->CCR1 = 0;
		TIM3->CCR1 = 0;
	}
}

/*------------------------------------------------------------------------------
 *  void initGPIO (void)
 *  Turns on GPIOA and GPIOC, enables SysTick, sets pins
 *-----------------------------------------------------------------------------*/
void initGPIO (void){   
	RCC->AHBENR |= (1UL << 19)|(1UL << 17);
  SysTick_Config(SystemCoreClock / 1000);
  GPIOA->MODER    |=  (3UL<<2*1); 
}

/*------------------------------------------------------------------------------
 *  void initAnalogDigital (void)
 *  Turns on and calibrates analog to digital converter
 *-----------------------------------------------------------------------------*/
void initAnalogDigital (void){
  RCC->APB2ENR |= (1UL << 9);
	ADC1->CR |= (uint32_t)ADC_CR_ADEN; 
  ADC->CCR |= (1UL<<24);
  ADC1->CFGR1 |= (1UL<<13);
  ADC1->CHSELR |= (1UL<<1);
  ADC1->SMPR  |= (7UL<<0);
  while(!(ADC1->ISR)){
		;
	}
  ADC1->CR |= (uint32_t)ADC_CR_ADSTART; 
}

/*------------------------------------------------------------------------------
 *  void clearSong(void)
 *  Sets the song array/struct to all zeroes
 *-----------------------------------------------------------------------------*/
void clearSong(void){
	int i;
	for(i = 0; i < 100; i++){
		song[i].number = (uint8_t)0;
		song[i].startTime = (uint32_t)0;
		song[i].stopTime = (uint32_t)0;
	}
}

/*------------------------------------------------------------------------------
 *  void initLED(void)
 *  Sets GPIOC pins for LED, sets as output
 *-----------------------------------------------------------------------------*/
void initLED(void){
  RCC->AHBENR 	|=  (0x00080000);   
  GPIOC->MODER  &= ~(0x000F0000); 
  GPIOC->MODER  |=  (0x00050000);  
  GPIOC->OTYPER &= ~(0x0300);   
}

/*------------------------------------------------------------------------------
 *  void initSpeakers(void)
 *  Initializes TIM1 and TIM3, sets the pins and modes
 *-----------------------------------------------------------------------------*/
void initSpeakers(void){
  RCC->AHBENR  |= 0x00020000;
  RCC->APB2ENR |= 0x00000800;
  TIM1->PSC   = 0x00000030;
  TIM1->CCMR1 = 0x00006060;
  TIM1->ARR 	= 0x00000000;
  TIM1->SR 		= 0x0000001F;
  TIM1->BDTR 	= 0x00008000;
  TIM1->DMAR 	= 0x00000001;
	TIM1->CCER 	= 0x00000001;
	
	RCC->APB1ENR |= 0x00000002;  
	TIM3->PSC = 0x00000030;
	TIM3->CCMR1 = 0x00006060;
	TIM3->SR   = 0x0000001F;  
	TIM3->ARR  = 0x00000000;
	TIM3->DMAR = 0x00000001;
	TIM3->CCER = 0x00000001;
	GPIOA->AFR[0] = 0x01000000;

	GPIOA->MODER  |= 0x28AA2000;
  GPIOA->OTYPER  = 0;
  GPIOA->OSPEEDR = 0x0CFF0000;
	GPIOA->AFR[1]  = 0x00000022;
}

/*------------------------------------------------------------------------------
 *  void SysTick_Handler(void)
 *  Fairly think SysTick_Handler this time, just gets the pot reading and
 *	increments MSTICKS
 *-----------------------------------------------------------------------------*/
void SysTick_Handler(void){
	MSTICKS++;
	VOLUME = ADC1->DR;
}

/*------------------------------------------------------------------------------
 *  int getKeypad(void)
 *  Checks the keypad for press, returns scancode
 *-----------------------------------------------------------------------------*/
int getKeypad(void){
	for(n = 0; n < 4; n++){
		GPIOC->BSRR |= (1UL<<0|1UL<<1|1UL<<2|1UL<<3); 
		GPIOC->BSRR |= (1UL << (n + 16));							
		for(m = 4; m < 8; m++){
			if((~(GPIOC->IDR) & (1UL<<m)) == (1UL<<m)){
				return(n*4 + m-3);
				}
			}
  }
	return 0;
}

/*------------------------------------------------------------------------------
 *  void modeRecord(void)
 *  Controls the 'Record Mode' putting values into the song struct/array
 *-----------------------------------------------------------------------------*/
void modeRecord(void){
	int i = 0;
	MSTICKS = 0;
	blueLED(OFF);
	greenLED(ON);
	delay(500);
	clearSong();
	
	while(1){
		while((key = getKeypad()) == 0){
			;
		}
		if(key == KEYB){
			delay(1);
			while(getKeypad() != 0){
				;
			}
			delay(1);
			song[i].number = 0;
			return;
		}else{
			playNoteBoth(key);
			song[i].number = key;
			song[i].startTime = MSTICKS;
			while(getKeypad() != 0){
				playNoteBoth(key);
				TIM1->CCR1 = (uint32_t)VOLUME*100/0x0FFF;
			}
			stopNoteBoth();
			song[i].stopTime = MSTICKS;
			i++;
		}
	}		//while(1)
}

/*------------------------------------------------------------------------------
 *  void modePlay(void)
 *  Controls the 'Play Mode' allowing for free play of notes
 *-----------------------------------------------------------------------------*/
void modePlay(void){
	int i = 0;
	MSTICKS = 0;
	blueLED(ON);
	greenLED(OFF);
	delay(500);
	while(1){
		while((key = getKeypad()) == 0){
			;
		}
		if(key == KEYB){
			delay(1);
			while(getKeypad() != 0){
				;
			}
			delay(1);
			return;
		}else{

			while(getKeypad() != 0){
					playNoteBoth(key);
			}
			stopNoteBoth();
			i++;
		}
	}		//while(1)
}

/*------------------------------------------------------------------------------
 *  void modePlayback(void)
 *  Controls 'Playback Mode' stepping through song struct/array and playing 
 *	those notes on TIM1 while responding to input from TIM3
 *-----------------------------------------------------------------------------*/
void modePlayback(void){
	int i;
	i = 0;
	blueLED(ON);
	greenLED(ON);
	delay(500);
	MSTICKS = song[0].startTime - 50;
	while(1){
		if(song[i].number == 0){
			return;
		}
		while(MSTICKS != song[i].startTime){
			if(getKeypad() == KEYB){
				return;
			}
			TIM3->ARR = (uint16_t)period[getKeypad()];
			
		}
		if(song[i].number == 0){
			return;
		}
		playNote(song[i].number);
		while(MSTICKS != song[i].stopTime){
			if(getKeypad() == KEYB){
				return;
			}
			TIM3->ARR = (uint16_t)period[getKeypad()];
			TIM1->CCR1 = (uint32_t)VOLUME*100/0x0FFF;
			TIM3->CCR1 = (uint32_t)VOLUME*100/0x0FFF;
		}
		stopNote();
		
		i++;
		if(song[i].number == 0){
			return;
		}
	}
}

/*------------------------------------------------------------------------------
 *  void playNote(int noteNum)
 *  Turns the note on
 *-----------------------------------------------------------------------------*/
void playNote(int noteNum){
	registerVal = (uint16_t)period[noteNum];
	TIM1->ARR = registerVal;
	TIM1->CCR1 = (uint32_t)VOLUME*100/0x0FFF;
	TIM3->CCR1 = (uint32_t)VOLUME*100/0x0FFF;
}

/*------------------------------------------------------------------------------
 *  void stopNote(void)
 *  Turns sound off
 *-----------------------------------------------------------------------------*/
void stopNote(void){
	TIM1->CCR1 = 0;
}

/*------------------------------------------------------------------------------
 *  void playNoteBoth(int noteNum)
 *  Vestigal function that made it work by not touching TIM3 registers
 *-----------------------------------------------------------------------------*/
void playNoteBoth(int noteNum){
	registerVal = (uint16_t)period[noteNum];
	TIM1->ARR = registerVal;
	TIM1->CCR1 = (uint32_t)VOLUME*100/0x0FFF;
}

/*------------------------------------------------------------------------------
 *  void playNoteBoth(int noteNum)
 *  Vestigal function that made it work by not touching TIM3 registers
 *-----------------------------------------------------------------------------*/
void stopNoteBoth(void){
	TIM1->CCR1 = 0;
	TIM3->CCR1 = 0;
}

/*------------------------------------------------------------------------------
 *  void initFrequency(void)
 *  Sets the period array with known frequency values
 *-----------------------------------------------------------------------------*/
void initFrequency(void){
	int i;

	frequency[0] = 0;
	frequency[1] = 261.626;
	frequency[2] = 277.183;
	frequency[3] = 293.665;
	frequency[4] = 311.127;
	frequency[5] = 329.628;
	frequency[6] = 349.228;
	frequency[7] = 369.994;
	frequency[8] = 0;
	frequency[9] = 391.995;
	frequency[10] = 415.305;
	frequency[11] = 440;
	frequency[12] = 466.164;
	frequency[13] = 493.883;
	frequency[14] = 523.251;
	frequency[15] = 554.365;
	frequency[16] = 587.33;
	period[0] = 0;
	period[8] = 0;
	for(i = 1; i < 8; i++){
		period[i] = 1000000/frequency[i];
	}
	for(i = 9; i < 17; i++){
		period[i] = 1000000/frequency[i];
	}
}

/*------------------------------------------------------------------------------
 *  void delay(uint32_t ms)
 *  Delays by milliseconds
 *-----------------------------------------------------------------------------*/
void delay(uint32_t ms){
  int i;
  int k = 4500;
  for(i = 0; i < k*ms; i++){
    ;
  }
}

/*------------------------------------------------------------------------------
 *  void blueLED(int state)
 *  Turns Blue LED on or off
 *-----------------------------------------------------------------------------*/
void blueLED(int state){
  if(state == ON){                
    GPIOC->BSRR |= (0x00000100);   
  }else{                      
    GPIOC->BSRR |= (0x01000000); 
  }
}

/*------------------------------------------------------------------------------
 *  void greenLED(int state)
 *  Turns Green LED on or off
 *-----------------------------------------------------------------------------*/
void greenLED(int state){
  if(state == ON){             
    GPIOC->BSRR |= (0x00000200); 
  }else{                    
    GPIOC->BSRR |= (0x02000000);  
  }
}

/*------------------------------------------------------------------------------
 *  void blinkLEDwrong(void)
 *  Blinks LED's quickly when wrong button is pushed in 'Control Mode'
 *-----------------------------------------------------------------------------*/
void blinkLEDwrong(void){
		uint32_t i;
	for(i = 0; i < 6; i++){
		blueLED(OFF);
		greenLED(OFF);
		delay(1000/(2*6));
		blueLED(ON);
		greenLED(ON);
		delay(1000/(2*6));
	}
}

/*------------------------------------------------------------------------------
 *  void blinkLEDwait(void)
 *  Blinks LED's slowly while waiting for input in 'Control Mode'
 *-----------------------------------------------------------------------------*/
void blinkLEDwait(void){
	blueLED(OFF);
	greenLED(OFF);
	delay(1000/(2*2));
	blueLED(ON);
	greenLED(ON);
	delay(1000/(2*2));
}
