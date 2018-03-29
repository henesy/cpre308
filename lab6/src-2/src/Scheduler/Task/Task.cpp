/*
 * Task.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: vens
 */

//#include "Kernel.h"
#include <stdlib.h>
#include "Task.h"

//Task Task::idle_task("Idle", 0);



Task::Task() {
	this->task_info.block_time = 0;
	this->task_info.run_time = 0;
	//this->task_info.state = task_info::TASK_READY;
	this->task_info.parrent = (void*) this;
	this->task_info.deadline = 0;

	this->task.task_info = &this->task_info;
	this->task.next = 0;
	this->task.prev = 0;
	this->task.scheduler_data = 0;
}

Task::~Task() {
	if(this->task.scheduler_data)
	{
		free(this->task.scheduler_data);
	}
}

void Task::OnArrive(unsigned long sys_time)
{
	this->task_info.time_arive = sys_time;
}

void Task::SetPriority(int p)
{
	this->task_info.priority = p;
}

void Task::SetName(std::string name)
{
	std::size_t len = name.copy(this->task_info.name, 128, 0);
	this->task_info.name[len] = '\0';
}

void Task::SetDeadline(unsigned long t)
{
	this->task_info.deadline = t;
}


