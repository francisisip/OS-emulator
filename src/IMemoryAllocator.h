#pragma once
#include <vector>
#include "Process.h"
class IMemoryAllocator {
    public:
        virtual ~IMemoryAllocator() = default;
        virtual bool allocate(std::shared_ptr<Process> processToAllocate) = 0;
        virtual void deallocate(std::shared_ptr<Process> processToDeallocate) = 0;
        virtual void visualizeMemory() = 0;
};