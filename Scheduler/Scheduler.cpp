#include"Scheduler.h"
Scheduler::Scheduler() {
	timestep = 0;
	srand(time(NULL));
	stealed_processes = 0;
	forked_processes = 0;
	migrated_maxw = 0;
	migrated_rtf = 0;
	killed_processes = 0;
	processors_count = 0;
	UI_IO = new UI(this);
}


Scheduler::~Scheduler() {
	// delete all processes
	Process* p;
	while (TRM_LIST.dequeue(p)) {
		delete p;
	}
	// delete remaining kill signals
	kill_signal* k;
	while (SIGKILL_LIST.dequeue(k)) {
		delete k;
	}
	// delete processors
	for (int i = 0; i < processors_count; i++)
	{
		delete Processors[i];
	}
}


void Scheduler::schedule() {

	timestep++;

	handleProcessArrival();

	checkProcessIO();

	handleBlockList();

	moveRunToTrm();

    handleMigration();

	Steal();

	handleForking();

	handleKillProcesses();

	executeProcesses();
}


void Scheduler::sendToProccessor(Process* p) {
	if (!processors_count) {
		return;
	}
	//gets the processor with shortestReadyQueue
	Processor* processor = getProcessorWithShortestQueue();
	processor->AddProccess(p);
}

// Function to get the processor with the shortest queue
Processor  * Scheduler::getProcessorWithShortestQueue(string type)
{
	Processor* min = nullptr;
	if (type == "") {
		min = Processors[0];
	}
	else {
		for (int i = 0; i < processors_count; i++)
		{
			if (Processors[i]->getType() == type) {
				min = Processors[i];
				break;
			}
		}
	}
	if (!min) {
		return nullptr;
	}
	
	for (int i = 1; i < processors_count; i++) {
		
		if (Processors[i]->ExpectedFinishTime() < min->ExpectedFinishTime()) {
			if (type == "" || type == Processors[i]->getType()) {
				min = Processors[i];
			} else {
				continue;
			}
		}
		
	}
	return min;
}


// Function to get the processor with the longest queue
Processor* Scheduler::getProcessorWithLongestQueue(string type)
{
	Processor* max = nullptr;
	if (type == "") {
		max = Processors[0];
	}
	else {
		for (int i = 0; i < processors_count; i++)
		{
			if (Processors[i]->getType() == type) {
				max = Processors[i];
				break;
			}
		}
	}
	if (!max) {
		return nullptr;
	}

	for (int i = 1; i < processors_count; i++) {

		if (Processors[i]->ExpectedFinishTime() > max->ExpectedFinishTime()) {
			if (type == "" || type == Processors[i]->getType()) {
				max = Processors[i];
			}
			else {
				continue;
			}
		}

	}
	return max;
}

bool Scheduler::migrateToRR(Process* p) {
	//gets the RR processor with shortestReadyQueue
	Processor* processor = getProcessorWithShortestQueue("RR");
	if (!processor) {
		return false;
	}
	processor->AddProccess(p);
	migrated_maxw++;
	return true;
}

bool Scheduler::migrateToSJF(Process* p) {
	//gets the RR processor with shortestReadyQueue
	Processor* processor = getProcessorWithShortestQueue("SJF");
	if (!processor) {
		return false;
	}
	migrated_rtf++;
	processor->AddProccess(p);
	return true;
}




void Scheduler::handleMigration() {
	for (int i = 0; i < processors_count; i++)
	{
		if (Processors[i]->getType() == "RR" || Processors[i]->getType() == "FCFS") {
			Processors[i]->migrate();
		}
	}
}

void Scheduler::moveRunToTrm() {
	for (int i = 0; i < processors_count; i++)
	{
		if (Processors[i]->getRunningProcess()) {
			if (Processors[i]->getRunningProcess()->isFinished()) {
				// send the current process to block list
				addtoTrmList(Processors[i]->getRunningProcess());

				// run the next process
				Processors[i]->runNextProcess();
			}
		}
	}
}

void Scheduler::checkProcessIO() {
	for (int i = 0; i < processors_count; i++)
	{
		if (Processors[i]->getRunningProcess()) {
			if (Processors[i]->getRunningProcess()->wantsIO()) {
				// send the current process to block list
				addtoBlkList(Processors[i]->getRunningProcess());

				// run the next process
				Processors[i]->runNextProcess();
			}
		}
		
	}
}


void Scheduler::executeProcesses() {
	for (int i = 0; i < processors_count; i++)
	{
		Processors[i]->ScheduleAlgo();
	}
}

