#ifndef SHORTEST_JOB_FIRST_ 
#define SHORTEST_JOB_FIRST_
#include<cstdlib>
#include"Processors/Processor.h"
#include"Datastructures/LinkedPriorityQueue.h"
class ShortestJobFirst : public Processor
{
	LinkedPriorityQueue<Process*> RDY_LIST;
	int timeslice;
public:
	ShortestJobFirst(Scheduler* sch, int id, int ts);

	virtual void printReadyList();

	virtual void AddProccess(Process* add_process);

	virtual void ScheduleAlgo();

	virtual int ExpectedFinishTime();
};





#endif
