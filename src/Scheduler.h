#pragma once

#include <vector>
#include <queue>
#include "Process.h"
#include "CPUCoreWorker.h"

class Scheduler {
public:
    Scheduler();

    // Add a Process
    void addProcess(const Process& process);

    // Access the core workers
    // Access all the processes

    // Endlessly runs the scheduler
    void runScheduler();

    // Initialize the scheduler
    void initialize();

    // Initialize all the cores within the scheduler
    void initializeCores(int numCores);
private:
    bool running;
    std::vector<std::unique_ptr<CPUCoreWorker>> coreList;
    std::vector<std::shared_ptr<Process>> processList;
    std::queue<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> completedProcesses;
    
    std::mutex readyQueueMutex; // Protect access to the readyQueue
    std::mutex processMutex; // Protect access to the list of processes
    std::thread schedulerThread;

    void runFCFS();

};