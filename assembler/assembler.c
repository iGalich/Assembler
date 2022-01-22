#include "assembler.h"

int main(int argc, char *argv[])
{
    while (--argc)
    {
        look_for_file(argv[argc]);
    }
    return 0;
}