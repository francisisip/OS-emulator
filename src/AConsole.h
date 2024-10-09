#pragma once
#include <string>

class AConsole {
public:
	AConsole(std::string name);
	~AConsole();

	std::string getName();
	virtual void onExecute() = 0;
	virtual void display() = 0;
	virtual void process() = 0;

	std::string name;

	friend class ConsoleManager;
};