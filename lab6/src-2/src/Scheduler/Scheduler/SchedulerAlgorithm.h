/*
 * SchedulerAlgorithm.h
 *
 *  Created on: Mar 8, 2016
 *      Author: vens
 */

#ifndef SCHEDULERALGORITHM_H_
#define SCHEDULERALGORITHM_H_

#include <string>

#include "Scheduler.h"

#include "include/scheduler_algorithm.h"



class SchedulerAlgorithm {
	friend class Scheduler;

public:
	SchedulerAlgorithm(std::string name, const struct scheduler_operations * sops);
	virtual ~SchedulerAlgorithm();

	/// Called every time a task is able to be run
	struct task * EnqueueTask(struct task * running_task, struct task * new_task);
	/// Called every time the running task is no longer available to run
	struct task * DequeueTask(struct task * running_task);
	/// Called every system tick
	struct task * OnTick(struct task * running_task);

private:
	const std::string name;
	const struct scheduler_operations * sops;
	unsigned long ticks;

	int handle;
};


#endif /* SCHEDULERALGORITHM_H_ */
