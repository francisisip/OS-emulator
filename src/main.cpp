#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <sstream>

#include "Screen.h"
using namespace std;


void exit(bool* flag) {
	*flag = 0;
}

int main() {

	// initialization
	Screen mainScreen;
	std::vector<Screen> screens;
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

		// for 2 or more wordscl
		if (wordCount > 1) {
			std::string instruction, option, param;
		    iss >> instruction >> option >> param;
			
			if (instruction == "screen" && option == "-s") {
				//std::cout << "Screen command recognized. Doing somethingw" << std::endl;
				bool processRunning = true;

				screens.push_back(Screen(param));
				screens.back().clear();
				screens.back().printProcessInfo();

				while (processRunning) {
					std::cout << screens.back().getScreenName() << "@csopesy:~$ ";

					std::string command;
					std::getline(std::cin, command);
					
					if (command == "clear") {
						screens.back().clear();
					}
					else if (command == "exit") {
						exit(&processRunning);
						system("cls");
						mainScreen.printInstructions();
					}
					else {
						std::cout << "Command not recognized\n" << std::endl;
					}
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
	}

	

    return 0;
}

