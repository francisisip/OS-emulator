#pragma once
#include <unordered_map>
#include <memory>
#include <string>

#include "AConsole.h"
#include "MenuScreen.h"
#include "ProcessScreen.h"
#include "Scheduler.h"

class ConsoleManager {
public:
	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void run();
	void switchScreen(const std::string& name);
	void switchScreenBack();
	std::string createProcessScreen(const std::string& name);
	bool ifProcessScreenExistsAndNotFinished(const std::string& name);

private:
	ConsoleManager();
	~ConsoleManager();
	ConsoleManager& operator=(ConsoleManager const&) {return *this;};
	static ConsoleManager* instance;

	int pid_counter;
	std::unordered_map<std::string, int> consoleNameTracker;
	std::unordered_map<std::string, std::shared_ptr<AConsole>> consoles;
	std::shared_ptr<AConsole> previousConsole; 
	std::shared_ptr<AConsole> currentConsole;
};