#include "ConsoleManager.h"

int main()
{
	ConsoleManager::initialize();

	while (true) {
		ConsoleManager::getInstance()->run();
	};

	return 0;
}