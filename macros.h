#include "command.h"

class Macros
{
    public:
        Macros(int listSize, Command *list);
        void exec();
    private:
        Command *commandList;
        int size;
};
