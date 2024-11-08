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
    memory.push_back({0, maxSize, true});
}

// Implements first-fit approach by default for now.
bool FlatMemoryAllocator::allocate(Process processToAllocate) {
    // check first if memory is allocated already for the process
    // loop thru the memory vector, and find the first fitting block
    for (auto& block : memory){
        if(!block.isFree) continue;
        
        size_t blockSize = block.end - block.start + 1;
        if(!blockSize >= processToAllocate.getMemoryRequired()) continue;

        // we have one whole block, we split it to make the block fit exactly the current process
        MemoryBlock fittingBlock = {block.start, block.start + processToAllocate.getMemoryRequired(), false};
        size_t fittingBlockSize = fittingBlock.end - fittingBlock.start + 1;

        block.isFree = false;
        block.end = block.start + processToAllocate.getMemoryRequired();
        
        // make a new memory block if the fittingblock.end == block.end
        if(fittingBlockSize < blockSize){
            MemoryBlock fragmentedBlock = {fittingBlock.end + 1, block.end, true};
            memory.push_back(fragmentedBlock);
        }

        block = fittingBlock;
        allocationMap[processToAllocate.getPId()] = block.start;
        return true;
    }
    return false;
}

void FlatMemoryAllocator::deallocate(void* ptr) {
    size_t index = static_cast<char*>(ptr) - &memory[0];
    if (allocationMap[index]) {
        deallocateAt(index);
    }
}

std::string FlatMemoryAllocator::visualizeMemory() {
    return std::string(memory.begin(), memory.end());
}

size_t FlatMemoryAllocator::getMaxSize() {
    return maxSize;
}


bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const {
    return (index + size <= maxSize);
}

void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
    for (size_t i = index; i < index + size; i++) {
        // printf("allocateAt Current Index: %lu out of %lu\n", i, index + size);
        allocationMap[i] = true;
        memory[i] = '#';
    }
    allocatedSize += size;
}

void FlatMemoryAllocator::deallocateAt(size_t index) {
    allocationMap[index] = false;
    memory[index] = '.';
    allocatedSize -= 1;
}