#ifndef UI_H
#define UI_H

#include"Processors/Processor.h"
#include "Datastructures/LinkedQueue.h"

class Scheduler;
class UI
{
	Scheduler* scheduler;
public:
	UI(Scheduler* S);
	void PrintUI(Processor * processors[], int processors_count, LinkedQueue<Process*>& TRM_LIST, LinkedQueue<Process*>& BLK_LIST);
	void SelectMode();
};
#endif