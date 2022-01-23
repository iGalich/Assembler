#include "assembler.h"
#include "linkedlist.h"
#include "const.h"

FILE *original_f; /* original .as file */
FILE *post_macro_f; /* post macro file with the file extension .am */

void look_for_file(char * filename)
{
    int filename_length = 0;
    const int extension_length = 3;

    strcat(filename, ".as"); /* adds the file extension .as to the filename */

    if (!(original_f = fopen(filename, "r")))
    {
        fprintf(stderr, "cannot open file\n");
        exit(0);
    }
    
    filename_length = strlen(filename);
    filename[filename_length - extension_length] = '\0'; /* remove the file extension */

    duplicate_file(filename);
    check_macro();
}

void duplicate_file(char * filename)
{
    char ch;
    int filename_length = 0;
    const int extension_length = 3;

    strcat(filename, ".am"); /* adds the file extension .am to the file name */

    if (!(post_macro_f = fopen(filename, "w+")))
    {
        fprintf(stderr, "cannot create file\n");
        exit(0);
    }

    while ((ch = fgetc(original_f)) != EOF)
        fputc(ch, post_macro_f);

    rewind(post_macro_f);

    filename_length = strlen(filename);
    filename[filename_length - extension_length] = '\0'; /* remove the file extension */
}

void check_macro()
{
    const char *start_of_macro_pattern = "macro";
    const char *end_of_macro_pattern = "mend";
    char line[MAX_LENGTH + 1];
    char *found_macro;
    char *temp;

    linked_list * line_list;

    line_list = create_empty_list();

    temp = (char *)malloc(MAX_LENGTH * sizeof(char));

    while (fgets(line, MAX_LENGTH + 1, post_macro_f))
    {
        if (strstr(line, start_of_macro_pattern) != NULL)
        {
            found_macro = line + strlen(start_of_macro_pattern); /* A macro has been found in the line, so we remove the macro keyword */

            found_macro = skip_white_space_at_start(found_macro);

            /* We loop and look for the keyword indicating the end of the macro. As stated in the instructions, we assume it exists. */
            while (fgets(line, MAX_LENGTH + 1, post_macro_f))
            {
                /* Each line that does not contain the keyword indicating the end of the macro, we add to a linked list */
                if (strstr(line, end_of_macro_pattern) == NULL)
                {
                    temp = (char *)malloc(MAX_LENGTH * sizeof(char));
                    strcpy(temp, line);
                    line_list->head = add_to_list(line_list, temp);
                }
            }
            print_list(line_list);
            free(temp);
            break;
        }
    }
}