#pragma once
#include "Scheduler.h"

Scheduler::Scheduler() {
    // TODO: add constructor logic
}

void Scheduler::addProcess(const Process& process) {
    std::lock_guard<std::mutex> lock(processMutex);
    auto newProcess = std::make_shared<Process>(process); // Create a shared pointer for the new process
    processList.push_back(newProcess);
    std::lock_guard<std::mutex> queueLock(readyQueueMutex);
    readyQueue.push(newProcess);
}

void Scheduler::runFCFS() {
    while (running) {
        std::lock_guard<std::mutex> lock(readyQueueMutex);
        // Check if ready queue is not empty
        if (!readyQueue.empty()) {
            // If not empty, grab top process
            auto curProcess = readyQueue.front();
            readyQueue.pop();

            // Find a core that is available
            auto coreId = getAvailableCore();

            if (coreId >= 0) {
                curProcess->setCore(coreId);
                coreList[coreId - 1]->setCurrentProcess(curProcess);
            } 
        }
    }
};

void Scheduler::runScheduler() {
    // Continuously run scheduling algorithms
    while (running) {
        // For now, only run FCFS
        runFCFS();
    }
}

void Scheduler::initialize() {
    running = true;
    schedulerThread = std::thread(&Scheduler::runScheduler, this);
} 

void Scheduler::initializeCores(int coreNum) {
    for (int i = 0; i < coreNum; i++) {
        // Initialize a new core then add it to coreList 
        coreList.push_back(std::make_unique<CPUCoreWorker>(i));
        // Initialize the most recently created 
        coreList.back()->initialize();
    }
}

int Scheduler::getAvailableCore() {
    for (auto& core: coreList) {
        if (!core->hasCurrentProcess()) {
            return core->getCoreId();
        }
    }
    return -1;
}

const std::vector<std::unique_ptr<CPUCoreWorker>>& Scheduler::getCoreList() const {
    return coreList;
}

const std::vector<std::shared_ptr<Process>>& Scheduler::getProcessList() const {
    return processList;
}