#include <vector>
#include <unordered_map>
#include "IMemoryAllocator.h"

class FlatMemoryAllocator : public IMemoryAllocator {
    public:
        FlatMemoryAllocator(size_t maxSize) : maxSize(maxSize), allocatedSize(0) {
            memory.resize(maxSize, '.');

            
            // memory.reserve(maxSize);

            initialize();
        }

        ~FlatMemoryAllocator() {
            memory.clear();
        }

        void* allocate(size_t size) override {
            // Find first available block to store process
            for (size_t i = 0; i <= maxSize - size; i++) {
                if (!allocationMap[i] && canAllocateAt(i, size)) {
                    allocateAt(i, size);
                    return &memory[i];
                }
            }

            // If unable to find memory block, return nullptr
            return nullptr;
        }

        // TODO: potentially let this handle deallocating several memory blocks at a time?
        void deallocate(void* ptr) override {
            size_t index = static_cast<char*>(ptr) - &memory[0];
            if (allocationMap[index]) {
                deallocateAt(index);
            }
        }

        std::string visualizeMemory() override {
            return std::string(memory.begin(), memory.end());
        }


    private:
        size_t maxSize;
        size_t allocatedSize;
        std::vector<char> memory;
        std::unordered_map<size_t, bool> allocationMap;

        void initialize() {
            // '.' == unallocated memory
            std::fill(memory.begin(), memory.end(), '.'); 

            for (size_t i = 0; i < maxSize; ++i) {
                allocationMap[i] = false;
            }

            // std::fill(allocationMap.begin(), allocationMap.end(), false);


        }

        bool canAllocateAt(size_t index, size_t size) const {
            // Checks if memory block is large enough to accomodate process
            return (index + size <= maxSize);
        }

        void allocateAt(size_t index, size_t size) {
            
            for (size_t i = index; i < index + size; i++) {
                // Mark the corresponding addresses as occupied
                allocationMap[i] = true;

                // Also update the corresponding memory blocks
                memory[i] = '#'; // TODO: maybe change this to mark as the pID instead.
            }

            // Update amount of memory currently occupied
            allocatedSize += size;
        }

        // void allocateAt(size_t index, size_t size) {
        //     // Mark the corresponding addresses as occupied
        //     std::fill(std::next(allocationMap.begin(), 10), std::next(allocationMap.begin(), index + size), true);

        //     // fill memory vector as well?
        //     // Fill up corresponding memory blocks within vector

        //     // Update amount of memory currently occupied
        //     allocatedSize += size;
        // }

        void deallocateAt(size_t index) {
            // Mark memory block as deallocated/unoccupied
            allocationMap[index] = false;

            // Reset memory block
            memory[index] = '.';

            // Update amount of allocated memory.
            allocatedSize -= 1;
        }
};