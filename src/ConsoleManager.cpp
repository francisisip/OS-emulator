#include "ConsoleManager.h"

ConsoleManager* ConsoleManager::instance = nullptr;

ConsoleManager::ConsoleManager() {
    this->pid_counter = 0;
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


std::string ConsoleManager::createProcessScreen(const std::string& baseName) {
    Scheduler* scheduler = Scheduler::getInstance();
    pid_counter++;

    std::string newName = baseName;
    if (consoles.find(newName) != consoles.end()){
        int* count = &consoleNameTracker[newName];

        do {
            newName = baseName + "-" + std::to_string(*count);
            (*count)++;
        } while (consoles.find(newName) != consoles.end());
    }
    
    // TODO: add a range for the process memory allocation
    size_t memProcess = Config::getInstance()->getMemoryPerProcess();
    Process newProcess(newName, pid_counter, memProcess);
    auto sharedProcessAddress = scheduler->addProcess(newProcess);
    consoles[newName] = std::make_shared<ProcessScreen>(sharedProcessAddress);
    consoleNameTracker[newName] = 1;

    return newName;
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

bool ConsoleManager::ifProcessScreenExistsAndNotFinished(const std::string& name) {
    auto item = consoles.find(name);

    if (item == consoles.end()) {
        return false;
    }
    else {
        auto processScreen = std::dynamic_pointer_cast<ProcessScreen>(item->second);
        return processScreen->isFinished();
    }
}