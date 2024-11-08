#pragma once
#include <vector>

class IMemoryAllocator {
    public:
        virtual ~IMemoryAllocator() = default;
        virtual bool allocate(size_t size) = 0;
        virtual void deallocate(void* ptr) = 0;
        virtual std::string visualizeMemory() = 0;
};