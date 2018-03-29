/*
 * Simulator.cpp
 *
 *  Created on: Mar 10, 2016
 *      Author: vens
 */

#include <iostream>
#include "SimTask.h"
#include "Scheduler/Scheduler.h"
#include "Simulator.h"
#include "Debug.h"

Simulator::Simulator()
	: remaining_tasks(0)
{
	// TODO Auto-generated constructor stub
	this->wave_running = this->wave_root.AddSignal("Running");
	this->running_task = 0;
	this->num_tasks = 0;
	this->sys_time = 0; 
}

Simulator::~Simulator() {
	// TODO Auto-generated destructor stub
	for(auto it=this->sim_task_list.begin(); it != this->sim_task_list.end();++it)
	{
		delete *it;
	}

	for(auto it=this->finished_list.begin(); it != this->finished_list.end();++it)
	{
		delete *it;
	}
}

void Simulator::MakeTaskList(json_value * array)
{
	this->num_tasks = array->u.array.length;
	for(unsigned int x=0;x<this->num_tasks;x++)
	{
		SimTask * task = new SimTask(array->u.array.values[x], &this->wave_root);
		this->sim_task_list.push_back(task);
	}
	this->remaining_tasks = this->num_tasks;

	this->sim_task_list.sort();

}

bool Simulator::IsDone()
{
	return this->remaining_tasks == 0;
}

void Simulator::RunTick()
{
	std::list<SimTask*>::iterator it;
	static SimTask* running_task = 0;
	SimTask* last_running_task = running_task;
	running_task = 0;

	// run the OnStartTick for each task in the system
	for(it=this->sim_task_list.begin(); it != this->sim_task_list.end();++it)
	{
		SimTask* this_task = *it;
		this_task->OnStartTick(this->sys_time);

	}

	// run the scheduler tick
	Scheduler::OnSysTick(this->sys_time);

	// advance the sys time to show the end of this tick
	this->sys_time ++;

	// run the system tick on every task in the system
	for(it=this->sim_task_list.begin(); it != this->sim_task_list.end();++it)
	{
		SimTask* this_task = *it;
		// if this is the task that ran this tick
		if(this_task->IsRunning())
		{
			//std::cout << "Running = " << this_task->GetName() << std::endl;
			running_task = this_task;
		}
		this_task->OnSysTick(this->sys_time);
	}

	// run the system tick on every task that has finished
	for(it=this->finished_list.begin(); it != this->finished_list.end(); ++it)
	{
		SimTask* this_task = *it;
		this_task->OnSysTick(sys_time);
	}

	// run the on end tick for every task in the system
	for(it=this->sim_task_list.begin(); it != this->sim_task_list.end();)
	{
		SimTask* this_task = *it;
		this_task->OnEndTick(this->sys_time);
		if(this_task->IsFinished())
		{
			it = this->sim_task_list.erase(it);
			this->finished_list.push_back(this_task);
			this->remaining_tasks --;
			//std::cout << "Finished " << this_task->GetName() << "Remaining=" << this->remaining_tasks << std::endl;
		}
		else
		{
			++it;
		}
	}

	// print the running task info to the output wave
	if(running_task == 0)
	{
		this->wave_running->AddNode(wavedrom::NODE::Z);
	}
	else if(running_task == last_running_task)
	{
		this->wave_running->ContinueNode();
	}
	else if(running_task)
	{
		wavedrom::NODE::node_data_type color;
		switch(running_task->GetPriority())
		{
		case 0:
			// low prioirty
			color = wavedrom::NODE::WHITE;
			break;
		case 1:
			color = wavedrom::NODE::BLUE;
			break;
		case 2:
			color = wavedrom::NODE::YELLOW;
			break;
		case 3:
		default:
			color = wavedrom::NODE::RED;
			break;
		}
		this->wave_running->AddNode(color, running_task->GetName().c_str());
	}
}

void Simulator::PrintLogData()
{
	this->finished_list.sort();
	for(auto it = finished_list.begin(); it != finished_list.end(); ++it)
	{
		Debug::AppendLog((*it)->GetLogString());
	}
}

void Simulator::ExportWaveform(std::ofstream& out, bool include_legend)
{
	if(include_legend)
	{
		wavedrom::Group* leg = this->wave_root.AddGroup("Legend");
		wavedrom::Signal* sig;
		sig = leg->AddSignal("Running");
		sig->AddNode(wavedrom::NODE::WHITE, "Priority 0");
		sig->ContinueNode(5);
		sig = leg->AddSignal("Running");
		sig->AddNode(wavedrom::NODE::BLUE, "Priority 1");
		sig->ContinueNode(5);
		sig = leg->AddSignal("Running");
		sig->AddNode(wavedrom::NODE::YELLOW, "Priority 2");
		sig->ContinueNode(5);
		sig = leg->AddSignal("Running");
		sig->AddNode(wavedrom::NODE::RED, "Priority 3");
		sig->ContinueNode(5);
		sig = leg->AddSignal("Blocked");
		sig->AddNode(wavedrom::NODE::X);
		sig->ContinueNode(5);
		sig = leg->AddSignal("Ready");
		sig->AddNode(wavedrom::NODE::Z);
		sig->ContinueNode(5);
		sig = leg->AddSignal("Not Arrived");
		sig->AddNode(wavedrom::NODE::DOWN);
		sig->ContinueNode(5);
		sig = leg->AddSignal("Finished");
		sig->AddNode(wavedrom::NODE::UP);
		sig->ContinueNode(5);
	}

	char * buf = this->wave_root.Export();
	out << std::string(buf);
	delete[] buf;
}
