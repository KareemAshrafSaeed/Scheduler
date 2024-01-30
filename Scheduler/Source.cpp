using namespace std;
#include "Scheduler.h"
int main() {
	
	Scheduler sc;
	sc.ReadFile();
	sc.FullSchedule();
	/*while (sc.getProcessesCount() != sc.getTRM_LIST_COUNT())
	{
		sc.schedule();
		sc.Print();
		system("pause");
		
	}
	sc.generateOutputFile();*/
	/*sc.clearData();*/
	
	return 0;
}

