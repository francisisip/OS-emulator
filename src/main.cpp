
#include "ConsoleManager.h"
#include "Scheduler.h"
#include <iostream>
int main()
{
	ConsoleManager::initialize();

	while (true) {
		ConsoleManager::getInstance()->run();
	};

	return 0;
}