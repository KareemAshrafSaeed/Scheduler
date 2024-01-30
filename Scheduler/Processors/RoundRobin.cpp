#
#include "RoundRobin.h"
#include "..\Scheduler.h"


RoundRobin::RoundRobin(Scheduler* sch, int id, int ts, int rtf) : Processor(sch, id) {
	timeslice = ts;
	remaining_ts = ts;
	RTF = rtf;
}

string RoundRobin::getType() {
	return "RR";
}

void RoundRobin::printReadyList() {
	RDY_LIST.Print();
}

void RoundRobin::AddProccess(Process* add_process) {
	RDY_LIST.enqueue(add_process);
	expected_finish_time += add_process->getRemainingCT();
}

int RoundRobin::getprocesses_count() { return RDY_LIST.getcount(); }

void RoundRobin::ScheduleAlgo() {
		if (!running_process) {
			runNextProcess();
		}
		else {
			if(running_process->isFinished()) {
				return;
			}
			// increment processor busytime
			busy_time++;
			running_process->execute();
			// set first runtime 
			// if it was set before it won't overwrite it
			running_process->setFirstRuntime(scheduler->getTimeStep());
			// decrement the remaining timesteps for current time slice
			remaining_ts--;
			if (remaining_ts == 0) {
				// send the current process to the end of ready list
				AddProccess(running_process);
				// run the next process
				runNextProcess();
			}
		}
}

void RoundRobin::migrate() {
	while (running_process && running_process->getRemainingCT() < RTF && !running_process->getParent()) {
		if (scheduler->migrateToSJF(running_process)) {
			runNextProcess();
		}
		else {
			// migration failed because there is no SJF processors
			// so just break out of the loop
			break;
		}
	}
}

void RoundRobin::runNextProcess() {
	// reset the remaining time slice
	remaining_ts = timeslice;
	if (!RDY_LIST.dequeue(running_process)) {
		// if the list is empty set running process to null
		running_process = nullptr;
	}
	else {
		expected_finish_time -= running_process->getRemainingCT();
	}
	
}

bool RoundRobin::Kill_Signal(int idkilled) { return true; }

void RoundRobin::get_delete_top(Process*& toGet)
{
	RDY_LIST.dequeue(toGet);
}

bool RoundRobin::Kill_Orphan(Process* p) { return false; }