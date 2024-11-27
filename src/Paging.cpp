#include "Paging.h"
#include "Config.h"

Paging::Paging(){
  unsigned int maxMemory = Config::getInstance()->getMaxMemory();
  unsigned int memoryPerFrame = Config::getInstance()->getMemoryPerFrame();
  // Get the total number of frames 
  maxPages = maxMemory / memoryPerFrame;
}

void Paging::initialize(){
  instance = new Paging();
}
