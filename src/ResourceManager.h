#include "Scheduler.h"
#include "Process.h"
#include "Config.h"
class ResourceManager{
    // This is going to be a singleton also. para everything can access it
public:
    ResourceManager();
    static ResourceManager* getInstance();
    static void initialize();

    
    // spam add process with Scheduler::addProcess. 
    void schedulerTestStart();
    void schedulerTestStop(); // this is the scheduler-stop command. sounds weird so I just made it test stop




private:
   static ResourceManager* instance;
    Scheduler* schedulerInstance;

};
