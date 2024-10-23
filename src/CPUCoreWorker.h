#include "Process.h"
#include <thread>
#include <memory>
#include <chrono>
#include <mutex>
class CPUCoreWorker 
{
public:
    CPUCoreWorker(int coreId);
    ~CPUCoreWorker();
    // initializes the core worker, runs the runCoreWorker command 
    void initialize();
    // Get coreId
    int getCoreId();
    // sets the current process of the core worker
    void setCurrentProcess(std::shared_ptr<Process> process);
    // boolean to check if there is a process
    bool hasCurrentProcess();
    

private:
    // runs the current process assigned
    void runProcess();
    void runCoreWorker();
    void stop();
    int coreId;
    std::thread coreThread;
    bool running;
    bool assignedProcess;
    std::shared_ptr<Process> currentProcess; // if empty, hasCurrentProcess is false
    std::mutex coreMutex;
    int totalCPUTicks = 0; // 1 cpu tick = 1 instruction line

};