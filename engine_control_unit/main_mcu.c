#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "mcu.h"

#define thread_decl(NAME) \
void* thread_##NAME(void* param) { \
	SharedVariable* pV = (SharedVariable*) param; \
	body_##NAME(pV); \
	return NULL; }

thread_decl(leds)
thread_decl(track)
thread_decl(schedule)

#define thread_create(NAME) pthread_create(&t_##NAME, NULL, thread_##NAME, &v);
#define thread_join(NAME) pthread_join(t_##NAME, NULL);

int main(int argc, char* argv[]) {
	// Initialize
	SharedVariable v;

	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi.");
		return 1; 
	}

	init_shared_variable(&v);
	init_sensors(&v);

	pthread_t t_leds, //read knock and write ignite
			  t_track,
			  t_schedule;

	while (v.bProgramExit != 1) {
		// Create sensing threads
		thread_create(leds);
		thread_create(track);
		thread_create(schedule);

		// Wait until all threads finish
		thread_join(leds);
		thread_join(track);
		thread_join(schedule);

		delay(1);
	}

	//TODO: Print output curve of max advance. 
	
	printf("Program finished.\n");

	return 0;
}
