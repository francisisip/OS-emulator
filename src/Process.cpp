#include "Process.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

Config* config = nullptr;

// Constructor for Process without specified memory size
Process::Process(std::string name, int pid){
	this->pid = pid;
	this->name = name;
  this->memoryRequired = setMemoryRequired();
	this->commandCount = setCommandCount();
	this->commandCounter = 0;
	this->cycleCount = 0;
	timeCreated = std::chrono::system_clock::now();
	currentState = ProcessState::READY;
}

// Constructor for Process with specified memory size
Process::Process(std::string name, int pid, size_t memoryRequired){
	this->pid = pid;
	this->name = name;
	this->memoryRequired = memoryRequired;
	this->commandCount = setCommandCount();
	this->commandCounter = 0;
	this->cycleCount = 0;
	timeCreated = std::chrono::system_clock::now();
	currentState = ProcessState::READY;
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
	oss << std::put_time(std::localtime(&now_c), "(%m/%d/%Y %I:%M:%S%p)");
	return oss.str(); // Return current date and time as a string
}
std::string Process::getTimeCreated() const {
	auto time_c = std::chrono::system_clock::to_time_t(timeCreated);
	std::ostringstream oss;
	oss << std::put_time(std::localtime(&time_c), "(%m/%d/%Y %I:%M:%S%p)");
	return oss.str(); // Return formatted creation time
}
int Process::getCommandCounter() const {
	return commandCounter;
}

int Process::getCommandCount() const {
	return commandCount;
};

int Process::getCycleCount() const {
	return cycleCount;
}

size_t Process::getMemoryRequired() const {
	return memoryRequired;
}

bool Process::isFinished() const {
	if (currentState == ProcessState::FINISHED) {
		return true;
	}
	return false;
}

void Process::setCore(int coreID) {
	cpuCoreID = coreID;
}

void Process::resetCore() {
	cpuCoreID = -1;
}

unsigned int Process::setMemoryRequired(){
  Config* config = Config::getInstance();

	int lower_boundary = config->getMinMemoryPerProcess();
	int upper_boundary = config->getMaxMemoryPerProcess();

	std::random_device rd;  // Seed for the generator
	std::mt19937 gen(rd()); // Mersenne Twister generator
	std::uniform_int_distribution<> dis(lower_boundary, upper_boundary);
	int memoryRequired = dis(gen);

  return memoryRequired;
}
unsigned int Process::setCommandCount() {
	Config* config = Config::getInstance();

	int lower_boundary = config->getMinIns();
	int upper_boundary = config->getMaxIns();

	std::random_device rd;  // Seed for the generator
	std::mt19937 gen(rd()); // Mersenne Twister generator
	std::uniform_int_distribution<> dis(lower_boundary, upper_boundary);
	commandCount = dis(gen);

	return commandCount;
}

void Process::incrementCycleCount() {
	cycleCount++;
}

void Process::resetCycleCount() {
	cycleCount = 0;
}

void Process::executeCurrentCommand() {
	if (commandCounter < commandCount) {
		moveToNextLine();
	} else {
		currentState = ProcessState::FINISHED;
	}
}
void Process::moveToNextLine() {
	commandCounter++;
}
