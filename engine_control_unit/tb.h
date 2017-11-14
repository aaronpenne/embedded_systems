#ifndef _MCU_BODY_
#define _MCU_BODY_

#define PIN_SPARK_BASE 0
#define PIN_KNOCK_BASE 8
#define PIN_TRACK 16
#define PIN_ROTARY0 17
#define PIN_ROTARY1 18
#define PIN_MOTOR 19

// B. Shared structure
// All thread fuctions get a shared variable of the structure
// as the function parameter.
// If needed, you can add anything in this structure.
typedef struct SharedVariable {
  long long spark_time[8];
  int bProgramExit;
  int motor_speed;
  float max_advance[11];
  int spark[8];
  int knock[8];
  long long spark_t[8];
  long long next_knock[8];
  int encoder_pos;
  long long time_per_rot;
  long long last_tdc;
  long long encoder_time;
} SharedVariable;


// C. Functions
// You need to implement following functions.
// Do not change any function name here.
void init_shared_variable(SharedVariable* sv);
void init_sensors(SharedVariable* sv);

void body_leds(SharedVariable* sv);
void body_track(SharedVariable* sv);
void body_rotary(SharedVariable* sv);
void body_motor(SharedVariable* sv);
void body_knock(SharedVariable* sv);

long long get_current_time_us();

#endif
