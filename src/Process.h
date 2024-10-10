#pragma once
#include <string>

class Process {
public:

	enum ProcessState {
		READY,
		RUNNING,
		WAITING,
		FINISHED
	};

	Process(std::string name);

	void executeCurrentCommand();
	void moveToNextLine();

	bool isFinished();	
	int getCommandCounter();
	int getLinesOfCode();
	int getCPUCoreID();
	ProcessState getState();
	std::string getName();

private:
	std::string name;

	int commandCounter;
	int cpuCoreID = -1;
	ProcessState currentState;
};
