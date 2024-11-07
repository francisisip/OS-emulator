#include <iostream>
#include <string>
#include <sstream>
#include "ProcessScreen.h"
#include "ConsoleManager.h"
#include "FlatMemoryAllocator.h"
#include <iomanip>

ConsoleManager* currentInstance;
FlatMemoryAllocator* allocator;

ProcessScreen::ProcessScreen(std::shared_ptr<Process> process): AConsole(process->getName()) {
    currentProcess = process;
    currentInstance = ConsoleManager::getInstance();
    allocator = FlatMemoryAllocator::getInstance();
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
    std::cout << "Process: " << currentProcess->getName() << std::endl;
    std::cout << "ID: " << currentProcess->getPId() << std::endl;
    // Visualize process memory here:
    std::cout << "Memory Required:" << currentProcess->getMemoryRequired() << std::endl;
    std::cout << "\n";
    
    if (currentProcess->getState() != Process::ProcessState::FINISHED) {
        std::cout << "Current instruction line: " << currentProcess->getCommandCounter() << std::endl;
        std::cout << "Lines of Code: " << currentProcess->getCommandCount() << "\n" << std::endl;
    }

    std::cout << "--------------------------------------------\n" << std::endl;

    for (int i = 0; i < commandHistory.size(); ++i) {
        std::cout << currentProcess->getName() <<"@csopesy:~$ " << commandHistory[i] << std::endl;
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
            if (command == "process-smi") {
                std::string smi_string = "\n";

                smi_string += "Process: " + currentProcess->getName() + "\n";
                smi_string += "ID: " + std::to_string(currentProcess->getPId()) + "\n";
                smi_string += "\n";

                if (currentProcess->getState() != Process::ProcessState::FINISHED) {
                    smi_string += "Current instruction line: " + std::to_string(currentProcess->getCommandCounter()) + "\n";
                    smi_string += "Lines of Code: " + std::to_string(currentProcess->getCommandCount());
                } else {
                    smi_string += "Finished!";
                }

                std::cout << smi_string; 
                std::cout << "\n\n";
                commandHistory.back() += "\n" + smi_string;
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
            commandHistory.back() += "\nCommand not recognized.";
            std::cout << "Command not recognized.\n" << std::endl;
        }
    }
    else if (wordCount != 0) {
        commandHistory.back() += "\nCommand not recognized.";
        std::cout << "Command not recognized.\n" << std::endl;
    }
}

bool ProcessScreen::isFinished() const {
    if (currentProcess->getState() == Process::ProcessState::FINISHED) {
        return false;
    }
    else {
        return true;
    }
}
