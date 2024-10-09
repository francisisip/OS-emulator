#pragma once
#include <string>

class Process {
public:
	Process(std::string name);
	std::string getName();
	std::string name;
};
