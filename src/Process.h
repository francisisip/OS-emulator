#pragma once
#include <vector>
#include <chrono>
#include <string>
#include <random>
#include "ICommand.h"
#include "Config.h"

class Process {
public:

	enum ProcessState {
		READY,
		RUNNING,
		WAITING,
		FINISHED
	};
	Process(std::string name);

	// Getters
	int getPId() const;
	std::string getName() const;
	int getCPUCoreID() const;
	ProcessState getState() const;
	std::string getTimeCreated() const;
	std::string getCurrentCommandTime() const;
	int getCommandCounter() const; 
	int getCommandCount() const;
	int getCycleCount() const;
	bool isFinished() const;
	void setCore(int coreID);
	void resetCore();
	unsigned int setCommandCount();
	void incrementCycleCount();
	void resetCycleCount();
	
	void executeCurrentCommand();
	void moveToNextLine();


private:
	int pid;
	std::string name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;

	int cpuCoreID = -1;
	ProcessState currentState;
	std::chrono::system_clock::time_point timeCreated;
	int commandCounter; // indicate current command
	int commandCount; // total no. of commands
	int cycleCount; // no. of current cycles taken (for RR scheduling)
};
