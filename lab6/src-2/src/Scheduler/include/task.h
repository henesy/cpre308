/*
 * task.h
 *
 *  Created on: Mar 10, 2016
 *      Author: vens
 */

#ifndef TASK_H
#define TASK_H


#ifdef __cplusplus
extern "C"{
#endif

struct task_info
{
	void* parrent;

	char name[128];

	/// 3= Higest priority, 0= Lowest priority
	unsigned char priority;

	/// The number of cycles the task has run on the CPU
	unsigned long run_time;

	/// The number of cycles the task has been blocked
	unsigned long block_time;

	/// The time the task was first created
	unsigned long time_arive;

	/// The time the task must be completely finished by
	unsigned long deadline;
};

struct task
{
	struct task *next;
	struct task *prev;

	const struct task_info * task_info;

	/// Called whenever the task is being destroyed to clean up
	/// any resources created by the scheduler
	//void (*cleanup)(struct task* t);
	/// A pointer that the scheduler can use to store needed information
	/// about this task.
	void * scheduler_data;
};

#ifdef __cplusplus
}
#endif

#endif /* TASK_H_ */
