#include "assembler.h"
#include "const.h"

FILE *original_f; /* original .as file */
FILE *post_macro_f; /* post macro file (1st pass) with the file extension .am */
FILE *copy_f;

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

    create_first_pass_file(filename);
    check_macro();
}

void create_first_pass_file(char * filename)
{
    strcat(filename, ".am"); /* adds the file extension .am to the file name */

    if (!(post_macro_f = fopen(filename, "w+")))
    {
        fprintf(stderr, "cannot create file\n");
        exit(0);
    }
}

void check_macro()
{
    const char *start_of_macro_pattern = "macro";
    const char *end_of_macro_pattern = "endm";
    char line[MAX_LENGTH + 1];
    char **temp_text = (char **)calloc(6, sizeof(char *));
    char * found_macro;
    char * temp_name;
    int i;
    
    linked_list * macro_keywords_list;

    macro_keywords_list = create_empty_list();

    temp_name = (char *)malloc(MAX_LENGTH * sizeof(char));

    for (i = 0; i < 6; i++)
        temp_text[i] = (char *)calloc(81, sizeof(char));

    i = 0;

    while (fgets(line, MAX_LENGTH + 1, original_f))
    {
        if (strstr(line, start_of_macro_pattern) != NULL)
        {    
            found_macro = line + strlen(start_of_macro_pattern);
            found_macro = skip_white_space_at_start(found_macro);
            found_macro[strlen(found_macro) - 1] = '\0';

            strcpy(temp_name, found_macro);
            while (fgets(line, MAX_LENGTH + 1, original_f))
            {
                if (strstr(line, end_of_macro_pattern) == NULL)
                {
                    if (i > 6)
                    {
                        perror("macro text out of bounds");
                        exit(0);
                    }
                    strcpy(temp_text[i++], line);
                }
                else
                {
                    macro_keywords_list->head = add_to_list(macro_keywords_list, temp_name, temp_text);
                    /* Reset string array */
                    for (i = 0; i < 6; i++)
                        free(temp_text[i]);
                    free(temp_text);
                    free(temp_name);
                    i = 0;
                    break; /* We go and look for the next macro definition */
                }
            }
        }
    }

    rewind(original_f);
    first_pass(macro_keywords_list);
}

/* FIND BUG HERE */
void first_pass(linked_list * list)
{
    char line[MAX_LENGTH + 1];
    int i = 0;
    int macro_found_flag = 0;
    node * temp;

    temp = list->head;

    while (fgets(line, MAX_LENGTH + 1, original_f))
    {
        /* for each line, we check if it contains one of the macro keywords */
        for (i = 0; i < get_number_of_nodes(list); i++)
        {
            if (strstr(line, temp->name) != NULL)
            {
                macro_found_flag = 1;
                for (i = 0; i < sizeof(temp->text) / sizeof(temp->text[0]); i++)
                {
                    if (fputs(temp->text[i], post_macro_f) == EOF)
                    {
                        fprintf(stderr, "write error");
                        exit(0);
                    }
                }
            }
            else
                temp = temp->next;
        }
        if (!macro_found_flag)
        {
            if (fputs(line, post_macro_f) == EOF)
            {
                fprintf(stderr, "write error");
                exit(0);
            }
            macro_found_flag = 0;
        }
    }

    rewind(original_f);
    rewind(post_macro_f);
}