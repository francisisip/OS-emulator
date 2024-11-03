#pragma once
# include "AConsole.h"
#include <fstream>
#include <filesystem>

class MenuScreen : public AConsole {
public:
	MenuScreen();
	~MenuScreen();

	void onExecute() override;
	void display() override; 
	void process() override;
	bool getInitialized();
	void setInitialized(bool);

private:
	void displayASCII();
	void handleInput(std::string command);
	bool isInitialized;
};
