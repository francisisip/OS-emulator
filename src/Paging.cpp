#include "Paging.h"

Paging* Paging::instance = nullptr;

Paging::Paging(){
 
}

Paging::~Paging(){
  memory.clear();
  pageTables.clear();
  freeFrames.clear();
}

Paging* Paging::getInstance(){
  return instance;
}

void Paging::initialize(){
  instance = new Paging();

  // Get the maximum memory and memory per frame from the config file 
  unsigned int maxMemory = Config::getInstance()->getMaxMemory();
  unsigned int memoryPerFrame = Config::getInstance()->getMemoryPerFrame();

  // compute the number of frames
  instance->maxPages = maxMemory / memoryPerFrame;
  // initialize memory to -1 for all memory locations
  instance->memory.resize(instance->maxPages, -1);
  // initialize the number of free frames 
  for (int i = 0; i < instance->maxPages; i++) instance->freeFrames.push_back(i);
}

int Paging::findFreeFrame(){
  if (freeFrames.empty()) return -1;
  int frame = freeFrames.back();
  freeFrames.pop_back();
  return frame;
}

size_t Paging::getMaxPages(){
  return maxPages;
}

bool Paging::allocate(std::shared_ptr<Process> processToAllocate) {
    int pid = processToAllocate->getPId();
    int pagesNeeded = processToAllocate->getPagesNeeded();

    bool processInMemory = false;
    bool needsEviction = false;

    { 
        std::lock_guard<std::mutex> lock(allocationMutex); 

        // Check if process is already in memory
        if (pageTables.find(pid) != pageTables.end()) {
            processInMemory = true;
        } else if (pagesNeeded > freeFrames.size()) {
            needsEviction = true;
        }
    }

    if (processInMemory) return true;

    if (needsEviction) {
        // Evict processes outside the lock
        for (size_t i = 0; i < allocatedProcessOrder.size(); ++i) {
            std::shared_ptr<Process> process = allocatedProcessOrder[i];
            if (process != nullptr && process->getCPUCoreID() == -1) {
                placeIntoBackingStore(process);
                if (pagesNeeded <= freeFrames.size()) break;
            }
        }
    }

    {
        std::lock_guard<std::mutex> lock(allocationMutex); 

        if (pagesNeeded > freeFrames.size()) {
            return false; // Not enough memory after eviction
        }

        // Allocate frames for the process
        for (int i = 0; i < pagesNeeded; ++i) {
            int frame = findFreeFrame();
            memory[frame] = pid;
            pageTables[pid][i] = frame;
        }
        allocatedSize += processToAllocate->getMemoryRequired();
        allocatedProcessOrder.push_back(processToAllocate);

        // Check if process was previously in the backing store
        auto backStoreIt = std::find(backingStore.begin(), backingStore.end(), pid);
        if (backStoreIt != backingStore.end()) {
            pagedOut += pagesNeeded;
            backingStore.erase(backStoreIt);

            // Remove the process from the backing store file
            std::ifstream backingStoreFile("backing_store.txt");
            std::ofstream tempFile("temp.txt");
            if (backingStoreFile.is_open() && tempFile.is_open()) {
                std::string line;
                while (std::getline(backingStoreFile, line)) {
                    std::istringstream iss(line);
                    std::string pId;
                    iss >> pId;
                    if (std::stoi(pId) != pid) {
                        tempFile << line << std::endl;
                    }
                }
                backingStoreFile.close();
                tempFile.close();
                std::remove("backing_store.txt");
                std::rename("temp.txt", "backing_store.txt");
            } else {
                std::cerr << "Error: Unable to open backing_store.txt for reading.\n";
            }
        }
    }

    return true;
}


void Paging::deallocate(std::shared_ptr<Process> processToDeallocate) {
    int pid = processToDeallocate->getPId();

    {
        std::lock_guard<std::mutex> lock(allocationMutex); 
        auto it = pageTables.find(pid);
        if (it == pageTables.end()) return; // Process not found

        for (auto& page : it->second) {
            memory[page.second] = -1;
            freeFrames.push_back(page.second);
        }

        allocatedSize -= processToDeallocate->getMemoryRequired();
        pageTables.erase(it);
    }

    // Update the allocated process order outside the lock
    {
        std::lock_guard<std::mutex> lock(allocationMutex);
        auto index = std::find(allocatedProcessOrder.begin(), allocatedProcessOrder.end(), processToDeallocate);
        if (index != allocatedProcessOrder.end()) {
            allocatedProcessOrder.erase(index);
        }
    }
}


void Paging::visualizeMemory() {
    for (size_t i = 0; i < memory.size(); ++i) {
        std::cout << "Frame " << i << ": ";
        if (memory[i] == -1) {
            std::cout << "Free" << std::endl;
        } else {
            std::cout << "Process " << memory[i] << std::endl;
        }
    }
}


size_t Paging::getAllocatedSize(){ return allocatedSize; }

void Paging::placeIntoBackingStore(std::shared_ptr<Process> process) {
    {
        std::lock_guard<std::mutex> lock(allocationMutex);
        backingStore.push_back(process->getPId());
    }

    
    //write into file
    {
        std::ofstream backingStoreFile("backing_store.txt", std::ios::app); // Open in append mode
        if (backingStoreFile.is_open()) {
            backingStoreFile << process->getPId() << ", " << process->getCommandCounter() << "\n";
        } else {
            std::cerr << "Error: Unable to open backing_store.txt for writing.\n";
        }
    }

    // Call deallocate outside the mutex lock
    deallocate(process);

    {
        std::lock_guard<std::mutex> lock(allocationMutex);
        pagedIn += process->getPagesNeeded();
    }
}

int Paging::getPageIn() {
    return pagedIn;
}

int Paging::getPageOut() {
    return pagedOut;
}
