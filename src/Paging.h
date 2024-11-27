#pragma once
#include "IMemoryAllocator.h"
#include <unordered_map>
#include "Process.h"
#include "Config.h"

class Paging : public IMemoryAllocator {
public:
    Paging();
    ~Paging();

    static Paging* getInstance();
    static void initialize();
    bool allocate(std::shared_ptr<Process> processToAllocate) override;
    void deallocate(std::shared_ptr<Process> processToDeallocate) override;
    void visualizeMemory() override;
    int findFreeFrame();
    size_t getMaxPages();
private:
    static Paging* instance;
    size_t maxPages;
    size_t allocatedSize;
    int numFrames;

    // Physical memory represented as frames.
    // -1 means in a given index means no value
    std::vector<int> memory; 
    // Map of process ID to page table.
    std::unordered_map<int, std::unordered_map<int, int>> pageTables; // Map of process ID to page table.
    // List of free frames.
    std::vector<int> freeFrames; 
};
