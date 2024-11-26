#include "Scheduler.h"
#include "FlatMemoryAllocator.h"
#include <iomanip>

Scheduler* Scheduler::instance = nullptr;
FlatMemoryAllocator* allocator = nullptr;

Scheduler::Scheduler() {
}

void Scheduler::initialize() {
    instance = new Scheduler();
} 

Scheduler* Scheduler::getInstance(){
    return instance;
};

void Scheduler::run() {
    running = true;
    schedulerThread = std::thread(&Scheduler::startSchedulerLoop, this);
}

void Scheduler::initializeCores(int coreNum) {
    for (int i = 0; i < coreNum; i++) {
        // Initialize a new core then add it to coreList 
        coreList.push_back(std::make_unique<CPUCoreWorker>(i));
        // Initialize the most recently created 
        coreList.back()->initialize();
    }
}

std::shared_ptr<Process> Scheduler::addProcess(const Process& process) {
    std::lock_guard<std::mutex> lock(processMutex);
    std::shared_ptr<Process> newProcess = std::make_shared<Process>(process); // Create a shared pointer for the new process
    processList.push_back(newProcess);
    std::lock_guard<std::mutex> queueLock(readyQueueMutex);
    readyQueue.push(newProcess);

    return newProcess;
}

void Scheduler::requeueProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(readyQueueMutex);
    process->resetCore();
    readyQueue.push(process);
}

// TODO: Implement a first-fit memory checker. and refactor to "allocateMemory"
bool Scheduler::allocateMemoryForProcess(std::shared_ptr<Process> processToAllocate) {
    allocator = FlatMemoryAllocator::getInstance();

    if (!allocator) {
        std::cerr << "Error: Allocator not initialized.\n";
        return false;
    } 
    else return allocator->allocate(processToAllocate);


}

const std::vector<std::unique_ptr<CPUCoreWorker>>& Scheduler::getCoreList() const {
    return coreList;
}

const std::vector<std::shared_ptr<Process>>& Scheduler::getProcessList() const {
    return processList;
}

std::string Scheduler::getSchedulerStatus() const{
    std::string string_ls = "";

    int coresUsed = 0;
	const std::vector<std::unique_ptr<CPUCoreWorker>>& cores = getCoreList();
	for (const auto& core : cores) {
		if (core->hasCurrentProcess()) {
			coresUsed++;
		}
	}
    int cpuCount = getCoreList().size();
    double cpuUtilization = (cpuCount > 0) ? (static_cast<double>(coresUsed) / cpuCount) * 100 : 0.0;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << cpuUtilization;
    std::string formattedUtilization = oss.str();

    string_ls += "CPU utilization: " + formattedUtilization  + "%\n";
    string_ls += "Cores used: " + std::to_string(coresUsed) + "\n";
    string_ls += "Cores available: " + std::to_string(cpuCount - coresUsed) + "\n";
    string_ls += "\n--------------------------------------------\n";

    string_ls += "Running processes:\n";
    const std::vector<std::shared_ptr<Process>>& processes = this->getProcessList();
        for (const auto& process : processes) {
            if (!process->isFinished()) {
                std::ostringstream oss;
                oss << std::left << std::setw(20) << process->getName()
                    << std::left << std::setw(30) << process->getTimeCreated();

                if (process->getCPUCoreID() != -1) {
                    oss << "Core:   " << std::setw(15) << process->getCPUCoreID();
                    oss << std::left << std::setw(1) << process->getCommandCounter() << " / "
                        << process->getCommandCount() << "\n";
                } else {
                    oss << "Core:   " << std::setw(15) << " "; 
                    oss << std::left << std::setw(1) << process->getCommandCounter() << " / "
                        << process->getCommandCount() << "\n";
                }
                string_ls += oss.str(); 
            }
        }

    string_ls += "\nFinished processes:\n"; 
    for (const auto& process : processes) {
        if (process->isFinished()) {
            std::ostringstream oss; 
            oss << std::left << std::setw(20) << process->getName()
                << std::left << std::setw(30) << process->getTimeCreated()
                << "Core:   " << std::setw(15) << "Finished"
                << std::left << std::setw(1) << process->getCommandCounter() << " / "
                << process->getCommandCount() << "\n";
            string_ls += oss.str(); 
        }
    }
    string_ls += "--------------------------------------------";

    return string_ls;
}

int Scheduler::getAvailableCore() {
    for (auto& core: coreList) {
        if (!core->hasCurrentProcess()) {
            return core->getCoreId();
        }
    }

    return -1;
}


void Scheduler::setSchedulerAlgorithm(std::string algorithm) {
    schedulerAlgo = algorithm;
}

void Scheduler::setQuantumCycles(unsigned int cycles) {
    quantumCycles = cycles;
}

void Scheduler::startSchedulerLoop() {
    // Continuously run scheduling algorithms
    if (schedulerAlgo == "rr") {
        while (running) {
            schedRR();
        }
    } else {
        while (running) {
            schedFCFS();
        }
    }
}

void Scheduler::schedFCFS() {
    while (running) {
        std::lock_guard<std::mutex> lock(readyQueueMutex);
        // Check if ready queue is not empty
        if (!readyQueue.empty()) {
            // If not empty, grab top process
            auto curProcess = readyQueue.front();

            // Find a core that is available
            auto coreId = getAvailableCore();

            // TODO: call the memory checker here

            if (coreId != -1) {
                // check if a process is already allocated memory
                readyQueue.pop();
            
                // if it can allocate memory, but need to check if process is already allocated
                if(allocateMemoryForProcess(curProcess)){
                    curProcess->setCore(coreId);
                    coreList[coreId]->setCurrentProcess(curProcess);
                }
                else readyQueue.push(curProcess);
            }
        }
    }
}

void Scheduler::schedRR() {
    while (running) {
        std::lock_guard<std::mutex> lock(readyQueueMutex);

        // Check if ready queue is not empty
        if (!readyQueue.empty()) {
            // If not empty, grab top process
            auto curProcess = readyQueue.front();

            // Find available core
            auto coreId = getAvailableCore();

            // TODO: call the memory checker here
            if (coreId != -1) {
                // check if a process is already allocated memory
                readyQueue.pop();
            
                // if it can allocate memory, but need to check if process is already allocated
                if(allocateMemoryForProcess(curProcess)){
                    curProcess->setCore(coreId);
                    coreList[coreId]->setCurrentProcess(curProcess);
                }
                else readyQueue.push(curProcess);
            }
        }
    }
}

