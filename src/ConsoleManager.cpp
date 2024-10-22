#include "ConsoleManager.h"

ConsoleManager* ConsoleManager::instance = nullptr;

ConsoleManager::ConsoleManager() {
    auto MAIN_MENU = std::make_shared<MenuScreen>();
    consoles[MAIN_MENU->getName()] = MAIN_MENU;
    consoleNameTracker[MAIN_MENU->getName()] = 1;
    currentConsole = MAIN_MENU;
}

ConsoleManager::~ConsoleManager() {
    destroy();
}

ConsoleManager* ConsoleManager::getInstance() {
    return instance;
}

void ConsoleManager::initialize() {
    instance = new ConsoleManager();
}

void ConsoleManager::destroy() {
    delete instance;
}

void ConsoleManager::run() {
    currentConsole->onExecute();
}


void ConsoleManager::createProcessScreen(const std::string& baseName) {
    std::string newName = "P_" + baseName;
    if (consoles.find(newName) != consoles.end()){
        int* count = &consoleNameTracker[newName];

        do {
            newName = "P_" + baseName + "-" + std::to_string(*count);
            (*count)++;
        } while (consoles.find(newName) != consoles.end());
    }
    
    consoles[newName] = std::make_shared<ProcessScreen>(std::make_shared<Process>(newName));
    consoleNameTracker[newName] = 1;
    switchScreen(newName);
}

void ConsoleManager::switchScreen(const std::string& name) {
    previousConsole = currentConsole;
    currentConsole = consoles[name];
    currentConsole->onExecute();
}

void ConsoleManager::switchScreenBack() {
    auto temp = currentConsole;
    currentConsole = previousConsole;
    previousConsole = temp;
    system("clear");
    currentConsole->onExecute();
}

bool ConsoleManager::ifProcessScreenExists(const std::string& name) {
    auto item = consoles.find(name);

    if (item == consoles.end())
        return false;
    else
        return true;
}