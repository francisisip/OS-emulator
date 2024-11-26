#include "ResourceManager.h"
Config* currentConfig = nullptr;
ResourceManager* ResourceManager::instance = nullptr;
ConsoleManager* consoleManager = nullptr;

ResourceManager::ResourceManager(){
    
}

void ResourceManager::initialize(){
    instance = new ResourceManager();
    instance->running = false;
    instance->batchNum = 0;
}

ResourceManager* ResourceManager::getInstance(){
    return instance;
}

void ResourceManager::schedulerTestStart(){
    // do a loop, once stop happens, get out of loop
    currentConfig = Config::getInstance();
    Scheduler* scheduler = Scheduler::getInstance();
    ConsoleManager* consoleManager = ConsoleManager::getInstance();
    unsigned int frequency = currentConfig->getBatchProcessFreq();
    running = true;

    while (running)
    {
        std::string newName = "process_" + std::to_string(batchNum);
        newName = consoleManager->createProcessScreen(newName);

        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (frequency)));
        batchNum++;
    }
} 

bool ResourceManager::startSchedulerInThread(){
    if (running == false){
        std::thread schedulerTestThread(&ResourceManager::schedulerTestStart, this); 
        schedulerTestThread.detach();
        return false;
    } 
    else 
    {
        return true;
    }

}

void ResourceManager::initializeScheduler(){
    currentConfig = Config::getInstance();
    Scheduler* scheduler = Scheduler::getInstance();
    // Initializes the scheduler, together with the input from the config.txt

    int numCores = currentConfig->getNumCpu();
    std::string algorithm = currentConfig->getScheduler();
    unsigned int quantumCycles = currentConfig->getQuantumCycles();
    scheduler->initializeCores(numCores);
    scheduler->setSchedulerAlgorithm(algorithm);
    scheduler->setQuantumCycles(quantumCycles);
    scheduler->run();
}

bool ResourceManager::schedulerTestStop(){
    // stop scheduler test, if it's running
    if (running == true){
        running = false;
        return true;
    }
    return running;
}

long long ResourceManager::getTotalCPUTicks() {
    Scheduler* scheduler = Scheduler::getInstance();
    const std::vector<std::unique_ptr<CPUCoreWorker>>& coreList = scheduler->getCoreList();
    long long totalCPUTicks = 0;

    for (const auto& core: coreList) {
        std::cout << "TOTAL CPU TICK TO BE ADDED:" << core->getTotalCPUTicks() << std::endl;
        totalCPUTicks += core->getTotalCPUTicks();
    }

    return totalCPUTicks;
}

long long ResourceManager::getActiveCPUTicks() {
    Scheduler* scheduler = Scheduler::getInstance();
    const std::vector<std::unique_ptr<CPUCoreWorker>>& coreList = scheduler->getCoreList();
    long long activeCPUTicks = 0;

    for (const auto& core: coreList) {
        activeCPUTicks += core->getActiveCPUTicks();
    }

    return activeCPUTicks;
}

long long ResourceManager::getIdleCPUTicks() {
    Scheduler* scheduler = Scheduler::getInstance();
    const std::vector<std::unique_ptr<CPUCoreWorker>>& coreList = scheduler->getCoreList();
    long long idleCPUTicks = 0;

    for (const auto& core: coreList) {
        idleCPUTicks += core->getIdleCPUTicks();
    }

    return idleCPUTicks;
}