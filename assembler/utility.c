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

char * chop_first_n_characters(char * str, size_t n)
{
    if (n > strlen(str))
    {
        fprintf(stderr, "out of bounds after getting %s and %d\n", str, n);
        exit(0);
    }
    return str + n;
}

int power(int base, int power)
{
    int result = 1;
    int i = 0;

    for (i = 0; i < power; i++)
    {
        result *= base;
    }
    return result;
}

int count_words_in_lines(char * string)
{
    int i, count = 0;
    int in_word_flag = 0;

    string = skip_white_space_at_start(string);

    do switch (*string)
    {
        case '\0':
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case ',':
            if (in_word_flag)
            {
                in_word_flag = 0;
                count++;
            }
            break;
        default:
            in_word_flag = 1;
    } while (*string++);
    
    return count;
}

int count_number_of_lines_in_file(FILE * file)
{
    int count = 0;
    char ch;

    for (ch = getc(file); ch != EOF; ch = getc(file))
    {
        if (ch == '\n')
            count++;
    }
    rewind(file);
    return count;
}