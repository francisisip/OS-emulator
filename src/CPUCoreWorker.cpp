#include "CPUCoreWorker.h"
#include "Scheduler.h"

#include <utility>

CPUCoreWorker::CPUCoreWorker(int coreId){
    this->coreId = coreId;
    assignedProcess = false;
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
        if(assignedProcess && currentProcess){
            runProcess();
            // ITS RUNNING
        }
        // ITS IDLE
        totalCPUTicks++;
        // sleep based off CPU Cycle delay
    }
}


void CPUCoreWorker::runProcess() {
    if (!currentProcess) return;
    
    Scheduler* scheduler = Scheduler::getInstance();
    Config* currentConfig = Config::getInstance();
    int processCycles;
    unsigned int delaysPerExec = currentConfig->getDelaysPerExec();
    std::string scheduleType = currentConfig->getScheduler();
    int quantumCycles = currentConfig->getQuantumCycles();

    if (scheduleType == "\"fcfs\"") {
        while(!currentProcess->isFinished()){
            currentProcess->executeCurrentCommand();
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * (delaysPerExec + 1)));
            totalCPUTicks++;
        }

        currentProcess.reset();
        assignedProcess = false;

    } else if (scheduleType == "\"rr\"") {
        while(!currentProcess->isFinished()) {
            // At every CPU cycle, check if the process reached the max no. of quantum cycles
            processCycles = currentProcess->getCycleCount();
            
            // If still under quantum cycles, execute the process
            if (processCycles <= quantumCycles) {
                currentProcess->executeCurrentCommand();
                currentProcess->incrementCycleCount();
                totalCPUTicks++;
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (delaysPerExec + 1)));
            } else {
                currentProcess->resetCycleCount();
                totalCPUTicks++;
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (delaysPerExec + 1))); 

                scheduler->requeueProcess(currentProcess);
                currentProcess.reset();
                assignedProcess = false;
                break;
            }
        }
        // If process finished during quantum, do not requeue
        if (currentProcess && currentProcess->isFinished()) {
            currentProcess.reset();  // Process finished, reset the pointer
            assignedProcess = false;
        }
    }
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