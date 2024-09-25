#pragma warning(disable : 4996) 
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

class Process {
private:
    std::string processName;
    std::string instruction;
    std::string formattedTime;
// TODO: add methods
public:

    Process() {
        formattedTime = getFormattedTime();
        instruction = "7 / 100";
    }

    void setProcessName(const std::string& name) {
        processName = name;
    }

    std::string getFormattedTime() {
        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);
        std::stringstream ss;
        ss << std::put_time(localTime, "%m/%d/%Y, %I:%M:%S ");
        ss << (localTime->tm_hour >= 12 ? "PM" : "AM");
        return ss.str();
    }
    
    void printDummyInfo() {
        std::cout << processName << std::endl;
        std::cout << instruction << std::endl;
        std::cout << formattedTime << "\n" << std::endl;
    }

};
