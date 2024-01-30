#include"UI.h"
#include"Scheduler.h"

UI::UI(Scheduler* S)
{
	scheduler = S;
}

void UI::PrintUI(Processor * processors[], int processors_count, LinkedQueue<Process*>& TRM_LIST, LinkedQueue<Process*>& BLK_LIST)
{
	cout << "Current Timestep : " << scheduler->getTimeStep() << endl;
	cout << "------------ RDY LIST  ------------ \n";
	for (int i = 0; i < processors_count; i++)
	{
		processors[i]->printInfo();
		cout << processors[i]->getprocesses_count() << " RDY :";
		processors[i]->printReadyList();
	}
	cout << endl;

	cout << "------------ BLK LIST  ------------ \n";
	cout << BLK_LIST.getcount() << " BLK: ";
	BLK_LIST.Print();
	cout << "------------ RUN LIST  ------------ \n";
	int countRun = 0;
	for (int i = 0; i < processors_count; i++)
	{
		if (processors[i]->getRunningProcess()) {
			countRun++;
		}
	}
	cout << countRun << " RUN" << ": ";
	for (int i = 0; i < processors_count; i++)
	{
		if (processors[i]->getRunningProcess()) {
			cout << "  " << *processors[i]->getRunningProcess() << "(" << "P" << processors[i]->getID() << ")";
		}
	}
	cout << endl;
	cout << "\n ------------ TRM LIST  ------------ \n";
	cout << TRM_LIST.getcount() << " TRM: ";
	TRM_LIST.Print();
}

void UI::SelectMode()
{
	cout << "Select Mode:" << endl << "1-Interactive Mode" << endl << "2-Step-By-Step Mode" << endl << "3-Silent Mode" << endl;
	int x;
	cin >> x;
	while (x!=3&&x!=2&&x!=1)
	{
		cin >> x;
	}
	if (x == 1)
	{
		cout << "Interactive Mode..." << endl;
		while (scheduler->getProcessesCount() != scheduler->getTRM_LIST_COUNT())
		{
			scheduler->schedule();
			scheduler->Print();
			
			system("pause");
		}
	}
	else if (x == 2)
	{
		cout << "Step-By-Step Mode..." << endl;
		while(scheduler->getProcessesCount() != scheduler->getTRM_LIST_COUNT())
		{
			scheduler->schedule();
			scheduler->Print();
			Sleep(5);
		}
	}
	else if (x == 3)
	{
		cout <<"Silent Mode..." << endl;
		cout << "Simulation Starts..." << endl;
		while (scheduler->getProcessesCount() != scheduler->getTRM_LIST_COUNT())
		{
			scheduler->schedule();
		}
		cout << "Simulation end, Output file is created" << endl;
	}
}
