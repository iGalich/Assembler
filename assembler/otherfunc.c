#include "assembler.h"

char *skip_white_space_at_start(char input[])
{
    int i;
    for (i = 0; input[i] != 0 && (input[i] == ' ' || input[i] == '\t'); i++) /* Skips all white space at the start of the string, until reaching non white space */
    {
    }
    return input + i; /* Returns the string from the space where first encounters a non white space character */
}