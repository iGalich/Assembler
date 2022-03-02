#include "assembler.h"
#include "const.h"

FILE *original_f; /* original .as file */
FILE *post_macro_f; /* post macro file with the file extension .am */
FILE *copy_f;

char * macro_keyword;
char * global_filename;

void look_for_file(char * filename)
{
    int filename_length = 0;
    const int extension_length = 3;

    global_filename = (char *)malloc(MAX_LENGTH * sizeof(char));
    strcpy(global_filename, filename);

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

    rewind(original_f);
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
    fpos_t checkpoint;

    linked_list * line_list;

    line_list = create_empty_list();

    temp = (char *)malloc(MAX_LENGTH * sizeof(char));
    macro_keyword = (char *)malloc(MAX_LENGTH * sizeof(char));

    while (fgets(line, MAX_LENGTH + 1, original_f))
    {
        if (strstr(line, start_of_macro_pattern) != NULL)
        {
            found_macro = line + strlen(start_of_macro_pattern); /* A macro has been found in the line, so we remove the macro keyword */

            found_macro = skip_white_space_at_start(found_macro);

            found_macro[strlen(found_macro) - 1] = '\0';

            strcpy(macro_keyword, found_macro);

            /* We loop and look for the keyword indicating the end of the macro. As stated in the instructions, we assume it exists. */
            while (fgets(line, MAX_LENGTH + 1, original_f))
            {
                /* Each line that does not contain the keyword indicating the end of the macro, we add to a linked list */
                if (strstr(line, end_of_macro_pattern) == NULL)
                {
                    temp = (char *)malloc(MAX_LENGTH * sizeof(char));
                    strcpy(temp, line);
                    line_list->head = add_to_list(line_list, temp);
                }
                else
                    break;
            }

            fgetpos(original_f, &checkpoint); /* saves the current position, so we can come back to it later and continute checking for more macros */

            find_macro_instances(line_list);

            fsetpos(original_f, &checkpoint); /* returns to the saved position */

            free(temp);
        }
    }
}

void find_macro_instances(linked_list * list)
{
    char line[MAX_LENGTH + 1];
    int line_number = 0;
    int macro_def_flag = 0; /* a flag used to indicate if we've passed the first instance of macro appearance */

    rewind(post_macro_f);

    while (fgets(line, MAX_LENGTH + 1, post_macro_f))
    {
        line_number++;
        
        if (strstr(line, macro_keyword) != NULL)
        {
            if (macro_def_flag == 1)
            {
                replace_macro(list, line_number);
            }
            else
            {
                macro_def_flag = 1;
            }
        }

    }
}

/* FOR FUTURE ME: clean this function, add functinallity of adding the new lines */
void replace_macro(linked_list * list, int line_number)
{
    int c, last, lineno;

    last = '\n';
    lineno = 0;

    strcat(global_filename, ".am");

    rewind(post_macro_f);

    while ((c = getc(post_macro_f)) != EOF)
    {
        if (last == '\n')
            lineno++;

        last = c;
    }

    rewind(post_macro_f);

    if (!(copy_f = fopen("copy.c", "w")))
    {
        fprintf(stderr, "cannot create copy.c\n");
        exit(0);
    }

    lineno = 1;

    while ((c = getc(post_macro_f)) != EOF)
    {
        if (lineno != line_number)
            putc(c, copy_f);
        
        if (c == '\n')
            lineno++;
    }

    if (fclose(copy_f))
    {
        perror("write error to copy.c");
        exit(0);
    }

    rename("copy.c", global_filename);

    global_filename[strlen(global_filename) - 3] = '\0';
}