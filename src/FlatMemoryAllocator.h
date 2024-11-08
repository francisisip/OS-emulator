#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "IMemoryAllocator.h"
#include "Config.h"

class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator();
    ~FlatMemoryAllocator();

    static FlatMemoryAllocator* getInstance();

    static void initialize();
    void* allocate(size_t size) override;
    void deallocate(void* ptr) override;
    std::string visualizeMemory() override;

private:
    static FlatMemoryAllocator* instance;
    size_t maxSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;

    void initializeMemory(size_t maxSize);
    bool canAllocateAt(size_t index, size_t size) const;
    void allocateAt(size_t index, size_t size);
    void deallocateAt(size_t index);
};
