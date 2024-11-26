#pragma once

#include "Scheduler.h"
#include "Process.h"
#include "ConsoleManager.h"
#include "Config.h"
#include <thread>

class ResourceManager{
    // This is going to be a singleton also. para everything can access it
public:
    ResourceManager();
    static ResourceManager* getInstance();
    static void initialize();

    
    // spam add process with Scheduler::addProcess. 
    void schedulerTestStart();
    bool startSchedulerInThread();
    void initializeScheduler();
    bool schedulerTestStop(); // this is the scheduler-stop command. sounds weird so I just made it test stop
    long long getTotalCPUTicks();
    long long getActiveCPUTicks();
    long long getIdleCPUTicks();

private:
   static ResourceManager* instance;
   bool running;
   int batchNum;
};
