#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <sstream>

#include "ScreenManager.h"
using namespace std;

ScreenManager manager;
Screen mainScreen;

void exit(bool* flag) {
	*flag = 0;
}

void enterSubScreen(Screen* currentScreen) {
	bool processRunning = true;

	while (processRunning) {
		std::cout << currentScreen->getScreenName() << "@csopesy:~$ ";

		std::string command;
		std::getline(std::cin, command);
		currentScreen->storeInstructions(command);

		if (command == "clear") {
			currentScreen->clear();
		}
		else if (command == "exit") {
			exit(&processRunning);
			system("cls");
			mainScreen.displayASCII();
			mainScreen.printInstructions();
		}
		else {
			std::cout << "Command not recognized\n" << std::endl;
		}
	}
}


int main() {
	manager.createScreen("Main");
	mainScreen = manager.getScreens().back();
	mainScreen.displayASCII();

	bool osRunning = true;

	while (osRunning) {
		std::cout << "root@csopesy:~$ ";

		std::string command;
		std::getline(std::cin, command);
		mainScreen.storeInstructions(command);

		// parse command and get word count
		std::istringstream iss(command);
		std::istringstream copy(command);
		std::string word;
		int wordCount = 0;

		while (copy >> word) {
			wordCount++;
		}

		if (wordCount == 3) {
			std::string instruction, option, param;
		    iss >> instruction >> option >> param;
			
			if (instruction == "screen" && option == "-s") {
				manager.createScreen(param);
				system("cls");
				Screen* curr = &manager.getScreens().back();
				curr->printProcessInfo();
				enterSubScreen(curr);
			}
			else if (instruction == "screen" && option == "-r") {
				Screen* currentScreen = manager.findScreenByName(param);

				if (currentScreen == nullptr) {
					std::cout << "Screen not found\n" << std::endl;
				}
				else {
					system("cls");
					currentScreen->printProcessInfo();
					currentScreen->printInstructions();
					enterSubScreen(currentScreen);
				}
			}
			else {
				std::cout << "Command not recognized\n" << std::endl;
			}
		}
		else if (wordCount == 1) {
			if (command == "initialize") {
				mainScreen.initialize();
			}
			else if (command == "screen") {
				mainScreen.screen();
			}
			else if (command == "scheduler-test") {
				mainScreen.schedulerTest();
			}
			else if (command == "scheduler-stop") {
				mainScreen.schedulerStop();
			}
			else if (command == "report-util") {
				mainScreen.reportUtil();
			}
			else if (command == "clear") {
				mainScreen.clear();
			}
			else if (command == "exit") {
				exit(&osRunning);
			}
			else {
				std::cout << "Command not recognized\n" << std::endl;
			}
		}
		else {
			std::cout << "Command not recognized\n" << std::endl;
		}	
	}

    return 0;
}

