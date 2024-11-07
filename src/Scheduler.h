#pragma once

#include <vector>
#include <queue>
#include "Process.h"
#include "CPUCoreWorker.h"

class Scheduler {
public:
    Scheduler();

    static void initialize();
    static Scheduler* getInstance();
    void run();
    void initializeCores(int numCores);

    std::shared_ptr<Process> addProcess(const Process& process);
    void requeueProcess(std::shared_ptr<Process> process); 

    const std::vector<std::unique_ptr<CPUCoreWorker>>& getCoreList() const;
    const std::vector<std::shared_ptr<Process>>& getProcessList() const;
    int getAvailableCore();

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