#include "Scheduler.h"
#include <iomanip>

Scheduler* Scheduler::instance = nullptr;
Scheduler::Scheduler() {
    // TODO: add constructor logic
}

Scheduler* Scheduler::getInstance(){
    return instance;
};

std::shared_ptr<Process> Scheduler::addProcess(const Process& process) {
    std::lock_guard<std::mutex> lock(processMutex);
    std::shared_ptr<Process> newProcess = std::make_shared<Process>(process); // Create a shared pointer for the new process
    processList.push_back(newProcess);
    std::lock_guard<std::mutex> queueLock(readyQueueMutex);
    readyQueue.push(newProcess);

    return newProcess;
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

            if (coreId != -1) {
                readyQueue.pop();
                curProcess->setCore(coreId);
                coreList[coreId]->setCurrentProcess(curProcess);
            }
        }
    }
};

void Scheduler::schedRR() {
    while (running) {
        std::lock_guard<std::mutex> lock(readyQueueMutex);

        // Check if ready queue is not empty
        if (!readyQueue.empty()) {
            // If not empty, grab top process
            auto curProcess = readyQueue.front();

            // Find available core
            auto coreId = getAvailableCore();

            if (coreId != -1) {
                // TODO: maybe rr doesn't need it's own function?
                readyQueue.pop();
                curProcess->setCore(coreId);
                coreList[coreId]->setCurrentProcess(curProcess);
            }
        }
    }
};

void Scheduler::initialize() {
    instance = new Scheduler();
} 

void Scheduler::run() {
    running = true;
    schedulerThread = std::thread(&Scheduler::startSchedulerLoop, this);
}
// void ConsoleManager::initialize() {
//     instance = new ConsoleManager();
// }


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


void Scheduler::initializeCores(int coreNum) {
    for (int i = 0; i < coreNum; i++) {
        // Initialize a new core then add it to coreList 
        coreList.push_back(std::make_unique<CPUCoreWorker>(i));
        // Initialize the most recently created 
        coreList.back()->initialize();
    }
}

int Scheduler::getAvailableCore() {
    for (auto& core: coreList) {
        if (!core->hasCurrentProcess()) {
            return core->getCoreId();
        }
    }
    return -1;
}

const std::vector<std::unique_ptr<CPUCoreWorker>>& Scheduler::getCoreList() const {
    return coreList;
}

const std::vector<std::shared_ptr<Process>>& Scheduler::getProcessList() const {
    return processList;
}

void Scheduler::requeueProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(readyQueueMutex);
    process->resetCore();
    readyQueue.push(process);
}

void Scheduler::setSchedulerAlgorithm(std::string algorithm) {
    schedulerAlgo = algorithm;
}

void Scheduler::setQuantumCycles(unsigned int cycles) {
    quantumCycles = cycles;
}

std::string Scheduler::printSchedulerStatus() const{
    std::string string_ls = "\n";

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
    string_ls += "Cores Used: " + std::to_string(coresUsed) + "\n";
    string_ls += "Cores available: " + std::to_string(cpuCount - coresUsed) + "\n";
    string_ls += "--------------------------------------------\n\n";

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
    string_ls += "--------------------------------------------\n\n";

    std::cout << string_ls; // Print accumulated string

    return string_ls;

}