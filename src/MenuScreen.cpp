#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include <iomanip> 

#include "MenuScreen.h"
#include "ConsoleManager.h"
#include "Paging.h"
#include "Scheduler.h"
#include "ResourceManager.h"
#include "FlatMemoryAllocator.h"
#include "Config.h"

ConsoleManager* instance;
Scheduler* schedulerInstance;
ResourceManager* resourceInstance;
Config* configInstance;

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
	configInstance = Config::getInstance();
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


void MenuScreen::displayVMStat() {
    size_t maxMemory = configInstance->getMaxMemory();
    size_t usedMemory = resourceInstance->getActiveMemory();
    size_t freeMemory = maxMemory - usedMemory;
    long long idleCPUTicks = resourceInstance->getIdleCPUTicks();
    long long activeCPUTicks = resourceInstance->getActiveCPUTicks();
    long long totalCPUTicks = idleCPUTicks + activeCPUTicks;

    // Define the width for alignment
    const int valueWidth = 15;
    const int labelWidth = 20;

    std::cout << std::endl;
    std::cout << std::setw(valueWidth) << maxMemory 
              << " " << std::setw(labelWidth) << "KB total memory" << std::endl;

    std::cout << std::setw(valueWidth) << usedMemory 
              << " " << std::setw(labelWidth) << "KB used memory" << std::endl;

    std::cout << std::setw(valueWidth) << freeMemory 
              << " " << std::setw(labelWidth) << "KB free memory" << std::endl;

    std::cout << std::setw(valueWidth) << idleCPUTicks 
              << " " << std::setw(labelWidth) << "idle cpu ticks" << std::endl;

    std::cout << std::setw(valueWidth) << activeCPUTicks 
              << " " << std::setw(labelWidth) << "active cpu ticks" << std::endl;

    std::cout << std::setw(valueWidth) << totalCPUTicks 
              << " " << std::setw(labelWidth) << "total cpu ticks" << std::endl;

	// TODO: Replace placeholders fo paged-in values.
    std::cout << std::setw(valueWidth) << 0 
              << " " << std::setw(labelWidth) << "pages paged in" << std::endl;

    std::cout << std::setw(valueWidth) << 0 
              << " " << std::setw(labelWidth) << "pages paged out" << std::endl;

    std::cout << std::endl;
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
				instance->switchScreen(instance->createProcessScreen(param));
			}
			else if (instruction == "screen" && option == "-r") {
				bool flag = instance->ifProcessScreenExistsAndNotFinished(param);

				if (!flag) {
					commandHistory.back() += "\nProcess " + param + " not found.";
					std::cout << "Process "<< param << " not found.\n" << std::endl;
				}
				else {
					instance->switchScreen(param);
				}
			}
			else {
				commandHistory.back() += "\nCommand not recognized.";
				std::cout << "Command not recognized.\n" << std::endl;
			}
		}
		else if (wordCount == 2) {
			std::string instruction, option;
			iss >> instruction >> option;

			if (instruction == "screen" && option == "-ls") {
				commandHistory.back() += schedulerInstance->getSchedulerStatus();
				std::cout << "\n" << schedulerInstance->getSchedulerStatus() << "\n\n";
				// FlatMemoryAllocator::getInstance()->visualizeMemory();
        // Paging::getInstance()->visualizeMemory();
			}
			else {
				commandHistory.back() += "\nCommand not recognized.";
				std::cout << "Command not recognized.\n" << std::endl;
			}
		}
		else if (wordCount == 1) {
			if (command == "initialize") {
				commandHistory.back() += "\nOS is already initialized.";
				std::cout << "OS is already initialized.\n" << std::endl;
			}

			else if (command == "scheduler-test") {
				if (!resourceInstance->startSchedulerInThread()) {
					commandHistory.back() += "\nscheduler-test command recognized. Starting creation of processes...";
					std::cout << "scheduler-test command recognized. Starting creation of processes...\n" << std::endl;
				}
				else {
					commandHistory.back() += "\nscheduler-test is already running.";
					std::cout << "scheduler-test is already running.\n" << std::endl;
				}
			}
			else if (command == "scheduler-stop") {
				if (resourceInstance->schedulerTestStop())
				{
					commandHistory.back() += "\nscheduler-stop command recognized. Stopping creation of processes...";
					std::cout << "scheduler-stop command recognized. Stopping creation of processes...\n" << std::endl;
				}
				else
				{
					commandHistory.back() += "\nscheduler-test is not running, there is nothing to stop.";
					std::cout << "scheduler-test is not running, there is nothing to stop.\n" << std::endl;
				}
			} else if (command == "vm-stat") {
				displayVMStat();
			}
			else if (command == "report-util") {
				std::ofstream outFile("csopesy-log.txt");

				if (outFile.is_open()) {
					outFile << schedulerInstance->getSchedulerStatus();
					outFile.close();

					std::filesystem::path currentPath = std::filesystem::current_path();
					commandHistory.back() += "\nroot@csopesy:~$ Report generated at " + currentPath.string() + "\\csopesy-log.txt";
					std::cout << "Report generated at " << currentPath.string() << "\\csopesy-log.txt\n" << std::endl;
				}
				else {
					commandHistory.back() += "\nError generationt report file.";
					std::cout << "Error generating report file.\n" << std::endl;
				}
			}
			else if (command == "clear") {
				commandHistory.clear();
				system("clear");
			}
			else if (command == "exit") {
				exit(0);
			}
			else {
				commandHistory.back() += "\nCommand not recognized.";
				std::cout << "Command not recognized.\n" << std::endl;
			}
		}
		else if (wordCount != 0) {
			commandHistory.back() += "\nCommand not recognized.";
			std::cout << "Command not recognized.\n" << std::endl;
		}
	}
	else {
		if (wordCount == 1) {
			if (command == "initialize") {
				setInitialized(true);
				commandHistory.back() += "\nInitialized OS, all systems running.";
				std::cout << "Initialized OS, all systems running.\n" << std::endl;
				// ResourceManager initialize = Scheduler, which will set its cpu cores, scheduling type
				resourceInstance->initializeScheduler();
			}
			else if (command == "exit") {
				exit(0);
			}
			else {
				commandHistory.back() += "\nInitialize the OS first with command \"initialize.\"";
				std::cout << "Initialize the OS first with command \"initialize.\"\n" << std::endl;
			}
		}
		else {
			commandHistory.back() += "\nInitialize the OS first with command \"initialize.\"";
			std::cout << "Initialize the OS first with command \"initialize.\"\n" << std::endl;
		}
	}
}
