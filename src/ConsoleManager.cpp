#include "ConsoleManager.h"
#include <iostream>

ConsoleManager* ConsoleManager::instance = nullptr;

ConsoleManager::ConsoleManager() {
    auto MAIN_MENU = std::make_shared<MenuScreen>();
    consoles[MAIN_MENU->getName()] = MAIN_MENU;
    consoleNameTracker[MAIN_MENU->getName()] = 1;
    currentConsole = MAIN_MENU;
}

ConsoleManager::~ConsoleManager() {

}

ConsoleManager* ConsoleManager::getInstance() {
    return instance;
}

void ConsoleManager::initialize() {
    system("cls");
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


    // if (consoles.count(newName) > 0) {

    //     int& count = consoleNameTracker[baseName];

    //     do {
    //         newName = "P_" + baseName + "-" + std::to_string(count);
    //         count++;
    //     } while (consoles.count(newName) > 0);
    // }
    
    auto newProcess = std::make_shared<ProcessScreen>(std::make_shared<Process>(newName));
    auto temp = std::make_shared<MenuScreen>();
    //consoles[newName] = temp;
    std::cout << "Process created: " << newName << std::endl;
    consoleNameTracker[newName] = 1;
}

void ConsoleManager::switchScreen(const std::string& name) {
    previousConsole = currentConsole;
    currentConsole = consoles[name];
    system("cls");
    currentConsole->onExecute();
}

void ConsoleManager::switchScreenBack() {
    auto temp = currentConsole;
    currentConsole = previousConsole;
    previousConsole = temp;
    system("cls");
    currentConsole->onExecute();
}

bool ConsoleManager::ifProcessScreenExists(const std::string& name) {
    auto item = consoles.find(name);

    if (item == consoles.end())
        return false;
    else
        return true;
}