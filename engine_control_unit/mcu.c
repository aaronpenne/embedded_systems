#include "mcu.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
  sv->bProgramExit=0;
}

void init_sensors(SharedVariable* sv) {
  pinMode(PIN_SPARK_BASE+0, OUTPUT);
  pinMode(PIN_SPARK_BASE+1, OUTPUT);
  pinMode(PIN_SPARK_BASE+2, OUTPUT);
  pinMode(PIN_SPARK_BASE+3, OUTPUT);
  pinMode(PIN_SPARK_BASE+4, OUTPUT);
  pinMode(PIN_SPARK_BASE+5, OUTPUT);
  pinMode(PIN_SPARK_BASE+6, OUTPUT);
  pinMode(PIN_SPARK_BASE+7, OUTPUT);
  pinMode(PIN_KNOCK_BASE+0, INPUT);
  pinMode(PIN_KNOCK_BASE+1, INPUT);
  pinMode(PIN_KNOCK_BASE+2, INPUT);
  pinMode(PIN_KNOCK_BASE+3, INPUT);
  pinMode(PIN_KNOCK_BASE+4, INPUT);
  pinMode(PIN_KNOCK_BASE+5, INPUT);
  pinMode(PIN_KNOCK_BASE+6, INPUT);
  pinMode(PIN_KNOCK_BASE+7, INPUT);
  pinMode(PIN_TRACK, INPUT);
}

void body_leds(SharedVariable* sv) {
  int i, knock;
  long long time;
  static int lastKnock[8] ={0,0,0,0,0,0,0,0};
   
  while (sv->bProgramExit != 1) {
    //Detect positive edge of knock and update SV
    for(i=0;i<8;i++) {
    knock = digitalRead(PIN_KNOCK_BASE+i);
      if(!lastKnock[i] && knock) {
        sv->knock[i] = 1;
        sv->knock_t[i] = get_current_time_us();
      }
      lastKnock[i] = knock;
    }
    
    //Drive Spark for 100ms after "next spark"
    time = get_current_time_us();
    for(i=0;i<8;i++) {
      if(time > sv->NextSpark[i] && time < (sv->NextSpark[i]+100000))
        digitalWrite(PIN_SPARK_BASE+i,HIGH);
      else 
        digitalWrite(PIN_SPARK_BASE+i,LOW)
    }
  }
  
}

void body_track(SharedVariable* sv) {
  int last_track = 0, track=0;
  while (sv-        >bProgramExit != 1) {
    track = digitalRead(PIN_TRACK);
    if(!last_track && track) {
      sv->TDC = 1;
      sv->TDCT = get_current_time_us();
    }
    last_track = track;      
  }
}

void body_schedule(SharedVariable* sv) {
  long long last_tdc, rot_time curr_time;
  int i;
  //Todo, when reading knock, set back to 0
  while (sv->bProgramExit != 1) {
    curr_time = get_current_time_us();
    //If TDC hit, update speed and angle
    if(sv->TDC) {
      sv->TDC = 0;
      rot_time = sv->TDCT - last_tdc; //TODO: 
      last_tdc = sv->TDCT;
    }
   
    curr_angle = (curr_time - last_tdc) * 360.f / rot_time;
    
    for(i=0;i<8;i++){
      if(sv->knock[i]) {
        //TODO: UPDATE TIMING FOR KNOCK. 
        //STORE LAST MAX ADVANCE POSITION.
        //SET NEW CURR ADVANCE POSITION.
        sv->knock[i] = 0;
      }
    }
    
  }
}


    //digitalWrite(PIN_BUZZER, LOW);
	//int val = digitalRead(PIN_BUTTON);