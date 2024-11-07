
#include "ConsoleManager.h"
#include "Scheduler.h"
#include "Config.h"
#include "ResourceManager.h"
#include "FlatMemoryAllocator.h"

#include <iostream>

void tickCPUCycle() {
	int CPUCycle = 0;
	while (true) {
		CPUCycle++;
		//std::cout << CPUCycle << "current tick" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	Scheduler::initialize();
	ConsoleManager::initialize();
	Config::initialize();
	ResourceManager::initialize();
	FlatMemoryAllocator::initialize(16384); // initialize here for now.

	std::thread CPUCycleThread(tickCPUCycle);
	
	while (true) {
		CPUCycleThread.detach();
		ConsoleManager::getInstance()->run();
		
	};

	return 0;
}

