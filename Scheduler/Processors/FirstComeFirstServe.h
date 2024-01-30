#ifndef FIRST_COME_FIRST_SERVE_
#define FIRST_COME_FIRST_SERVE_
#include <cstdlib>
#include "Processor.h"
#include "..\Datastructures\LinkedList.h"
class FirstComeFirstServe :public Processor {
	LinkedList<Process*> RDY_LIST;

	int processes_count; // processes count
	int MaxW;
public:
	FirstComeFirstServe(Scheduler* sch, int id, int maxw);
	virtual void printReadyList();

	virtual void AddProccess(Process* add_process);

	virtual string getType();

	virtual void ScheduleAlgo();

	int getprocesses_count();

	void runNextProcess();

	virtual bool Kill_Signal(int idkilled);

	virtual void get_delete_top(Process*& toGet);

	void migrate();

	int WT();

	virtual bool Kill_Orphan(Process* p);

};

#endif