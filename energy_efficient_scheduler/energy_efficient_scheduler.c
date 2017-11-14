#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"

long long realDeadline[8]	= {0};
long long execTimeHi[8]		= {0};
long long execTimeLo[8]		= {0};
long long execTimeHiTotal	= 0;
long long execTimeLoTotal	= 0;
long long activeTimeHi		= 0;
long long activeTimeLo		= 0;
long long idleTimeTotal		= 0;
int wasAlive[8]		= {0};
int priority[10]	= {0};
int prioritySize	= 0;
int frequency		= 0;


void learn_workloads(SharedVariable* sv) {
	long long currentTime;
	
	// High Frequency
	set_by_max_freq();
	currentTime = get_current_time_us();

	thread_button(sv);
	execTimeHi[0] = get_current_time_us() - currentTime;
	
	thread_twocolor(sv);
	execTimeHi[1] = get_current_time_us() - currentTime - execTimeHi[0];
	
	thread_temp(sv);
	execTimeHi[2] = get_current_time_us() - currentTime - execTimeHi[1];
	
	thread_track(sv);
	execTimeHi[3] = get_current_time_us() - currentTime - execTimeHi[2];
	
	thread_shock(sv);
	execTimeHi[4] = get_current_time_us() - currentTime - execTimeHi[3];
	
	thread_rgbcolor(sv);
	execTimeHi[5] = get_current_time_us() - currentTime - execTimeHi[4];
	
	thread_aled(sv);
	execTimeHi[6] = get_current_time_us() - currentTime - execTimeHi[5];
	
	thread_buzzer(sv);
	execTimeHi[7] = get_current_time_us() - currentTime - execTimeHi[6];

	// Low Frequency
	set_by_min_freq();
	currentTime = get_current_time_us();

	thread_button(sv);
	execTimeLo[0] = get_current_time_us() - currentTime;
	
	thread_twocolor(sv);
	execTimeLo[1] = get_current_time_us() - currentTime - execTimeLo[0];
	
	thread_temp(sv);
	execTimeLo[2] = get_current_time_us() - currentTime - execTimeLo[1];
	
	thread_track(sv);
	execTimeLo[3] = get_current_time_us() - currentTime - execTimeLo[2];
	
	thread_shock(sv);
	execTimeLo[4] = get_current_time_us() - currentTime - execTimeLo[3];
	
	thread_rgbcolor(sv);
	execTimeLo[5] = get_current_time_us() - currentTime - execTimeLo[4];
	
	thread_aled(sv);
	execTimeLo[6] = get_current_time_us() - currentTime - execTimeLo[5];
	
	thread_buzzer(sv);
	execTimeLo[7] = get_current_time_us() - currentTime - execTimeLo[6];
}

TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
	int i = 0;
	int j = 0;
	int minIndex;
	int swap;
	long long currentTime = get_scheduler_elapsed_time_us();;
	long long margin  = 10000;
		
	// Insert/delete from queue as needed, find the real deadlines
	for(i = 0; i < NUM_TASKS; i++) {
		if((wasAlive[i] == 0) && (aliveTasks[i] == 1)) {
			
			// Update task schedulability
			wasAlive[i] = aliveTasks[i];
			
			// Get the deadline relative to arrival time
			realDeadline[i] = currentTime + workloadDeadlines[i];
			
			// Add task to a new spot at the end of the queue
			priority[prioritySize] = i;
			prioritySize++;
		
		// Task completed
		} else if((wasAlive[i] == 1) && (aliveTasks[i] == 0)) {
			
			// Update task schedulability
			wasAlive[i] = aliveTasks[i];
			
			// Replace the minimum element with the last, and shrink the queue
			priority[0] = priority[prioritySize - 1];
			priority[prioritySize - 1] = 0;
			prioritySize--;
			
			// Add to active time
			if(frequency == 1) {
				activeTimeHi = activeTimeHi + execTimeHi[i];
			} else if(frequency == 0) {
				activeTimeLo = activeTimeLo + execTimeLo[i];
			}
		}
	}
	
	// Sort the queue to find earliest deadline
	for(i = 0; i < prioritySize; i++) {
		minIndex = i;
		for(j = i; j < prioritySize; j++) {
			if(realDeadline[priority[minIndex]] > realDeadline[priority[j]]) {
				minIndex = j;
			}
		}
		swap = priority[i];
		priority[i] = priority[minIndex];
		priority[minIndex] = swap;
	}

	// Determine desired frequency
	if((execTimeHi[priority[0]] * 2) > ((execTimeLo[priority[0]]))) {
		if((execTimeLo[priority[0]] + currentTime + margin) < realDeadline[priority[0]]) {
			frequency = 0;
		} else {
			frequency = 1;
		}
	} else {
		frequency = 1;
	}
	
	
	printDBG("Hi F Time\t%lli\n", activeTimeHi/2);

	printDBG("Lo F Time\t%lli\n", activeTimeLo/2);	
	
	idleTimeTotal = idleTimeTotal + idleTime;
	printDBG("Idle Time\t%lli\n", idleTimeTotal);
	
	// The retun value can be specified like this:
	TaskSelection sel;
	sel.task = priority[0]; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
	sel.freq = frequency; // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)

    return sel;
}