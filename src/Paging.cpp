#include "Paging.h"
#include "Config.h"

Paging::Paging(){
  unsigned int maxMemory = Config::getInstance()->getMaxMemory();
  unsigned int memoryPerFrame = Config::getInstance()->getMemoryPerFrame();
  // Get the total number of frames 
  maxPages = maxMemory / memoryPerFrame;
  // initialize memory to have these number of pages
  memory.resize(maxPages, -1);
  // frame indices that are free
  for (int i = 0; i < maxPages; i++) freeFrames.push_back(i);
}

void Paging::initialize(){
  instance = new Paging();
}

bool allocate(std::shared_ptr<Process> processToAllocate){
  int pid = processToAllocate->getPId();

} 
