#include "ResourceManager.h"

ResourceManager* instance;
ResourceManager::ResourceManager(){
    
}

void ResourceManager::initialize(){
    instance = new ResourceManager();
}
ResourceManager* ResourceManager::getInstance(){
    return instance;
}

void ResourceManager::schedulerTestStart(){
    // do a loop, once stop happens, get out of loop
    // while (/* condition */)
    {
        // TODO: do this
        /* code */
        // create a process
        // get values from Config class.
        // random values
        // Process newProcess = new Process("test_print") 
        // sleep for getBatchProcessFreq from config
        // create a process based on these values
        // add it to the ready queue of Scheduler
        // Scheduler::addProcess()
        // sleep this thread CPU cycle delay times value in config.txt
    }
}

void ResourceManager::schedulerTestStop(){
    // stop scheduler test, if it's running

}