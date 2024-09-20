#include <string>
#include <vector>
#include "Process.h"

class Screen {
private:
    std::string screenName;
    Process process;
    std::vector<std::string> stringList;

// TODO: add methods
public:
    Screen(): screenName("Main") {
        displayASCII();
        process;
    }

    Screen(std::string& name): screenName(screenName) {
        displayASCII();
        process.setProcessName(screenName);
    }

    void printProcessInfo() {
        process.printDummyInfo();
    }

    void displayASCII() {
        std::cout << R"(
    $$$$$$\   $$$$$$\   $$$$$$\  $$$$$$$\  $$$$$$$$\  $$$$$$\ $$\     $$\ 
    $$  __$$\ $$  __$$\ $$  __$$\ $$  __$$\ $$  _____|$$  __$$\\$$\   $$  |
    $$ /  \__|$$ /  \__|$$ /  $$ |$$ |  $$ |$$ |      $$ /  \__|\$$\ $$  / 
    $$ |      \$$$$$$\  $$ |  $$ |$$$$$$$  |$$$$$\    \$$$$$$\   \$$$$  /  
    $$ |       \____$$\ $$ |  $$ |$$  ____/ $$  __|    \____$$\   \$$  /   
    $$ |  $$\ $$\   $$ |$$ |  $$ |$$ |      $$ |      $$\   $$ |   $$ |    
    \$$$$$$  |\$$$$$$  | $$$$$$  |$$ |      $$$$$$$$\ \$$$$$$  |   $$ |    
    \______/  \______/  \______/ \__|      \________| \______/    \__|    
    )";

        std::cout << "Hello, welcome to CSOPESY commandline!" << '\n';
        std::cout << "Type 'exit' to quit, 'clear' to clear the screen" << '\n';
    }

    void initialize (){
        std::cout << "initialize command recognized. Doing something\n" << std::endl;
    }

    void screen (){
        std::cout << "screen command recognized. Doing something\n" << std::endl;
    }
    void schedulerTest (){
        std::cout << "scheduler-test command recognized. Doing something\n" << std::endl;
    }
    void schedulerStop (){
        std::cout << "scheduler-stop command recognized. Doing something\n" << std::endl;
    }

    void reportUtil() {
        std::cout << "report-util command recognized. Doing something\n" << std::endl;
    }

    void clear() {
        system("cls");
        displayASCII();
    }
};