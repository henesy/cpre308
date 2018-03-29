/*
 * scheduler_algorithm.h
 *
 *  Created on: Mar 8, 2016
 *      Author: vens
 */

#ifndef SCHEDULER_ALGORITHM_H
#define SCHEDULER_ALGORITHM_H

#ifdef __cplusplus
extern "C"{
#endif


struct scheduler_operations
{
	/// Called when a new task arrives in the system.
	/// @param r The currently running task
	/// @param t The new task
	/// @returns The task to run next
	struct task * (*task_enqueue)(struct task* r, struct task* t);
	/// Called when the running task no longer will require any system time
	/// becuse it was blocked or is finished
	/// @param r The currently running task
	struct task * (*task_dequeue)(struct task* r);
	/// Called periodically by the system.
	/// @param r The currently running task
	/// @param t The current system time in ticks
	struct task * (*periodic_timer)(struct task* r);
	/// The period of the calls to the timer method
	unsigned long period;
};

int register_scheduler(const char * name, const struct scheduler_operations * sops);
int unregister_scheduler(int handle);

#ifdef __cplusplus
};
#endif

#endif /* SCHEDULER_ALGORITHM_H_ */
