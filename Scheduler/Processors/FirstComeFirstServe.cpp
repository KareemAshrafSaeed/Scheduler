#include "FirstComeFirstServe.h"
#include "..\Scheduler.h"
#include "..\Datastructures\Node.h"
FirstComeFirstServe::FirstComeFirstServe(Scheduler* sch, int id, int maxw) : Processor(sch, id) {
	MaxW = maxw;
}

string FirstComeFirstServe::getType() {
	return "FCFS";
}

void FirstComeFirstServe::printReadyList() {
	RDY_LIST.Print();
}

void FirstComeFirstServe::AddProccess(Process* add_process) {
	RDY_LIST.InsertEnd(add_process);
	expected_finish_time += add_process->getRemainingCT();
}

void FirstComeFirstServe::ScheduleAlgo() {

	if (!running_process) {
		runNextProcess();
	}
	else {
		// increment processor busytime
		busy_time++;
		running_process->execute();
		// set first runtime 
		// if it was set before it won't overwrite it
		running_process->setFirstRuntime(scheduler->getTimeStep());
	}
}

int FirstComeFirstServe::getprocesses_count() { return RDY_LIST.getcount(); }

void FirstComeFirstServe::runNextProcess() {
	if (RDY_LIST.gethead()) {
		running_process = RDY_LIST.gethead()->getItem();
		RDY_LIST.DeleteFirst();
		expected_finish_time -= running_process->getRemainingCT();
	}
	else { running_process = nullptr; }
}


bool FirstComeFirstServe::Kill_Signal(int id) {
	if (id == running_process->getID()) {
		scheduler->addtoTrmList(running_process);
		running_process = nullptr;
		return true;
	}
	else {
		Node<Process*>* temp = RDY_LIST.gethead();
		while (temp) {
			if (temp->getItem()->getID() == id) {
				scheduler->addtoTrmList(temp->getItem());
				RDY_LIST.deleteNodewithptr(temp);
				return true;
			}
			temp = temp->getNext();
		}
	}
	return false;
}

void FirstComeFirstServe::migrate() {
	while (running_process && WT() > MaxW && !running_process->getParent()) {
		if (scheduler->migrateToRR(running_process)) {
			runNextProcess();
		}
		else {
			// migration failed because there is no RR processors
			// so just break out of the loop
			break;
		}
	}
}
int FirstComeFirstServe::WT() {
	if (running_process) {
		return (scheduler->getTimeStep() - running_process->getAT() - running_process->getexcuted());
	}
	return 0;
}


void FirstComeFirstServe::get_delete_top(Process*& toGet)
{
	Node<Process*>* ptr = RDY_LIST.gethead();
	bool state = true;
	while (ptr && state)
	{
		if (!ptr->getItem()->getParent())
		{
			state = false;
			toGet = ptr->getItem();

		}
		else 
		{
			ptr = ptr->getNext();
		}
	}
	if(ptr)
	  RDY_LIST.deleteNodewithptr(ptr);

}

bool FirstComeFirstServe::Kill_Orphan(Process* child) {

	if (running_process && child->getID() == running_process->getID()) {
		scheduler->addtoTrmList(running_process);
		running_process = nullptr;
		return true;
	}
	else {

		Node<Process*>* temp = RDY_LIST.gethead();
		while (temp) {
			if (temp->getItem()->getID() == child->getID()) {
				scheduler->addtoTrmList(temp->getItem());
				RDY_LIST.deleteNodewithptr(temp);
				return true;
			}
			temp = temp->getNext();
		}
	}
	return false;

}