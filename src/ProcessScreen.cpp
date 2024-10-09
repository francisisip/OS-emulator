#include "ProcessScreen.h"
#include "ConsoleManager.h"

ProcessScreen::ProcessScreen(std::shared_ptr<Process> process): AConsole("P_" + process->getName()) {
}

ProcessScreen::~ProcessScreen() {
}

void ProcessScreen::onExecute() {
    display();
    process();
}

void ProcessScreen::display() {

}

void ProcessScreen::process() {

}


