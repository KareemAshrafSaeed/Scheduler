#include"ShortestJobFirst.h"
#include"../Scheduler.h"
ShortestJobFirst::ShortestJobFirst(Scheduler* sch, int id, int ts) : Processor(sch, id) 
{
	timeslice = ts;
}

string ShortestJobFirst::getType() {
	return "SJF";
}

void ShortestJobFirst::printReadyList()
{
	RDY_LIST.Print();
}

void ShortestJobFirst::AddProccess(Process* add_process) {
	int Priority = 1000000 - add_process->getCT();
	RDY_LIST.enqueue(add_process,Priority);
	expected_finish_time += add_process->getRemainingCT();
}

int ShortestJobFirst::getprocesses_count() { return RDY_LIST.gatcount(); }

void ShortestJobFirst::ScheduleAlgo() 
{
	if (!running_process) 
	{
		runNextProcess();
	}
	else
	{
		if (running_process->isFinished()) {
			return;
		}
		busy_time++;
		running_process->execute();
		running_process->setFirstRuntime(scheduler->getTimeStep());
	}
}

void ShortestJobFirst::runNextProcess() {
	if (!RDY_LIST.dequeue(running_process)) {
		// if the list is empty set running process to null
		running_process = nullptr;
	}
	else {
		expected_finish_time -= running_process->getRemainingCT();
	}
}

bool ShortestJobFirst::Kill_Signal(int idkilled) { return true; }

void ShortestJobFirst::get_delete_top(Process*& toGet)
{
	RDY_LIST.dequeue(toGet);
}

bool ShortestJobFirst::Kill_Orphan(Process* p) { return false; }