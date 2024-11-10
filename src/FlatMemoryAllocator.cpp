#include <iostream>
#include "FlatMemoryAllocator.h"

FlatMemoryAllocator* FlatMemoryAllocator::instance = nullptr;

// Constructor and Destructor Implementation
FlatMemoryAllocator::FlatMemoryAllocator() {

}

FlatMemoryAllocator::~FlatMemoryAllocator() {
    memory.clear();
}

FlatMemoryAllocator* FlatMemoryAllocator::getInstance() {
    return instance;
}

void FlatMemoryAllocator::initialize() {
    instance = new FlatMemoryAllocator();
    size_t maxSize = Config::getInstance()->getMaxMemory();
    instance->initializeMemory(maxSize);
}

void FlatMemoryAllocator::initializeMemory(size_t maxSize) {
    this->maxSize = maxSize;
    memory.push_back({0, static_cast<int>(maxSize) - 1, true});
}

// Implements first-fit approach by default for now.
bool FlatMemoryAllocator::allocate(std::shared_ptr<Process> processToAllocate) {
    
    // if PID is in memory, return true
    auto it = allocationMap.find(processToAllocate->getPId());
    if (it != allocationMap.end()) return true;

    for (auto& block : memory){
        if(!block.isFree) continue;        
        int origBlockSize = block.end - block.start + 1;
        int originalEnd = block.end;
        int processMemoryRequired = static_cast<int>(processToAllocate->getMemoryRequired());
        if(origBlockSize < processMemoryRequired) continue;

        block.isFree = false;
        block.end = block.start + processMemoryRequired - 1;

        allocationMap[processToAllocate->getPId()] = block.start;


        if (origBlockSize > block.end - block.start + 1)
            memory.push_back({ block.start + processMemoryRequired, originalEnd, true });

        return true;
    }
    return false;
}

void FlatMemoryAllocator::deallocate(std::shared_ptr<Process> processToDeallocate) {
    auto it = allocationMap.find(processToDeallocate->getPId());
    if (it == allocationMap.end()) return;

    for (auto& block : memory){
        if(block.start == it->second){
            block.isFree = true;
            allocationMap.erase(it);
            mergeFreeBlocks();
            break;
        }
    }
}

void FlatMemoryAllocator::mergeFreeBlocks() {

    std::sort(memory.begin(), memory.end(), [](const MemoryBlock& a, const MemoryBlock& b) {
        return a.start < b.start;
        });

    auto it = memory.begin();
    while (it != memory.end()) {
        if (it->isFree) {
            // Check the next block for merging
            auto next = it + 1;
            while (next != memory.end() && next->isFree) {
                it->end = next->end;  // Extend the current block to the end of the next free block
                next = memory.erase(next);  // Erase the next block
            }
        }
        ++it;  // Move to the next block if no further merges are possible
    }
}


void FlatMemoryAllocator::visualizeMemory() {
    std::cout << "Visualizing Memory...\n\n";
    for (auto& block : memory){
        std::cout << "------------------------\n";
        std::cout << "Block at address: " << block.start << std::endl;
        std::cout << "Ends at address: " << block.end << std::endl;
        std::cout << "Is it free? " << block.isFree << std::endl;
        std::cout << "------------------------\n";
        std::cout << "\n";
    }
}

size_t FlatMemoryAllocator::getMaxSize() {
    return maxSize;
}


bool FlatMemoryAllocator::canAllocateAt(int index, size_t size) const {
    return (index + size <= maxSize);
}
