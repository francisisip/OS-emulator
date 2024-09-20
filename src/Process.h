#include <iostream>

class Process {
private:
    std::string processName;
    std::string instruction;
    std::string timestamp; //TODO: change to actual time data type

// TODO: add methods
public:

    Process() {
        setDummyInfo(); // remove once actual data is available
    }

    void setProcessName(const std::string& name) {
        processName = name;
    }

    // reemove once actual data is available
    void setDummyInfo() {
        instruction = "7 / 100";
        timestamp = "2024-09-20 11:43 AM";
    }
    
    void printDummyInfo() {
        std::cout << processName << std::endl;
        std::cout << instruction << std::endl;
        std::cout << timestamp << "\n" << std::endl;
    }

};
