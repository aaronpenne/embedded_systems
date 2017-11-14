#ifndef _MCU_BODY_
#define _MCU_BODY_

// A. Pin number definitions (DO NOT MODIFY)
#define PIN_SPARK_BASE 0
#define PIN_KNOCK_BASE 8
#define PIN_TRACK 16

// B. Shared structure
// All thread fuctions get a shared variable of the structure
// as the function parameter.
// If needed, you can add anything in this structure.
typedef struct shared_variable {
  long long spark_time[8];
   wl_cnt[8];
   int bProgramExit;
} SharedVariable;


// C. Functions
// You need to implement following functions.
// Do not change any function name here.
void init_shared_variable(SharedVariable* sv);
void init_sensors(SharedVariable* sv);

void body_leds(SharedVariable* sv);
void body_track(SharedVariable* sv);
void body_schedule(SharedVariable* sv);

long long get_current_time_us();

#endif
