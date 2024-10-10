#include "Process.h"
#include <thread>
#include <memory>

class CPUCoreWorker 
{
public:
    void initialize();

    void setCurrentProcess(std::shared_ptr<Process> process);
    bool hasCurrentProcess();
    

private:
    void runProcess();
    int coreId;
    std::thread coreThread;
    bool running;
    std::shared_ptr<Process> currentProcess; // if empty, hasCurrentProcess is false

    int totalCPUTicks = 0; // 1 cpu tick = 1 instruction line

};