#include"ShortestJobFirst.h"
#include"Scheduler.h"
ShortestJobFirst::ShortestJobFirst(Scheduler* sch, int id, int ts) : Processor(sch, id) 
{
	timeslice = ts;
	type = SJF;
}

void ShortestJobFirst::printReadyList()
{
	RDY_LIST.Print();
}

void ShortestJobFirst::AddProccess(Process* add_process) {
	int Priority = 1000000 - add_process->getCT();
	RDY_LIST.enqueue(add_process,Priority);
}
void ShortestJobFirst::ScheduleAlgo() {


	if (!running_process) {
		RDY_LIST.dequeue(running_process);
	}
	else {
		int random = rand() % 100;
		if (random >= 1 && random <= 15) {
			scheduler->addtoBlkList(running_process);
			if (!RDY_LIST.dequeue(running_process)) {
				running_process = nullptr;
			}
		}

		if (random >= 20 && random <= 30) {
			scheduler->sendToProccessor(running_process);
			if (!RDY_LIST.dequeue(running_process)) {
				running_process = nullptr;
			}
		}

		if (random >= 50 && random <= 60) {
			scheduler->addtoTrmList(running_process);
			if (!RDY_LIST.dequeue(running_process)) {
				running_process = nullptr;
			}
		}

	}
}
int ShortestJobFirst::ExpectedFinishTime() {
	return 0;
}