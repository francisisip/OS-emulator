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

void Scheduler::printSchedulerStatus() const{
    std::cout << "--------------------------------------------\n";
	std::cout << "Running processes:\n";
	const std::vector<std::shared_ptr<Process>>& processes = this->getProcessList();
	for (const auto& process : processes) {
		if (!process->isFinished()) {
			std::cout << std::left << std::setw(20) << process->getName()
				<< std::left << std::setw(30) << process->getTimeCreated();

			// Check if the process has been assigned a core
			if (process->getCPUCoreID() != -1) {
				std::cout << "Core:   " << std::setw(15) << process->getCPUCoreID();
				std::cout << std::left << std::setw(1) << process->getCommandCounter() << " / "
					<< process->getCommandCount() << "\n";
			}
			else {
				std::cout << "Core:   " << std::setw(15) << " "; // Adjust the width to maintain alignment
				std::cout << std::left << std::setw(1) << process->getCommandCounter() << " / "
					<< process->getCommandCount() << "\n";
			}
		}
	}

	std::cout << "\nFinished processes:\n";
	for (const auto& process : processes) {
		if (process->isFinished()) {
			std::cout << std::left << std::setw(20) << process->getName()
				<< std::left << std::setw(30) << process->getTimeCreated()
				<< "Core:   " << std::setw(15) << "Finished"
				<< std::left << std::setw(1) << process->getCommandCounter() << " / "
				<< process->getCommandCount() << "\n";
		}
	}
    std::cout << "--------------------------------------------\n";
}