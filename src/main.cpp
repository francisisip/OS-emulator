#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdio>

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
		std::cout << "Enter Command: ";

		std::string name;
		std::getline(std::cin, name);

		if (name == "initialize") {
			mainScreen.initialize();
		}
		else if (name == "screen") {
			mainScreen.screen();
		}
		else if (name == "scheduler-test") {
			mainScreen.schedulerTest();
		}
		else if (name == "scheduler-stop") {
			mainScreen.schedulerStop();
		}
		else if (name == "report-util") {
			mainScreen.reportUtil();
		}
		else if (name == "clear") {
			mainScreen.clear();
		}
		else if (name == "exit") {
			exit(&osRunning);
		}
		else {
			std::cout << "Command not recognized\n" << std::endl;
		}
	}

    return 0;
}

