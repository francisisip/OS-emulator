#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
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
    
    
    void initializeMemory(size_t maxSize);
    bool canAllocateAt(int index, size_t size) const;
    // void allocateAt(size_t index, size_t size);
    // void deallocateAt(size_t index);
    void mergeFreeBlocks();
};
