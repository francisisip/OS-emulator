#include "CPUCoreWorker.h"

#include <utility>

CPUCoreWorker::CPUCoreWorker(int coreId){
    this->coreId = coreId;
    this->assignedProcess = false;
}

CPUCoreWorker::~CPUCoreWorker() {
    stop();
    if (coreThread.joinable()) {
        coreThread.join();
    }
}
void CPUCoreWorker::initialize() {
    // initialize a new thread for this CPU
    // NOTES use isFinished in Process 
    running = true;
    // add a thread that does a function
    coreThread = std::thread(&CPUCoreWorker::runCoreWorker, this);
    // REMEMBER: process allocation will be handled by the scheduler

}

int CPUCoreWorker::getCoreId() {
    std::lock_guard<std::mutex> lock(coreMutex);
    return coreId;
}

void CPUCoreWorker::runCoreWorker(){
    while(true){
        if(assignedProcess){
            runProcess();
            // ITS RUNNING
        }
        // ITS IDLE
        totalCPUTicks++;
        // sleep based off CPU Cycle delay
    }
}

void CPUCoreWorker::runProcess(){
    // FCFS
    unsigned int delay = Config::getInstance()->getDelaysPerExec();
    while(!currentProcess->isFinished()){
        currentProcess->executeCurrentCommand();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (delay+1)));
        totalCPUTicks++;
    }
    currentProcess.reset();
    assignedProcess = false;
}

void CPUCoreWorker::setCurrentProcess(std::shared_ptr<Process> process){
    std::lock_guard<std::mutex> lock(coreMutex);
    currentProcess = process;
    assignedProcess = true;
}

bool CPUCoreWorker::hasCurrentProcess(){
    std::lock_guard<std::mutex> lock(coreMutex);
    return assignedProcess;
}

void CPUCoreWorker::stop() {
    {
        std::lock_guard<std::mutex> lock(coreMutex);
        running = false;
    }
}