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
    memory.push_back({0, static_cast<int>(maxSize), true});
}

// Implements first-fit approach by default for now.
bool FlatMemoryAllocator::allocate(std::shared_ptr<Process> processToAllocate) {
    
    // if PID is in memory, return true
    auto it = allocationMap.find(processToAllocate->getPId());
    if (it != allocationMap.end()) return true;

    for (auto& block : memory){
        if(!block.isFree) continue;        
        int blockSize = block.end - block.start + 1;
        int processMemoryRequired = static_cast<int>(processToAllocate->getMemoryRequired());
        if(blockSize < processMemoryRequired) continue;

        // we have one whole block, we split it to make the block fit exactly the current process
        MemoryBlock fittingBlock = {block.start, block.start + processMemoryRequired, false};
        size_t fittingBlockSize = fittingBlock.end - fittingBlock.start + 1;
        
        // make a new memory block if the fittingblock.end == block.end
        if(fittingBlockSize < blockSize){
            MemoryBlock fragmentedBlock = {fittingBlock.end + 1, block.end, true};
            memory.push_back(fragmentedBlock);
        }

        std::cout << "\n------------------------\n";
        std::cout << "Before updating block: " << std::endl;
        std::cout << "Block at address: " << block.start << " Ends at address: " << block.end << " Is it free? " << block.isFree << std::endl;
        std::cout << "FittingBlock at address: " << fittingBlock.start << " Ends at address: " << fittingBlock.end << " Is it free? " << fittingBlock.isFree << std::endl;

        block = fittingBlock;

        std::cout << "After updating block: " << std::endl;
        std::cout << "Block at address: " << block.start << " Ends at address: " << block.end << " Is it free? " << block.isFree << std::endl;
        std::cout << "------------------------\n";
        allocationMap[processToAllocate->getPId()] = block.start;
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
    for (auto it = memory.begin(); it != memory.end(); it++){
        if(it->isFree){
            auto next = it + 1;
            if(next != memory.end() && next->isFree){
                it->end = next->end;
                memory.erase(next);
            }

            auto prev = it - 1;
            if(prev != memory.begin() && prev->isFree){
                prev->end = it->end;
                memory.erase(it);
            }
        }
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

// void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
//     for (size_t i = index; i < index + size; i++) {
//         // printf("allocateAt Current Index: %lu out of %lu\n", i, index + size);
//         allocationMap[i] = true;
//         memory[i] = '#';
//     }
//     allocatedSize += size;
// }

// void FlatMemoryAllocator::deallocateAt(size_t index) {
//     allocationMap[index] = false;
//     memory[index] = '.';
//     allocatedSize -= 1;
// }