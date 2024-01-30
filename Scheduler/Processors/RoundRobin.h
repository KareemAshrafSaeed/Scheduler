#ifndef ROUND_ROBIN_
#define ROUND_ROBIN_
#include <cstdlib>
#include "Processor.h"
#include "..\Datastructures\LinkedCircularQueue.h"
class RoundRobin :public Processor {
	LinkedCircularQueue<Process*> RDY_LIST;
	int RTF; // migration threshold
	int timeslice; // the time slice for each process
	int remaining_ts;  // the remaining time for current time slice
public:
	RoundRobin(Scheduler* sch, int id, int ts, int rtf);

	virtual void printReadyList();

	virtual string getType();

	virtual void AddProccess(Process* add_process);

	void runNextProcess();

	virtual void ScheduleAlgo();

	void migrate();

	int getprocesses_count();

	virtual bool Kill_Signal(int idkilled);
	virtual void get_delete_top(Process*& toGet);
	virtual bool Kill_Orphan(Process* p);

};

#endif