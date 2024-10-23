
#include "ConsoleManager.h"
#include "Scheduler.h"
#include "Config.h"
#include "ResourceManager.h"
#include <iostream>

int main()
{
	Scheduler::initialize();
	ConsoleManager::initialize();
	Config::initialize();
	ResourceManager::initialize();
	
	while (true) {
		ConsoleManager::getInstance()->run();
	};

	return 0;
}