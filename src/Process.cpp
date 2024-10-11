#pragma once
#include "Process.h"
#include <iostream>

Process::Process(std::string name) {
	this->name = name;
}

Process::Process(std::string name, int commandCount){
	this->name = name;
	this->commandCount = commandCount;
	this->commandCounter = 0;
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
	return finished;
}

void Process::setCore(int coreID) {
	cpuCoreID = coreID;
}

void Process::executeCurrentCommand() {
	std::cout << "Hello world from " << this->name << std::endl;
	moveToNextLine();
}

void Process::moveToNextLine() {
	this->commandCounter++;
}

