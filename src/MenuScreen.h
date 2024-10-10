#pragma once
# include "AConsole.h"

class MenuScreen : public AConsole {
public:
	MenuScreen();
	~MenuScreen();

	void onExecute() override;
	void display() override; 
	void process() override;

private:
	void displayASCII();
	void handleInput(std::string command);
};
