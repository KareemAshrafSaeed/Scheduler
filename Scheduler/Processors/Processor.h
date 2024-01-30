#ifndef PROCESSOR_
#define PROCESSOR_


#include "..\Process.h"

class Scheduler;

enum PROCESSOR_STATE{
	IDLE,
	BUSY
};
class Processor {
protected:
	Process* running_process;
	Scheduler * scheduler; // pointer to scheduler class to send processes to blk list and trm list
	int id;
	int busy_time;
	int expected_finish_time;
public:
	Processor(Scheduler * sch,int i) {
		id = i;
		running_process = nullptr;
		scheduler = sch;
		busy_time = 0;
		expected_finish_time = 0;
	}
	void printInfo() { 
		cout << "processor " << id <<" [" << getType() <<"]" <<" : ";
	}
	// returns the processor current state (busy or idle)
	PROCESSOR_STATE getState() {
		return running_process ? BUSY : IDLE; 
	}

	int getID() const
	{ 
		return id;
	}
	virtual int getprocesses_count()=0;
	virtual string getType() = 0;

	virtual void printReadyList() = 0;

	Process* getRunningProcess() {
		return running_process;
	}

	virtual void runNextProcess() = 0;

	virtual void migrate() {}

	virtual bool Kill_Signal(int idkilled) {
		return true;
	};



	virtual void get_delete_top(Process*& toGet) = 0; //Used for steal, different in FCFS

	virtual bool KillProccess(Process* kill_process) {
		return true;
	}
	virtual void AddProccess(Process* add_process) = 0;

	virtual void ScheduleAlgo() = 0;

	float ExpectedFinishTime() {
		return expected_finish_time;
	}

	int getBusyTime() {
		return busy_time;
	}

	virtual ~Processor() {
		
	}

	virtual bool Kill_Orphan(Process* p)=0;
};
#endif