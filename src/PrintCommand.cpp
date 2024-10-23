#include "PrintCommand.h"

PrintCommand::PrintCommand(int pid, std::string& toPrint) : ICommand(pid, PRINT)
{
    this->toPrint = toPrint;
}

void PrintCommand::execute()
{
    ICommand::execute();
    std::cout << "PID " << this->pid << ":" << this->toPrint << std::endl;
}