void Scheduler::handleKillProcesses() {
	kill_signal* killed = nullptr;
	SIGKILL_LIST.peek(killed);
	if (killed && killed->time == timestep) {
		for (int i = 0; i < processors_count; i++)
		{
			if (Processors[i]->getType() == "FCFS") {
				if (Processors[i]->Kill_Signal(killed->pid)) {
					killed_processes++;
					break;
				}
			}
		}
		SIGKILL_LIST.dequeue(killed);
		delete killed;
	}
	
}

void Scheduler::handleForking() {
	for (int i = 0; i < processors_count; i++)
	{
		if (Processors[i]->getType() == "FCFS")
		{
			Fork(*Processors[i]);
		}

	}
}

void Scheduler::handleProcessArrival() {
	Process* p;
	if (newList.peek(p)) {

		while (p->getArrivalTime() == timestep && !newList.isEmpty()) {

			newList.dequeue(p);

			sendToProccessor(p);

			newList.peek(p);

		}

	}
}

void Scheduler::handleBlockList() {
	Process* p_blocked;
	if (BLK_LIST.peek(p_blocked)) {
		p_blocked->startIO();
		p_blocked->executeIO();
		//  if it finished IO
		if (p_blocked->getRemainingIO() == 0) {
			p_blocked->endIO();
			// get it out of block list
			BLK_LIST.dequeue(p_blocked);
			// schedule it
			sendToProccessor(p_blocked);
			// start the next one
			if (BLK_LIST.peek(p_blocked)) {
				p_blocked->startIO();
			}
		}
	}
}

bool Scheduler::Fork(Processor& P)
{
	if (!P.getRunningProcess())
		return false;
	if (P.getRunningProcess()->getChild1()&& P.getRunningProcess()->getChild2())
		return false;
	
	int random = rand() % 100;
	if (!P.getRunningProcess()->getChild1()) {
		if (random <= ForkProb)
		{
			if (P.getRunningProcess())
			{
				//Process* child = P.getRunningProcess()->getChild();
				Process* child = new Process(timestep, ++NProcess, P.getRunningProcess()->getRemainingCT(), 0);
				Processor* Min = getProcessorWithShortestQueue("FCFS");
				if (Min == nullptr)
					return false;
				child->setParent(P.getRunningProcess());
				P.getRunningProcess()->setChild1(child);
				Min->AddProccess(child);
				forked_processes++;
				return true;
			}
			return false;
		}
	}else if(!P.getRunningProcess()->getChild2()) {
		if (random <= ForkProb)
		{
			if (P.getRunningProcess())
			{
				//Process* child = P.getRunningProcess()->getChild();
				Process* child = new Process(timestep, ++NProcess, P.getRunningProcess()->getRemainingCT(), 0);
				Processor* Min = getProcessorWithShortestQueue("FCFS");
				if (Min == nullptr)
					return false;
				child->setParent(P.getRunningProcess());
				P.getRunningProcess()->setChild2(child);
				Min->AddProccess(child);
				forked_processes++;
				return true;
			}
			return false;
		}

	}
	return false;
}

void Scheduler::Steal()
{
	if (timestep % STL == 0)
	{
		
		Processor* LQF = getProcessorWithLongestQueue();
		Processor* SQF = getProcessorWithShortestQueue();

		float calc = ((LQF->ExpectedFinishTime() - SQF->ExpectedFinishTime()) / LQF->ExpectedFinishTime())*100;

		
		while (calc > 40)
		{
			Process * temp = nullptr;
        	LQF->get_delete_top(temp);
			if (temp) {

				stealed_processes++;

				SQF->AddProccess(temp);

			}
			else
				break;
			calc = ((LQF->ExpectedFinishTime() - SQF->ExpectedFinishTime()) / LQF->ExpectedFinishTime()) * 100;
			
		}
	}
}

void Scheduler::addtoBlkList(Process* p) {
	BLK_LIST.enqueue(p);
}

int Scheduler::getProcessesCount() {
	return NProcess;
}

void Scheduler::addtoTrmList(Process* p) {
	if (p->getChild1() || p->getChild2())
		KillOrphan(p);
	p->SetTT(timestep);
	TRM_LIST.enqueue(p);
}

void Scheduler::KillOrphan(Process* p) {

	if (p->getChild1()) {
		for (int i = 0; i < processors_count; i++)
		{
			if (Processors[i]->getType() == "FCFS") {
				if (Processors[i]->Kill_Orphan(p->getChild1())) {
					break;
				}
			}
		}
	}
	if (p->getChild2()) {
		for (int i = 0; i < processors_count; i++)
		{
			if (Processors[i]->getType() == "FCFS") {
				if (Processors[i]->Kill_Orphan(p->getChild2())) {
					break;
				}
			}
		}
	}
}


