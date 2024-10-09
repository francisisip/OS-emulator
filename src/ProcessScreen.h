#pragma once
#include <memory>
#include "AConsole.h"
#include "Process.h"

class ProcessScreen : public AConsole {
public:
	ProcessScreen(std::shared_ptr<Process> process);
	~ProcessScreen();

	void onExecute() override; 
	void display() override;
	void process() override; 

private:
	std::shared_ptr<Process> currentProcess;
	void handleInput(std::string command);	
};