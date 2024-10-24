#include "ResourceManager.h"

Config* currentConfig = nullptr;

ResourceManager* ResourceManager::instance = nullptr;
Config* configInstance;
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
    unsigned int frequency = currentConfig->getBatchProcessFreq();
    running = true;

    while (running)
    {
        Process newProcess = Process("process_" + std::to_string(batchNum));
        scheduler->addProcess(newProcess);

        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (frequency+1)));
        batchNum++;
    }
} 

void ResourceManager::startSchedulerInThread(){
    if (running == true){
        std::cout << "Scheduler test is already running" << std::endl;
        return;
    } else {
        std::thread schedulerTestThread(&ResourceManager::schedulerTestStart, this); 
        schedulerTestThread.detach();  
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
void ResourceManager::schedulerTestStop(){
    // stop scheduler test, if it's running
    running = false;
}