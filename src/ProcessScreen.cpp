#include <iostream>
#include <string>
#include <sstream>
#include "ProcessScreen.h"
#include "ConsoleManager.h"

ConsoleManager* instance2 = ConsoleManager::getInstance();

ProcessScreen::ProcessScreen(std::shared_ptr<Process> process): AConsole("P_" + process->getName()) {
}

ProcessScreen::~ProcessScreen() {
}

void ProcessScreen::onExecute() {
    display();
    process();
}

void ProcessScreen::display() {
    
}

void ProcessScreen::process() {
    std::string command;
    while (true) {
        std::cout << currentProcess->getName() + "@csopesy:~$ ";

        std::string command;
        std::getline(std::cin, command);
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
        if (command == "clear") {
            system("cls");
        }
        else if (command == "exit") {
            instance2->switchScreenBack();
        }
        else {
            std::cout << "Command not recognized\n" << std::endl;
        }
    }
    else {
        std::cout << "Command not recognized\n" << std::endl;
    }
}


