#pragma once

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <sstream>
#include <filesystem> 
#include "IMemoryAllocator.h"
#include "Config.h"
#include "Process.h"

struct MemoryBlock {
	int start;
	int end;
	bool isFree;
};

class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator();
    ~FlatMemoryAllocator();

    static FlatMemoryAllocator* getInstance();

    static void initialize();
    bool allocate(std::shared_ptr<Process> processToAllocate) override;
    void deallocate(std::shared_ptr<Process> processToDeallocate) override;
    void visualizeMemory() override;
    size_t getMaxSize();
private:
    static FlatMemoryAllocator* instance;
    size_t maxSize;
    size_t allocatedSize;
    std::vector<MemoryBlock> memory;
    std::unordered_map<int, size_t> allocationMap;
    std::mutex printMemInfoMutex;

    std::vector<std::shared_ptr<Process>> allocatedProcessOrder;
    std::vector<int> backingStore;
    
    
    void initializeMemory(size_t maxSize);
    bool canAllocateAt(int index, size_t size) const;
    void mergeFreeBlocks();
    void placeIntoBackingStore(std::shared_ptr<Process>);
};
