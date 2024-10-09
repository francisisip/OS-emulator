#include "Process.h"

Process::Process(std::string name) {
	this->name = name;
}

std::string Process::getName() {
	return this->name;
}