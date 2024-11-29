#pragma once
#include "IMemoryAllocator.h"
#include <memory>
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
    size_t getAllocatedSize();
    void placeIntoBackingStore(std::shared_ptr<Process>);
    int getPageIn();
    int getPageOut();
private:
    static Paging* instance;
    size_t maxPages;
    size_t allocatedSize;
    int numFrames;
    int pagedIn = 0;
    int pagedOut = 0;

    // Physical memory represented as frames.
    // -1 means in a given index means no value
    std::vector<int> memory; 
    // Map of process ID to page table.
    std::unordered_map<int, std::unordered_map<int, int>> pageTables; // Map of process ID to page table.
    // List of free frames.
    std::vector<int> freeFrames; 

    std::vector<std::shared_ptr<Process>> allocatedProcessOrder;
    std::vector<int> backingStore;
};
