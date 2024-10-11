#include "CPUCoreWorker.h"

CPUCoreWorker::CPUCoreWorker(int coreId){
    this->coreId = coreId;
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
        }
        totalCPUTicks++;
    }
}

void CPUCoreWorker::runProcess(){
    // FCFS
    for(int i = 0; i < currentProcess->getCommandCount(); i++){
        currentProcess->executeCurrentCommand();
    }
    currentProcess.reset();
    assignedProcess = false;
}

void CPUCoreWorker::setCurrentProcess(std::shared_ptr<Process> process){
    std::lock_guard<std::mutex> lock(coreMutex);
    // TODO: public method to set the current process stored in this CPU to the process. handled by scheduler
    currentProcess = process;
    assignedProcess = true;
}

bool CPUCoreWorker::hasCurrentProcess(){
    std::lock_guard<std::mutex> lock(coreMutex);
    // TODO: check if there is a process assigned
    return assignedProcess;
}