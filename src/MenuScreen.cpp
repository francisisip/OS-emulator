#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>

#include "MenuScreen.h"
#include "ConsoleManager.h"

ConsoleManager* instance = ConsoleManager::getInstance();

MenuScreen::MenuScreen(): AConsole("MAIN_MENU") {

}

MenuScreen::~MenuScreen() {

}

void MenuScreen::onExecute() {
	display();
	process();
}

void MenuScreen::display() {
	displayASCII();
}

void MenuScreen::process() {
	std::string command;
	while (true) {
		std::cout << "root@csopesy:~$ ";

		std::string command;
		std::getline(std::cin, command);
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

	if (wordCount == 3) {
		std::string instruction, option, param;
		iss >> instruction >> option >> param;

		if (instruction == "screen" && option == "-s") {
			instance->createProcessScreen(param);
			instance->switchScreen("P_" + param);
		}
		else if (instruction == "screen" && option == "-r") {
			bool flag = instance->ifProcessScreenExists("P_" + param);

			if (!flag) {
				std::cout << "Screen not found\n" << std::endl;
			}
			else {
				instance->switchScreen("P_" + param);
			}
		}
		else {
			std::cout << "Command not recognized\n" << std::endl;
		}
	}
	else if (wordCount == 1) {
		if (command == "initialize") {
			std::cout << "initialize command recognized. Doing something\n" << std::endl;
		}
		else if (command == "scheduler-test") {
			std::cout << "scheduler-test command recognized. Doing something\n" << std::endl;
		}
		else if (command == "scheduler-stop") {
			std::cout << "scheduler-stop command recognized. Doing something\n" << std::endl;
		}
		else if (command == "report-util") {
			std::cout << "report-util command recognized. Doing something\n" << std::endl;
		}
		else if (command == "clear") {
			system("cls");
		}
		else if (command == "exit") {
			exit(0);
		}
		else {
			std::cout << "Command not recognized\n" << std::endl;
		}
	}
	else {
		std::cout << "Command not recognized\n" << std::endl;
	}
}