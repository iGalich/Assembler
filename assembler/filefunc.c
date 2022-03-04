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
    int ch;

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
}

void check_macro()
{
    const char *start_of_macro_pattern = "macro";
    const char *end_of_macro_pattern = "endm";
    char line[MAX_LENGTH + 1];
    char *found_macro;
    char *temp;

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
            rewind(post_macro_f);

            find_macro_instances(line_list);

            free(temp);
        }
    }
}

void find_macro_instances(linked_list * list)
{
    char * line;
    int line_number = 0;
    int last_line = 0;
    int bool_copy_created = 0;
    int macro_def_flag = 0; /* a flag used to indicate if we've passed the first instance of macro appearance */
    int flag_limiter = 1;
    int num_of_macros_instances = 0;

    line = (char *)malloc(MAX_LENGTH * sizeof(char));

    /* Go over each line in the file */
    while (fgets(line, MAX_LENGTH + 1, post_macro_f))
    {
        line_number++;

        if (strstr(line, macro_keyword) != NULL)
        {
            if (macro_def_flag >= flag_limiter)
            {
                rewind(post_macro_f);
                replace_macro(list, line_number, last_line); /* we pass the number of the line which contains the appearance of the macro keyword so we can replace it with the defined macro */
                last_line = line_number;
                line_number += get_number_of_nodes(list) - 1; /* We move the line number because we have changed the text and added new lines to the file */
                macro_def_flag = 0;
                rewind(post_macro_f);
                line_number = 0;
                flag_limiter++;
                num_of_macros_instances++;
            }
            else if (bool_copy_created)
            {
                macro_def_flag++;
            }
            else
            {
                bool_copy_created++; /* first instance of the macro appearance has been found, which defines the macro itself */
                rewind(post_macro_f);
                last_line = line_number + get_number_of_nodes(list) + 1;
                create_copy(last_line + 1);
                rewind(post_macro_f);
                line_number = 0;
            }
        }    
    }
    rewind(post_macro_f);
    replace_macro(list, count_number_of_lines_in_file(post_macro_f) + 1, count_number_of_lines_in_file(post_macro_f) - 1 - num_of_macros_instances + num_of_macros_instances * get_number_of_nodes(list));
}

/* FIX THE CUrrent bug present, just check tommorow */
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

void create_copy(int line_number)
{
    int ch, last, curr_line;

    last = '\0';
    curr_line = 1;

    if (!(copy_f = fopen("copy.am", "a+")))
    {
        fprintf(stderr, "cannot create copy.c\n");
        exit(0);
    }

    while ((ch = getc(post_macro_f)) != EOF)
    {
        if (curr_line < line_number)
            fputc(ch, copy_f);
        last = ch;
        if (last == '\n')
            curr_line++;
    }

}

void replace_macro(linked_list * list, int delete_line, int last_line)
{
    int ch, line_number;
    int result_check = 0; /* used to check if the functions fputs was successful */
    node * temp;

    line_number = 1;
    temp = list->head;

    strcat(global_filename, ".am");

    while ((ch = getc(post_macro_f)) != EOF)
    {
        if (line_number > delete_line)
            break;
        if (line_number <= last_line)
        {
            ;
        }
        /* We copy the contents of the file, but skip over the line which contains the macro keyword */
        else if (line_number != delete_line)
            fputc(ch, copy_f);
        else
        {
            while(temp != NULL)
            {
                if ((result_check = fputs(temp->data, copy_f)) == EOF)
                {
                    fprintf(stderr, "write error to copy.am");
                    exit(0);
                }
                temp = temp->next;
            }
            line_number += get_number_of_nodes(list) - 1;
        }
             
        if (ch == '\n')
            line_number++;
    }


    /* We overwrite the .am file with the new edited file 
    if ((rename("copy.am", global_filename)) == EOF)
    {
        fprintf(stderr, "error renaming file");
        exit(0);
    }*/

    global_filename[strlen(global_filename) - 3] = '\0';

    rewind(post_macro_f);
}