#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdio>
using namespace std;

void displayASCII() {
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
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen" << '\n';
}

void initialize (){
	std::cout << "initialize command recognized. Doing something\n" << std::endl;
}

void screen (){
    std::cout << "screen command recognized. Doing something\n" << std::endl;
}
void schedulerTest (){
    std::cout << "scheduler-test command recognized. Doing something\n" << std::endl;
}
void schedulerStop (){
    std::cout << "scheduler-stop command recognized. Doing something\n" << std::endl;
}

void reportUtil() {
    std::cout << "report-util command recognized. Doing something\n" << std::endl;
}

void clear() {
    system("cls");
	displayASCII();
}

void exit(bool* flag) {
	*flag = 0;
}

int main() {

    displayASCII();
	bool running = true;

	while (running) {
		std::cout << "Enter Command: ";

		std::string name;
		std::getline(std::cin, name);

		if (name == "initialize") {
			initialize();
		}
		else if (name == "screen") {
			screen();
		}
		else if (name == "scheduler-test") {
			schedulerTest();
		}
		else if (name == "scheduler-stop") {
			schedulerStop();
		}
		else if (name == "report-util") {
			reportUtil();
		}
		else if (name == "clear") {
			clear();
		}
		else if (name == "exit") {
			exit(&running);
		}
		else {
			std::cout << "Command not recognized\n" << std::endl;
		}
	}

    return 0;
}

