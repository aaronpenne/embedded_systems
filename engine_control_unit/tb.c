#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
  sv->bProgramExit=0;
  sv->motor_speed=0;
  sv->max_advance[0] = 5;
  sv->max_advance[1] = 6;
  sv->max_advance[2] = 7.5;
  sv->max_advance[3] = 9;
  sv->max_advance[4] = 12;
  sv->max_advance[5] = 14.5;
  sv->max_advance[6] = 18.5;
  sv->max_advance[7] = 20.0;
  sv->max_advance[8] = 20.5;
  sv->max_advance[9] = 21;
  sv->max_advance[10] = 21;
}

void init_sensors(SharedVariable* sv) {
  pinMode(PIN_SPARK_BASE+0, INPUT);
  pinMode(PIN_SPARK_BASE+1, INPUT);
  pinMode(PIN_SPARK_BASE+2, INPUT);
  pinMode(PIN_SPARK_BASE+3, INPUT);
  pinMode(PIN_SPARK_BASE+4, INPUT);
  pinMode(PIN_SPARK_BASE+5, INPUT);
  pinMode(PIN_SPARK_BASE+6, INPUT);
  pinMode(PIN_SPARK_BASE+7, INPUT);
  pinMode(PIN_KNOCK_BASE+0, OUTPUT);
  pinMode(PIN_KNOCK_BASE+1, OUTPUT);
  pinMode(PIN_KNOCK_BASE+2, OUTPUT);
  pinMode(PIN_KNOCK_BASE+3, OUTPUT);
  pinMode(PIN_KNOCK_BASE+4, OUTPUT);
  pinMode(PIN_KNOCK_BASE+5, OUTPUT);
  pinMode(PIN_KNOCK_BASE+6, OUTPUT);
  pinMode(PIN_KNOCK_BASE+7, OUTPUT);
  pinMode(PIN_TRACK, INPUT);
  pinMode(PIN_ROTARY0, INPUT);
  pinMode(PIN_ROTARY1, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  softPwmCreate(PIN_MOTOR, 0, 100);
}

void body_leds(SharedVariable* sv) {
  int i, knock, spark;
  long long time;
  static int lastSpark[8] ={0,0,0,0,0,0,0,0};
   
  while (sv->bProgramExit != 1) {
    //Detect positive edge of knock and update SV
    for(i=0;i<8;i++) {
      spark = digitalRead(PIN_SPARK_BASE+i);
      if(!lastSpark[i] && spark) {
        sv->spark[i] = 1;
        sv->spark_t[i] = get_current_time_us();
      }
      lastSpark[i] = spark;
    }
    
    //Drive Spark for 100ms after "next spark"
    time = get_current_time_us();
    for(i=0;i<8;i++) {
      if(time > sv->next_knock[i] && time < (sv->next_knock[i]+100000))
        digitalWrite(PIN_KNOCK_BASE+i,HIGH);
      else 
        digitalWrite(PIN_KNOCK_BASE+i,LOW)
    }
  }
}

void body_track(SharedVariable* sv) {
  int last_track0 = 0, last_track1=0, last_track2=0, track=0;
  long long time;
  while (sv->bProgramExit != 1) {
    track = digitalRead(PIN_TRACK);
    if(last_track0 != track) {
      sv->encoder_pos++;
      time = get_current_time_us();
      sv->encoder_time = time;
      if((track-last_track0) > (last_track0-last_track2)) {
        sv->encoder_pos=0; //TDC
        sv->time_per_rot = time - sv->last_tdc;
        sv->last_tdc = time;
        track = time;
        last_track0 = time;
        last_track1 = time;
        last_track2 = time;
      }
    }
    last_track2 = last_track1;
    last_track1 = last_track0;
    last_track0 = track;       
  }
}

void body_knock(SharedVariable* sv) {
  int i;
  long long time_offset;
  float curr_deg;
  while (sv->bProgramExit != 1) {
    curr_deg = (sv->encoder_pos * 10);
    time_offset = (get_current_time_us() - sv->encoder_time);
    time_per_deg = (sv->time_per_rot/360);
    curr_deg += (float) (time_offset/time_per_deg);
    curr_deg %= 360;
    for(i=0; i<8; i++) {
      if(sv->spark[i]) {
        max_advance = ((i+1)*40) - sv->max_advance[sv->motor_speed];
        if(max_advance<0)
          max_advance += 360;
        if(curr_deg < max_advance )
          sv->knock[i] = 1;
        sv->spark[i]=0;
      }
    }
  }
}

void body_rotary(SharedVariable* sv) {
  static int last0, last1;
  int curr0, curr1;
  while (sv->bProgramExit != 1) {
    curr0 = digitalRead(PIN_ROTARY0);
    curr1 = digitalRead(PIN_ROTARY1);
    speed = sv->motor_speed;
    if(curr0 && !last0 && last1) {
      if(speed < 10) sv->motor_speed++;
    }
    if(curr1 && !last1 && last0) {
      if(speed > 0) sv->motor_speed--;
    }
    last0 = curr0;
    last1 = curr1;
  }
}

void body_motor(SharedVariable* sv) {
  while (sv->bProgramExit != 1) 
    softPwmWrite(PIN_MOTOR,sv->motor_speed*10);
}