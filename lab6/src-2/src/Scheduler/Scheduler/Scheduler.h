/*
 * Scheduler.h
 *
 *  Created on: Mar 8, 2016
 *      Author: vens
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <vector>


class SchedulerAlgorithm;
class Task;

class Scheduler {
	//friend class SchedulerAlgorithm;

public:

	/**
	 * Register a new scheduling algorithm with the scheduler.
	 * @param      algorithm
	 *                  The algorithm to use for scheduling tasks
	 * @return     0 on success, negative number on failure
	 */
	static int RegisterAlgorithm(SchedulerAlgorithm *algorithm);
	/// @todo      Remove this	
	static int UnregisterAlgorithm(int handle);

	/**
	 * Called anytime a new task arrives in the system
     * @param      task
     *                  The new task that just arrived
     * @param      sys_time
     *                  The system time the task is arriving
	 */
	static void AddTask(Task* task, unsigned long sys_time);
	/**
	 * Called when the currently running task no longer wants to run anymore
	 * @param      sys_time
	 *                  The system time the task is requesting to stop running
	 */	
	static void RemoveTask(unsigned long sys_time);
	/**
	 * Called by the system on every system tick
	 * @param      sys_time
	 *                  The system time when the callback is being called
	 */	
	static void OnSysTick(unsigned long sys_time);

private:
	/**
	 * The scheduling algorithm being used
	 */
	static SchedulerAlgorithm* algorithm;

	/**
	 * The currently running task on the system
	 */
	static Task* running_task;
};

#endif /* SCHEDULER_H_ */
