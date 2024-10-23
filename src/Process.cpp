#include "Process.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

Process::Process(std::string name){
	//TODO: make processes based off the the ticker
	// Add to the ready queue
	this->name = name;
	this->commandCount = commandCount;
	this->commandCounter = 0;
	timeCreated = std::chrono::system_clock::now();
}

int Process::getPId() const {
	return pid;
}

std::string Process::getName() const {
	return name;
}

int Process::getCPUCoreID() const {
	return cpuCoreID;
}

Process::ProcessState Process::getState() const {
	return currentState;
}

std::string Process::getCurrentCommandTime() const {
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
	return oss.str(); // Return current date and time as a string
}
std::string Process::getTimeCreated() const {
	auto time_c = std::chrono::system_clock::to_time_t(timeCreated);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&time_c), "%Y-%m-%d %H:%M:%S%p");
	return oss.str(); // Return formatted creation time
}
int Process::getCommandCounter() const {
	return commandCounter;
}

int Process::getCommandCount() const {
	return commandCount;
};

bool Process::isFinished() const {
	return finished;
}

void Process::setCore(int coreID) {
	cpuCoreID = coreID;
}

void Process::executeCurrentCommand() {
	if (commandCounter < commandCount) {
		moveToNextLine();

		// Append to log with the specified format
		std::ofstream logFile(name + ".txt", std::ios::app);
		if (logFile) {
			logFile << "(" << getCurrentCommandTime() << ") Core: " << cpuCoreID
					<< " \"Hello world from " << name << "\"" << std::endl;
			logFile.close();
		}
	} else {
		finished = true;
	}
}
void Process::moveToNextLine() {
	commandCounter++;
}

