#pragma once
#include <string.h>
#include <iostream>
class ICommand
{
public:
    enum CommandType
    {
        PRINT
    };

    ICommand(int pid, CommandType type);
    CommandType getCommandType();
    virtual void execute();

protected:
    int pid;
    CommandType CommandType;
    
};