#ifndef _ASSIGNMENT2_BODY_
#define _ASSIGNMENT2_BODY_

#include "scheduler.h"

struct shared_variable; // Defined in assignment1.h

// Call at the start part of the program before actual scheduling
void learn_workloads(struct shared_variable* sv);

// Call in the scheduler thread
TaskSelection select_task(struct shared_variable* sv, const int* aliveTasks, long long idleTime);

#endif
