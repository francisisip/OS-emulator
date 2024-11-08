#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "IMemoryAllocator.h"
#include "Config.h"
#include "Process.h"

struct MemoryBlock {
	size_t start;
	size_t end;
	bool isFree;
};

class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator();
    ~FlatMemoryAllocator();

    static FlatMemoryAllocator* getInstance();

    static void initialize();
    bool allocate(Process processToAllocate) override;
    void deallocate(Process processToDeallocate) override;
    std::string visualizeMemory() override;
    size_t getMaxSize();
private:
    static FlatMemoryAllocator* instance;
    size_t maxSize;
    size_t allocatedSize;
    std::vector<MemoryBlock> memory;
    std::unordered_map<int, size_t> allocationMap;
    
    
    void initializeMemory(size_t maxSize);
    bool canAllocateAt(size_t index, size_t size) const;
    // void allocateAt(size_t index, size_t size);
    // void deallocateAt(size_t index);
    void mergeFreeBlocks();
};
