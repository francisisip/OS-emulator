
#include "ConsoleManager.h"
#include "Scheduler.h"
// #include "ResourceManager.h"
#include <iostream>
int main()
{
	Scheduler::initialize();
	ConsoleManager::initialize();
	// ResourceManager::initialize();

	while (true) {
		ConsoleManager::getInstance()->run();
	};

	return 0;
}