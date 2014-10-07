#include "macros.h"
#include <string>

Macros::Macros(int listSize, Command *list)
{
    size = listSize;
    commandList = list;
    memset(commandList, 0, size * sizeof(Command));
}

void Macros::exec()
{
    for (int i = 0; i < size; i++)
        commandList[i].exec();
}
