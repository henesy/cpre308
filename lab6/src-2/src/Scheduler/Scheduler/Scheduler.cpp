/*
 * Scheduler.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: vens
 */

//#include "Kernel.h"

#include "Task/Task.h"
#include "Scheduler.h"
#include "SchedulerAlgorithm.h"
//#include "Task.h"

#include "include/scheduler_algorithm.h"

#ifdef __cplusplus
extern "C"{
#endif

int register_scheduler(const char * name, const struct scheduler_operations * sops)
{
	SchedulerAlgorithm *alg = new SchedulerAlgorithm(name, sops);
	return Scheduler::RegisterAlgorithm(alg);
	//Kernel::GetCurrentKernel()->GetScheduler()->RegisterAlgorithm(alg);
}


int unregister_scheduler(int handle)
{
	return Scheduler::UnregisterAlgorithm(handle);
	return 0;
}


#ifdef __cplusplus
}
#endif

SchedulerAlgorithm* Scheduler::algorithm = 0;
Task* Scheduler::running_task = 0;

int Scheduler::RegisterAlgorithm(SchedulerAlgorithm *algorithm)
{
	static int handle = 1;
	algorithm->handle = handle++;
	Scheduler::algorithm = algorithm;
	return algorithm->handle;
}

int Scheduler::UnregisterAlgorithm(int handle)
{
	delete Scheduler::algorithm;
	Scheduler::algorithm = 0;
	/*for(std::vector<SchedulerAlgorithm*>::iterator it = Scheduler::algorithm_list.begin(); it != Scheduler::algorithm_list.end(); ++it)
	{
		if((*it)->handle == handle)
		{
			Scheduler::algorithm_list.erase(it);
			return 0;
		}
	}*/


	return 0;
}

void Scheduler::AddTask(Task* task, unsigned long sys_time)
{
	// TODO handle idle task being returned
	struct task* new_task;
	new_task = Scheduler::algorithm->EnqueueTask(&Scheduler::running_task->task, &task->task);
	if(new_task != &Scheduler::running_task->task)
	{
		if(Scheduler::running_task)
			Scheduler::running_task->SwapOut(sys_time);
		if(new_task)
		{
			Scheduler::running_task = (Task*)new_task->task_info->parrent;
			Scheduler::running_task->SwapIn(sys_time);
		}
		else
		{
			Scheduler::running_task = 0;
		}
	}
}

void Scheduler::RemoveTask(unsigned long sys_time)
{
	struct task* new_task;
	Scheduler::running_task->SwapOut(sys_time);
	new_task = Scheduler::algorithm->DequeueTask(&Scheduler::running_task->task);
	if(new_task)
	{
		Scheduler::running_task = (Task*)new_task->task_info->parrent;
		Scheduler::running_task->SwapIn(sys_time);
	}
	else
	{
		Scheduler::running_task = 0;
	}
}

void Scheduler::OnSysTick(unsigned long sys_time)
{
	struct task* new_task;
	new_task = Scheduler::algorithm->OnTick(&Scheduler::running_task->task);
	if(new_task != &Scheduler::running_task->task)
	{
		if(Scheduler::running_task)
			Scheduler::running_task->SwapOut(sys_time);
		if(new_task)
		{
			Scheduler::running_task = (Task*)new_task->task_info->parrent;
			Scheduler::running_task->SwapIn(sys_time);
		}
		else
		{
			Scheduler::running_task = 0;
		}
	}
}

/*Scheduler::Scheduler() {
	// TODO Auto-generated constructor stub

}

Scheduler::~Scheduler() {
	// TODO Auto-generated destructor stub
}*/

