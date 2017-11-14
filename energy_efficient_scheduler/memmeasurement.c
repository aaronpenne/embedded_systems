// memmeasurement.c
// Aaron Penne

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>
#include <pthread.h>

#include <sys/timeb.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "governor_part1.h"

// Workload that traverses allocated memory area in a reverse order (to minimize prefetching effect)
static int DEF_ITERATION = 10000;
static int DEF_STRIDE = 128;
static int DEF_SIZE = 1024*1024; 
static int DEF_FREQ = 1;

static int* v;
void workload_init(int STRIDE, int SIZE) {
	// Set up array
	v = (int*)malloc(sizeof(int)*SIZE);
	int i;
	for (i = STRIDE; i < SIZE; i += STRIDE) {
		v[i] = (int)&v[i-STRIDE];
	}
	v[0] = (int)&v[i-STRIDE];

	// Warmup the memory
	int* p = v;
	do {
		p = (int*)*p;
	} while (p != (int*)v);
}

void workload_finish() {
	free(v);
}

void workload_body(int iteration, int STRIDE, int SIZE) {
	if (SIZE <= STRIDE) return;

	// Get result
	long long sum_t = 0;
	int n = 0;
	int* p = v;
	int i;

	for (i = 0; i < iteration; i++) {
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 
		p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; p = (int*)*p; 

	}

}

static long long get_current_time_us() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long long curTime = (long long)tv.tv_sec * 1000 * 1000 + (long long)tv.tv_usec;
	return curTime;
}

static inline void reset_counters() {
	// Reset all cycle counter and event counters
	asm volatile ("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r" (0x00000017));  
}

static inline unsigned int read_event_counter(unsigned int counter) {
	unsigned int value = 0x00000000;

	// PMSELR
	// bit[4:0] selects counter to read from
	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r" (counter));

	// PMXEVCNTR
	// bit[31:0] contains the value of currently selected counter
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (value));

	return value;
}

static inline unsigned int read_cycle_counter(void) {
	unsigned int value = 0x00000000;

	// PMCCNTR
	// bit[31:0] contains the value of the cycle counter
	asm volatile ("mrc p15, 0, %0, c9, c13, 0\n\t" : "=r" (value));

	return value;
}

int main(int argc, char *argv[]) 
{
	if(argc == 5) {
		DEF_ITERATION = atoi(argv[1]);
		DEF_STRIDE = atoi(argv[2]);
		DEF_SIZE = atoi(argv[3])*atoi(argv[3]);
		DEF_FREQ = atoi(argv[4]);
		printf("Iteration: %d, Stride: %d, Size: %d, Freq: %d\n", DEF_ITERATION, DEF_STRIDE, DEF_SIZE, DEF_FREQ); 
	} else if(argc == 1) {
		printf("Iteration: %d, Stride: %d, Size: %d, Freq: %d\n", DEF_ITERATION, DEF_STRIDE, DEF_SIZE, DEF_FREQ); 
	} else {
		printf("USAGE:\nsudo ./memmeasurement <DEF_ITERATION> <DEF_STRIDE> <sqrt DEF_SIZE> <FREQ: 1 or 0>");
		exit(0);
	}

	char szOldGovernor[32];
	unsigned long long t0, t1;
	
	unsigned int i;
	unsigned int counter_value[4];
	unsigned int a, b;

	// Initialize
	set_governor("userspace", szOldGovernor);

	if(DEF_FREQ == 0) {
		set_by_min_freq();
	} else {
		set_by_max_freq();
	}

	workload_init(DEF_STRIDE, DEF_SIZE);
	printf("Measurement start.\n");
	
	// 1. Reset measurement metrics
	t0 = get_current_time_us();
	reset_counters();

	// 2. Run workload
	workload_body(DEF_ITERATION, DEF_STRIDE, DEF_SIZE);

	// 3. Here, you may need to get performance counters.
	t1 = get_current_time_us();

	for(i = 0; i < 4; i++) {
		counter_value[i] = read_event_counter(i);
	}
	counter_value[4] = read_cycle_counter();	
	

	// Print results
	int freq = get_cur_freq();
	
	

	printf("Exe_time: %lld us at %d kHz\n\n", t1-t0, freq);
	printf("Cycle Counter: %u\n\n", counter_value[4]);

	a = counter_value[0];
	b = counter_value[0]+counter_value[1];
	printf("L1 Hit:  %u\t%u%\n",   counter_value[0], (100*a + b/2)/b);

	a = counter_value[1];
	printf("L1 Miss: %u\t%u%\n\n", counter_value[1], (100*a + b/2)/b);

	a = counter_value[2];
	b = counter_value[2]+counter_value[3];
	printf("L2 Hit:  %u\t%u%\n",   counter_value[2], (100*a + b/2)/b);

	a = counter_value[3];
	printf("L2 Miss: %u\t%u%\n\n", counter_value[3], (100*a + b/2)/b);

	// Finish the program
	workload_finish();
	set_governor(szOldGovernor, NULL);

	return 0;
}
