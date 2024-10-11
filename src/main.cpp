
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
	std::shared_ptr<Process> process1 = std::make_shared<Process>("process 1", 100);
	std::shared_ptr<Process> process2 = std::make_shared<Process>("process 2", 100);
	std::shared_ptr<Process> process3 = std::make_shared<Process>("process 3", 100);
	std::shared_ptr<Process> process4 = std::make_shared<Process>("process 4", 100);
	std::shared_ptr<Process> process5 = std::make_shared<Process>("process 5", 100);
	std::shared_ptr<Process> process6 = std::make_shared<Process>("process 6", 100);
	std::shared_ptr<Process> process7 = std::make_shared<Process>("process 7", 100);
	std::shared_ptr<Process> process8 = std::make_shared<Process>("process 8", 100);
	std::shared_ptr<Process> process9 = std::make_shared<Process>("process 9", 100);
	std::shared_ptr<Process> process10 = std::make_shared<Process>("process 10", 100);
	scheduler.initialize();
	scheduler.initializeCores(4);
	scheduler.addProcess(*process1);
	scheduler.addProcess(*process2);
	scheduler.addProcess(*process3);
	scheduler.addProcess(*process4);
	scheduler.addProcess(*process5);
	scheduler.addProcess(*process6);
	scheduler.addProcess(*process7);
	scheduler.addProcess(*process8);
	scheduler.addProcess(*process9);
	scheduler.addProcess(*process10);

	std::string command;
	while (true) {
		// Wait for user input
		std::cout << "Enter command: ";
		std::getline(std::cin, command);

		// Handle commands
		if (command == "screen -ls") {
			scheduler.printSchedulerStatus();
		} else if (command == "exit") {
			break; // Exit the loop
		}
	}

	return 0;
}