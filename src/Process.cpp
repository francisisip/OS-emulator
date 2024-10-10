#include "Process.h"
#include <iostream>

Process::Process(std::string name) {
	this->name = name;
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

std::string Process::getTimeCreated() const {
	return "sample date";
}

int Process::getCommandCounter() const {
	return commandCounter;
}

int Process::getCommandCount() const {
	return commandCount;
};

bool Process::isFinished() const {
	return isFinished;
}


void Process::executeCurrentCommand() {
	return;
}

void Process::moveToNextLine() {
	this->commandCounter++;
}

