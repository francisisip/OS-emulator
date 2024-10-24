#include <iostream>
#include <string>
#include <sstream>
#include "ProcessScreen.h"
#include "ConsoleManager.h"
#include <iomanip>

ConsoleManager* currentInstance;

ProcessScreen::ProcessScreen(std::shared_ptr<Process> process): AConsole(process->getName()) {
    currentProcess = process;
    currentInstance = ConsoleManager::getInstance();
}

ProcessScreen::~ProcessScreen() {
}

void ProcessScreen::onExecute() {
    system("clear");
    display();
    process();
}

std::string getFormattedTime() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    std::stringstream ss;
    ss << std::put_time(localTime, "%m/%d/%Y, %I:%M:%S ");
    ss << (localTime->tm_hour >= 12 ? "PM" : "AM");

    return ss.str();
}

void ProcessScreen::display() {
    std::cout << currentProcess->getName()<< std::endl;
    std::cout << currentProcess->getCommandCounter() << "/"; 
    std::cout << currentProcess->getCommandCount() << std::endl;
    std::cout << getFormattedTime() << "\n" << std::endl;

    for (int i = 0; i < commandHistory.size(); ++i) {
        std::cout << "root@csopesy:~$ " << commandHistory[i] << std::endl;
        if (!commandHistory[i].empty()) {
			std::cout << "\n";
		}
	}
}

void ProcessScreen::process() {
    std::string command;
    while (true) {
        std::cout << currentProcess->getName() + "@csopesy:~$ ";
        
        std::string command;
        std::getline(std::cin, command);
        command != "exit" ? commandHistory.push_back(command) : void();
        handleInput(command);
    }
}

void ProcessScreen::handleInput(std::string command) {
    std::istringstream iss(command);
	std::istringstream copy(command);
	std::string word;
	int wordCount = 0;

	while (copy >> word) {
		wordCount++;
	}

    if (wordCount == 1) {
        if (command == "process-smi") {
            std::cout << "Process: " << currentProcess->getName() << std::endl;
            std::cout << "ID: " << currentProcess->getPId() << std::endl;
            std::cout << "\n";
            
            if (currentProcess->getState() != Process::ProcessState::FINISHED) {
                std::cout << "Current instruction line: " << currentProcess->getCommandCounter() << std::endl;
                std::cout << "Lines of Code: " << currentProcess->getCommandCount() << "\n" << std::endl;
            }
            else {
                std::cout << "Finished!\n" << std::endl;
            }
        }
        else if (command == "clear") {
            commandHistory.clear();
            system("clear");
        }
        else if (command == "exit") {
            currentInstance->switchScreenBack();
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

bool ProcessScreen::isFinished() const {
    if (currentProcess->getState() == Process::ProcessState::FINISHED) {
        return true;
    }
    else {
        return false;
    }
}
