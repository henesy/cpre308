/*
 * SchedulerAlgorithm.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: vens
 */

#include "SchedulerAlgorithm.h"

SchedulerAlgorithm::SchedulerAlgorithm(std::string _name, const struct scheduler_operations * _sops)
	: name(_name), sops(_sops), ticks(0), handle(0)
{

}

SchedulerAlgorithm::~SchedulerAlgorithm() {
	// TODO Auto-generated destructor stub
}

struct task * SchedulerAlgorithm::EnqueueTask(struct task * running_task, struct task * new_task)
{
	if(this->sops->task_enqueue == 0)
		return running_task;
	struct task * t = this->sops->task_enqueue(running_task, new_task);
	if(t && t != running_task)
		this->ticks = 0;
	return t;
}

struct task * SchedulerAlgorithm::DequeueTask(struct task * running_task)
{
	if(this->sops->task_dequeue == 0)
		return running_task;
	struct task * t = this->sops->task_dequeue(running_task);
	if(t)
		this->ticks = 0;
	return t;
}

struct task * SchedulerAlgorithm::OnTick(struct task * running_task)
{
	if(this->sops->periodic_timer == 0)
		return running_task;
	this->ticks ++;
	if(this->sops->period > 0 && this->sops->period == this->ticks)
	{
		this->ticks = 0;
		return this->sops->periodic_timer(running_task);
	}
	return running_task;
}
