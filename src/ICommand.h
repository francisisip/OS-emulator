#pragma once
#include <string.h>
class ICommand
{
public:
    enum CommandType
    {
        PRINT
    };

    ICommand(CommandType type);
    CommandType getCommandType();
    virtual void execute();

protected:
    CommandType CommandType;
};