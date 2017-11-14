#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

// Section 2: Energy-efficient real time scheduler
// Important! DO NOT MODIFY this file. You will not submit this file.
// This file is provided for your convenience to implement a user-space scheduler.
// For more details, please see the instructions in the class website.

// schduler.c: User-level scheduler implementation interface

// Scheduler controls
typedef struct task_selection {
	int task; // Task ID
	int freq; // 0: Low frequency, 1: High frequency
} TaskSelection;

void init_scheduler(int runningTimeInSec);
long long prepare_tasks(int* aliveTasks, void* sv);	// Return: Idle time in microeconds.
													// If less than 0, terminate the scheduler
													// because the running time is elapsed.
void execute_task(TaskSelection sel);

// Timing controls
long long get_current_time_us();
long long get_scheduler_elapsed_time_us();

// For Debug: Deferred print
void init_deferred_buffer(unsigned int size);
void printDBG(const char* format, ... ); // You can use the defered print for debug instead of "printf".
void release_buffer(int fd); // Called when the scheduler terminates.

#endif
