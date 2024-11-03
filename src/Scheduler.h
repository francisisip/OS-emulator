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
    std::shared_ptr<Process> addProcess(const Process& process);

    // Endlessly runs the scheduler
    void run();
    
    
    // Initialize all the cores within the scheduler
    void initializeCores(int numCores);


    // Find an available core
    int getAvailableCore();

    const std::vector<std::unique_ptr<CPUCoreWorker>>& getCoreList() const;
    const std::vector<std::shared_ptr<Process>>& getProcessList() const;

    void requeueProcess(std::shared_ptr<Process> process); 

    void setSchedulerAlgorithm(std::string algorithm);
    void setQuantumCycles(unsigned int cycles);
    
    std::string getSchedulerStatus() const;

private:
    void startSchedulerLoop();
    void schedFCFS();
    void schedRR();

    bool running = false;
    static Scheduler* instance;
    unsigned int quantumCycles;
    std::string schedulerAlgo;
    std::vector<std::unique_ptr<CPUCoreWorker>> coreList;
    std::vector<std::shared_ptr<Process>> processList;
    std::queue<std::shared_ptr<Process>> readyQueue;
    std::vector<std::shared_ptr<Process>> completedProcesses;
    
    std::mutex readyQueueMutex; // Protect access to the readyQueue
    std::mutex processMutex; // Protect access to the list of processes
    std::thread schedulerThread;
};