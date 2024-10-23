#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>

#include "MenuScreen.h"
#include "ConsoleManager.h"
#include "Scheduler.h"
#include "ResourceManager.h"

ConsoleManager* instance;
Scheduler* schedulerInstance;
ResourceManager* resourceInstance;

MenuScreen::MenuScreen(): AConsole("MAIN_MENU") {
	this->isInitialized = false;
}

MenuScreen::~MenuScreen() {

}

bool MenuScreen::getInitialized() {
	return this->isInitialized;
}

void MenuScreen::setInitialized(bool flag) {
	this->isInitialized = flag;
}

void MenuScreen::onExecute() {
	system("clear");
	instance = ConsoleManager::getInstance();
	schedulerInstance = Scheduler::getInstance();
	resourceInstance = ResourceManager::getInstance();
	display();
	process();
}

void MenuScreen::display() {
	displayASCII();
	for (int i = 0; i < commandHistory.size(); ++i) {
        std::cout << "root@csopesy:~$ " << commandHistory[i] << std::endl;
		if (!commandHistory[i].empty()) {
			std::cout << "\n";
		}
	}
}

void MenuScreen::process() {
	std::string command;
	while (true) {
		std::cout << "root@csopesy:~$ ";

		std::string command;
		std::getline(std::cin, command);
		commandHistory.push_back(command);
		handleInput(command);
	}
}

void MenuScreen::displayASCII() {
	std::cout << R"(
    $$$$$$\   $$$$$$\   $$$$$$\  $$$$$$$\  $$$$$$$$\  $$$$$$\ $$\     $$\ 
    $$  __$$\ $$  __$$\ $$  __$$\ $$  __$$\ $$  _____|$$  __$$\\$$\   $$  |
    $$ /  \__|$$ /  \__|$$ /  $$ |$$ |  $$ |$$ |      $$ /  \__|\$$\ $$  / 
    $$ |      \$$$$$$\  $$ |  $$ |$$$$$$$  |$$$$$\    \$$$$$$\   \$$$$  /  
    $$ |       \____$$\ $$ |  $$ |$$  ____/ $$  __|    \____$$\   \$$  /   
    $$ |  $$\ $$\   $$ |$$ |  $$ |$$ |      $$ |      $$\   $$ |   $$ |    
    \$$$$$$  |\$$$$$$  | $$$$$$  |$$ |      $$$$$$$$\ \$$$$$$  |   $$ |    
    \______/  \______/  \______/ \__|      \________| \______/    \__|    
    )";

	std::cout << "Hello, welcome to CSOPESY commandline!" << '\n';
	std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n" << '\n';
}

void MenuScreen::handleInput(std::string command) {
	std::istringstream iss(command);
	std::istringstream copy(command);
	std::string word;
	int wordCount = 0;

	while (copy >> word) {
		wordCount++;
	}

	if (getInitialized()) {
		if (wordCount == 3) {
			std::string instruction, option, param;
			iss >> instruction >> option >> param;

			if (instruction == "screen" && option == "-s") {
				instance->createProcessScreen(param);
			}
			else if (instruction == "screen" && option == "-r") {
				bool flag = instance->ifProcessScreenExists("P_" + param);

				if (!flag) {
					commandHistory.back() += "\nScreen not found";
					std::cout << "Screen not found\n" << std::endl;
				}
				else {
					instance->switchScreen("P_" + param);
				}
			}
			else if (instruction == "screen" && option == "-ls") {
				std::cout << "screen -ls called";
				schedulerInstance->printSchedulerStatus();
			}
			else {
				commandHistory.back() += "\nCommand not recognized";
				std::cout << "Command not recognized\n" << std::endl;
			}
		}
		else if (wordCount == 1) {
			if (command == "initialize") {
				std::cout << "OS is already initialized\n" << std::endl;
			}

			// TODO: disallow all commands until initialize is called
			else if (command == "scheduler-test") {
				std::cout << "scheduler-test command recognized. Doing something\n" << std::endl;
				// TODO: ResourceManager::schedulerTestStart();
			}
			else if (command == "scheduler-stop") {
				std::cout << "scheduler-stop command recognized. Doing something\n" << std::endl;
			}
			else if (command == "report-util") {
				// TODO: txt file
				std::cout << "report-util command recognized. Doing something\n" << std::endl;
			}
			else if (command == "clear") {
				commandHistory.clear();
				system("clear");
			}
			else if (command == "exit") {
				exit(0);
			}
			else {
				commandHistory.back() += "\nCommand not recognized";
				std::cout << "Command not recognized\n" << std::endl;
			}
		}
		else if (wordCount != 0) {
			commandHistory.back() += "\nCommand not recognized";
			std::cout << "Command not recognized\n" << std::endl;
		}
	}
	else {
		if (wordCount == 1) {
			if (command == "initialize") {
				setInitialized(true);
				std::cout << "Initialized OS, all systems running\n" << std::endl;
				// TODO: Implement initialize. start scheduler
				// ResourceManager initialize = Scheduler, which will set its cpu cores, scheduling type
				resourceInstance->initializeScheduler();
			}
			else if (command == "exit") {
				exit(0);
			}
			else {
				std::cout << "Initialize the OS first with command \"initialize\"\n" << std::endl;
			}
		}
		else {
			std::cout << "Initialize the OS first with command \"initialize\"\n" << std::endl;
		}
	}
}