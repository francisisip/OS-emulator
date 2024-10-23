#pragma once

#include <vector>
#include <queue>
#include "Process.h"
#include "CPUCoreWorker.h"

class Scheduler {
public:
    Scheduler();

    // Getters
    static Scheduler* getInstance();
    static void initialize();
    // Add a Process
    void addProcess(const Process& process);

    // Endlessly runs the scheduler
    void run();
    
    // Initialize the scheduler
    
    // Initialize all the cores within the scheduler
    void initializeCores(int numCores);


    // Find an available core
    int getAvailableCore();

    const std::vector<std::unique_ptr<CPUCoreWorker>>& getCoreList() const;
    const std::vector<std::shared_ptr<Process>>& getProcessList() const;

    void printSchedulerStatus() const;

    // TODO: All you enrique ;)
    void setSchedulerAlgorithm();
    void setQuantumCycles();
    
private:
    void startSchedulerLoop();
    void runFCFS();


    bool running = false;
    static Scheduler* instance;
    std::vector<std::unique_ptr<CPUCoreWorker>> coreList;
    std::vector<std::shared_ptr<Process>> processList;
    std::queue<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> completedProcesses;
    
    std::mutex readyQueueMutex; // Protect access to the readyQueue
    std::mutex processMutex; // Protect access to the list of processes
    std::thread schedulerThread;

    

};