#ifndef SCHEDULER_
#define SCHEDULER_
#include<fstream>
#include <iomanip>
#include<iostream>
#include <Windows.h>
#include"Process.h"
#include "Processors/RoundRobin.h"
#include "Processors/FirstComeFirstServe.h"
#include "Processors/ShortestJobFirst.h"
#include"Datastructures/LinkedQueue.h"
#include"Datastructures/LinkedList.h"
#include"UI.h"
class UI;
using namespace std;

struct kill_signal {
	int pid;
	int time;
};

class Scheduler
{
	int NF; // number of fcfs
	int NS; // number of sjf
	int NR; // number of round robin
	int STL; // work stealing time
	int migrated_rtf;
	int migrated_maxw;
	int forked_processes;
	int stealed_processes;
	int killed_processes;
	int ForkProb; // probability of process fork
	int NProcess;
	int timestep;
	LinkedQueue<Process*> newList;
	Processor * Processors[1000];
	int processors_count;
	LinkedQueue<kill_signal*> SIGKILL_LIST;
	LinkedQueue<Process*> BLK_LIST;
	LinkedQueue<Process*> TRM_LIST;
	UI * UI_IO;
	Processor * getProcessorWithShortestQueue(string type = "");
	Processor* getProcessorWithLongestQueue(string type = "");
public:
	Scheduler();
	~Scheduler();
	void sendToProccessor(Process* p);

	void handleProcessArrival();

	void KillOrphan(Process* p);

	void schedule();

	void FullSchedule();

	void addtoBlkList(Process* p);

	void addtoTrmList(Process* p);

	bool migrateToSJF(Process* p);

	bool migrateToRR(Process* p);

	void handleKillProcesses();

	void handleForking();

	bool Fork(Processor& P);
		
	void Steal();

	void executeProcesses();

	void handleMigration();

	void checkProcessIO();

	void moveRunToTrm();

	void handleBlockList();

	void ReadFile();

	void Print();

	void generateOutputFile();

	int getTimeStep();
	int getProcessesCount();
	int getTRM_LIST_COUNT();
};
#endif