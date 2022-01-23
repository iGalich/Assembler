#include "assembler.h"

char *skip_white_space_at_start(char input[])
{
    int i;
    for (i = 0; input[i] != 0 && (input[i] == ' ' || input[i] == '\t'); i++) /* Skips all white space at the start of the string, until reaching non white space */
    {
    }
    return input + i; /* Returns the string from the space where first encounters a non white space character */
}

void delete_spaces(char input[])
{
    int i;
    int position = 0;

    /* If a white space is encountered, save the position of the white space, and move the next non white space character encountered to the saved position. */
    for (i = 0; input[i]; i++)
    {
        if (!isspace(input[i]) && input[i] != '\n')
        {
            input[position++] = input[i];
            printf("removed %c\n", input[i]);
        }
    }

    input[position] = '\0'; /* sets the end of the string */
}