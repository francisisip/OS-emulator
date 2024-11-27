#include "IMemoryAllocator.h"

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
    size_t setMaxPages();
private:
    static FlatMemoryAllocator* instance;
    size_t maxPages;
    size_t allocatedSize;
    int numFrames;


};