void Scheduler::ReadFile()
{
	string FileName = "test.txt";
	ifstream File;
	int RTF;
	int RR;
	int MaxW;
	File.open(FileName);
	File >> NF >> NS >> NR >> RR >> RTF >> MaxW >> STL >> ForkProb >> NProcess;
	int j = 0;
	for (int i = 0; i < NF; i++)
	{
		FirstComeFirstServe* fcfs = new FirstComeFirstServe(this, ++j, MaxW);
		Processors[processors_count++] = fcfs;
	}
	for (int i = 0; i < NS; i++)
	{
		ShortestJobFirst * sjf = new ShortestJobFirst(this, ++j, RR);
		Processors[processors_count++] = sjf;
	}
	for (int i = 0; i < NR; i++)
	{
		RoundRobin* rr = new RoundRobin(this, ++j, RR, RTF);
		Processors[processors_count++] = rr;
	}
	for (int i = 0; i < NProcess; i++)
	{
		int AT;
		int PID;
		int CT;
		int N;
		File >> AT >> PID >> CT >> N;
		Process* P = new Process(AT, PID, CT, N);
		for (int i = 0; i < N; i++)
		{
			int IO_R;
			int IO_D;
			char C;
			File >> C >> IO_R >> C >> IO_D >> C;
			if (N > 1 && i != N - 1)
				File >> C;
			P->SetIO(IO_R, IO_D);
		}
		newList.enqueue(P);
	}
	int PID;
	int time;
	while (File >> time >> PID) {
		kill_signal * p = new kill_signal;
		p->pid = PID;
		p->time = time;
		SIGKILL_LIST.enqueue(p);
	}
}

void Scheduler::generateOutputFile() {
	Process* p;
	string FileName = "output.txt";
	ofstream File;
	File.open(FileName);
	int WT_sum = 0;
	int RT_sum = 0;
	int TRT_sum = 0;
	File << setprecision(2);
	File << "TT" << '\t'
		<< "PID" << '\t'
		<< "AT" << '\t'
		<< "CT" << '\t'
		<< "IO_D" << '\t'
		<< "WT" << '\t'
		<< "RT" << '\t'
		<< "TRT" << '\n';
	for (int i = 0; i < TRM_LIST.getcount(); i++)
	{
		TRM_LIST.dequeue(p);
		p->Printprocess(File);
		WT_sum += p->getWT();
		RT_sum += p->getRT();
		TRT_sum += p->getTRT();
		// put it back so I can delete it at the end
		TRM_LIST.enqueue(p);
	}
	File << "processes: " << NProcess<< "\n";
	if (NProcess != 0) {
		File << "Avg WT =" << (WT_sum / NProcess) << '\t';
		File << "Avg RT =" << (RT_sum / NProcess) << '\t';
		File << "Avg TRT =" << (TRT_sum / NProcess) << '\n';
	}

	File << "Migration %:\t"
		<< "RTF= " << ((float)migrated_rtf / NProcess) * 100 << "%,\t"
		<< "MaxW= " << ((float)migrated_maxw / NProcess) * 100 << "%,\n"
		<< "Work Steal%: "<< ((float)stealed_processes / NProcess) * 100 <<"%\n"
		<< "Forked Process%: " << ((float)forked_processes / NProcess) * 100 << "%\n"
		<< "Killed Process%: " << ((float)killed_processes / NProcess) * 100 << "%\n\n";

	File << "Processors: " << processors_count
		<< " [" << NF << " FCFS," << NS << " SJF," << NR << " RR]\n";
	File << "Processors Load\n";
	
	for (int i = 0; i < processors_count; i++)
	{
		File << "p" << Processors[i]->getID()
			<<"="<< ((float)Processors[i]->getBusyTime()/ (float)TRT_sum) * 100 <<"%";
		if (i != processors_count - 1) {
			File << ",\t";
		}
		else {
			File << "\n";
		}
	}
	File << "Processors Utiliz\n";
	float utilization_sum = 0;
	for (int i = 0; i < processors_count; i++)
	{
		File << "p" << Processors[i]->getID()
			<< "=" <<((float)Processors[i]->getBusyTime() / (float)timestep) * 100 << "%";
		utilization_sum += ((float)Processors[i]->getBusyTime() / (float)timestep) * 100;
		if (i != processors_count - 1) {
			File << ",\t";
		}
		else {
			File << "\n";
		}
	}
	File << "Avg Utilization = "<<(utilization_sum/(float)processors_count)<<"%";
}

void Scheduler::Print()
{
	UI_IO->PrintUI(Processors,processors_count, TRM_LIST, BLK_LIST);
}

int Scheduler::getTimeStep()
{
	return timestep;
}

int Scheduler::getTRM_LIST_COUNT()
{
	return TRM_LIST.getcount();
}


void Scheduler::FullSchedule() 
{
	UI_IO->SelectMode();
	generateOutputFile();
}