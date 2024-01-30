#ifndef PROCESS_
#define PROCESS_
#include <iostream>
#include <fstream>
#include"Datastructures/LinkedQueue.h"
#include"Datastructures/pair.h"
using namespace std;

struct IO {
	int R;
	int D;
};

class Process
{
private:
	Process* Child_1;
	Process* Child_2;
	Process* Parent;
	LinkedQueue<IO*>IO_LIST;
	int PID, AT, CT, TT;
	int Total_IO_D;
	int first_run_time;
	bool is_runningIO;
	
	int remaining_IO;
	// remaining timesteps to finish io 
	// if the process in blk list the 

	int N; // # of time the process requests I/O
	int executed_ts; // the executed timesteps so far
public:
	Process(int arrival, int Id, int ct, int num) {
		Child_1 = nullptr;
		Child_2 = nullptr;
		Parent = nullptr;
		executed_ts = 0;
		first_run_time = 0;
		AT = arrival;       PID = Id;   CT = ct;   N = num;
		TT = 0;
		Total_IO_D = 0;
		remaining_IO = 0;
		is_runningIO = false;
	}

	bool wantsIO() {
		IO * io = nullptr;
		IO_LIST.peek(io);
		if (!io) { return false; }
		return io->R == executed_ts;
	}

	void startIO() {
		if (!is_runningIO) {
			IO* io = nullptr;
			IO_LIST.peek(io);
			remaining_IO = io->D;
			is_runningIO = true;
		}
	}

	void endIO() {
		
		IO* io = nullptr;

		IO_LIST.dequeue(io);

		Total_IO_D += io->D;

		is_runningIO = false;

		delete io;
	}

	int getRemainingCT() {
		return CT - executed_ts;
	}

	int getRemainingIO() {
		return remaining_IO;
	}

	void executeIO() {
		remaining_IO--;
	}


	void setArrivalTime(int at) {
		AT = at;
	}

	void setFirstRuntime(int frt) {
		// condition to set the first run time one time only
		if (first_run_time==0) { 
			first_run_time = frt;
		}
	}

	int getRT() {
		return first_run_time - AT;
		//return ((first_run_time - AT) > 0 ? (first_run_time - AT) : 0);
	}
	int getTRT() {
		return TT - AT;
	}

	int getWT() {
		return getTRT() - CT;
		//return ((getTRT() - CT)>0? (getTRT() - CT): getTRT());
	}

	int getTT() {
		return TT;
	}

	int getTotalIo_D() {
		return Total_IO_D;
	}

	bool isFinished() {
		return executed_ts == CT;
	}

	int getArrivalTime() {
		return AT;
	}
	void SetIO(int ior, int iod) {
		IO * io = new IO;
		io->R = ior;
		io->D = iod;
		IO_LIST.enqueue(io);
	}

	Process*& getChild1() {
		return Child_1;
	}
	Process*& getChild2() {
		return Child_2;
	}


	Process* getParent() {
		return Parent;
	}

	void setParent(Process* P)
	{
		Parent = P;
	}
	void setChild1(Process* P)
	{
		Child_1 = P;
	}
	void setChild2(Process* P)
	{
		Child_2 = P;
	}
	int getID() { return PID; }

	void SetTT(int ter) {
		TT = ter;
	}

	void execute() {
		executed_ts++;
	}
	void Printprocess(ofstream& File) {
		File << TT << '\t'
			<< PID << '\t'
			<< AT << '\t'
			<< CT << '\t'
			<< Total_IO_D << '\t'
			<< getWT() << '\t'
			<< getRT() << '\t'
			<< getTRT() << '\n';
	}

	friend ostream& operator<<(ostream& os, const Process& p)
	{
		os << p.PID;
		return os;
	}

	//ForPiririty in PriorityQueue
	int getCT() { return CT; }

	int getexcuted() { return executed_ts; }



	int getAT() { return AT; }

	~Process() {
		IO* io;
		while (IO_LIST.dequeue(io)) {
			delete io;
		}
	}

	bool operator == (const Process& p) {
		return PID == p.PID;
	}

};
#endif