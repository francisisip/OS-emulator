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

bool Paging::allocate(std::shared_ptr<Process> processToAllocate){
  int pid = processToAllocate->getPId();
  int pagesNeeded = processToAllocate->getPagesNeeded();

  // returning false
  auto it = pageTables.find(pid);
  if (it != pageTables.end()){
    return true; // process already in memory
  }   

  
  if (pagesNeeded > freeFrames.size()) {

      for (size_t i = 0; i < allocatedProcessOrder.size(); ++i) {
          std::shared_ptr<Process> process = allocatedProcessOrder[i];

          if (process->getState() != Process::RUNNING) {
              placeIntoBackingStore(process);

              // exit the inner loop
              if (pagesNeeded <= freeFrames.size())
                  break;
          }
      }

      // cannot free up enough frames for the new process, return false
      if (pagesNeeded > freeFrames.size())
          return false;
  }

  //check if process to be inserted was from backing store
  auto backStoreIt = std::find(backingStore.begin(), backingStore.end(), pid);

  if (backStoreIt != backingStore.end()) {
      pagedOut += pagesNeeded;
      backingStore.erase(backStoreIt);
  }

  for (int i = 0; i < pagesNeeded; i++){
    int frame = findFreeFrame();
    memory[frame] = pid;
    pageTables[pid][i] = frame;
  }

  allocatedSize += pagesNeeded;
  allocatedProcessOrder.push_back(processToAllocate);
  return true;
}

void Paging::deallocate(std::shared_ptr<Process> processToDeallocate){
  int pid = processToDeallocate->getPId();
  auto it = pageTables.find(pid);

  if (it == pageTables.end()) return; // process not found

  for (auto& page : it->second){
    memory[page.second] = -1;
    freeFrames.push_back(page.second);
  }

  allocatedSize -= it->second.size();
  pageTables.erase(it);

  //remove the process in the allocated order queue
  auto index = std::find(allocatedProcessOrder.begin(), allocatedProcessOrder.end(), processToDeallocate);
  if (index != allocatedProcessOrder.end()) {
      allocatedProcessOrder.erase(index); 
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

void Paging::placeIntoBackingStore(std::shared_ptr<Process> process) {
    backingStore.push_back(process->getPId());
    deallocate(process);
    pagedIn += process->getPagesNeeded();
}

int Paging::getPageIn() {
    return pagedIn;
}

int Paging::getPageOut() {
    return pagedOut;
}
