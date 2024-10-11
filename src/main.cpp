
#include "ConsoleManager.h"
#include "Scheduler.h"
#include <iostream>
int main()
{
	// ConsoleManager::initialize();

	// while (true) {
	// 	ConsoleManager::getInstance()->run();
	// };
	Scheduler scheduler;
	scheduler.initialize();
	scheduler.initializeCores(4);
	std::shared_ptr<Process> process1 = std::make_shared<Process>("process 1", 100);
	std::shared_ptr<Process> process2 = std::make_shared<Process>("process 2", 100);
	std::shared_ptr<Process> process3 = std::make_shared<Process>("process 3", 100);
	std::shared_ptr<Process> process4 = std::make_shared<Process>("process 4", 100);
	
	scheduler.addProcess(*process1);
	scheduler.addProcess(*process2);
	scheduler.addProcess(*process3);
	scheduler.addProcess(*process4);

	scheduler.runScheduler();


	return 0;
}