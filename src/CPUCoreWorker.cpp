#include "CPUCoreWorker.h"

void CPUCoreWorker::initialize() {
    // initialize a new thread for this CPU
    // NOTES use isFinished in Process 
    
    // REMEMBER: process allocation will be handled by the scheduler
    while(true){
        // TODO: while loop for looping, and wait for assigning of process here.
        break;
    }

}


void CPUCoreWorker::runProcess(){
    // TODO: loop thru the process until the instruction line finishes
}

void CPUCoreWorker::setCurrentProcess(std::shared_ptr<Process> process){
    // TODO: public method to set the current process stored in this CPU to the process. handled by scheduler
}
bool CPUCoreWorker::hasCurrentProcess(){
    // TODO: check if there is a process assigned
    return false;
}