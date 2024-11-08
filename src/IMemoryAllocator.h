#pragma once
#include <vector>

class IMemoryAllocator {
    public:
        virtual ~IMemoryAllocator() = default;
        virtual bool allocate(Process processToAllocate) = 0;
        virtual void deallocate(Process processToDeallocate) = 0;
        virtual std::string visualizeMemory() = 0;
};