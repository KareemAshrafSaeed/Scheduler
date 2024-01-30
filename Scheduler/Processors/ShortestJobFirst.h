#ifndef SHORTEST_JOB_FIRST_ 
#define SHORTEST_JOB_FIRST_
#include<cstdlib>
#include"Processor.h"
#include"../Datastructures/LinkedPriorityQueue.h"
class ShortestJobFirst : public Processor
{
	LinkedPriorityQueue<Process*> RDY_LIST;
	int timeslice;
	int processes_count; // processes count
public:
	ShortestJobFirst(Scheduler* sch, int id, int ts);

	virtual void printReadyList();

	virtual string getType();

	virtual  void get_delete_top(Process*& toGet);

	virtual void AddProccess(Process* add_process);

	int getprocesses_count();

	virtual void ScheduleAlgo();

	void runNextProcess();

	virtual bool Kill_Signal(int idkilled);

	virtual bool Kill_Orphan(Process* p);
};





#endif
