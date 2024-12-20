#pragma once
#include <string>
#include <vector>

class AConsole {
public:
	AConsole(std::string name);
	~AConsole();

	std::string getName();
	virtual void onExecute() = 0;
	virtual void display() = 0;
	virtual void process() = 0;
	void clear();

	std::string name;
	std::vector<std::string> commandHistory;

	friend class ConsoleManager;
};