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
    //maxSize = Config::getMaxSize();
    size_t maxSize = 16384;
    instance->initializeMemory(maxSize);
}

// Implements first-fit approach by default for now.
bool FlatMemoryAllocator::allocate(size_t size) {
    for (size_t i = 0; i <= maxSize - size; i++) {
        if (!allocationMap[i] && canAllocateAt(i, size)) {
            allocateAt(i, size);
            return true;
        }
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

void FlatMemoryAllocator::initializeMemory(size_t maxSize) {
    this->maxSize = maxSize;
    memory.resize(maxSize, '.');
    std::fill(memory.begin(), memory.end(), '.');
    for (size_t i = 0; i < maxSize; i++) {
        allocationMap[i] = false;
    }
}

bool FlatMemoryAllocator::canAllocateAt(size_t index, size_t size) const {
    return (index + size <= maxSize);
}

void FlatMemoryAllocator::allocateAt(size_t index, size_t size) {
    for (size_t i = index; i < index + size; i++) {
        printf("allocateAt Current Index: %lu out of %lu\n", i, index + size);
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