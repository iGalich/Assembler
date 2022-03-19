#include "assembler.h"

FILE *original_f; /* original .as file */
FILE *post_macro_f; /* post macro file with the file extension .am */
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

    create_post_macro_file(filename);
    check_macro();
}

void create_post_macro_file(char * filename)
{
    strcat(filename, ".am"); /* adds the file extension .am to the file name */

    if (!(post_macro_f = fopen(filename, "w+")))
    {
        fprintf(stderr, "cannot create file\n");
        exit(0);
    }
}

void create_copy(FILE * target, FILE * source)
{
    char ch;

    if (!(copy_f = fopen("copy.am", "w+")))
    {
        fprintf(stderr, "cannot create copy file\n");
        exit(0);
    }
    
    while ((ch = fgetc(source)) != EOF){
        fputc(ch, target);
    }

    rewind(source);
    rewind(target);
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
    int num_of_rows;
    
    linked_list * macro_keywords_list;

    macro_keywords_list = create_empty_list();

    temp_name = (char *)malloc(MAX_LENGTH * sizeof(char));

    for (i = 0; i < 6; i++)
        temp_text[i] = (char *)malloc(81 * sizeof(char));
        

    while (fgets(line, MAX_LENGTH + 1, original_f))
    {
        temp_name = (char *)malloc(MAX_LENGTH * sizeof(char));
        temp_text = (char **)calloc(6, sizeof(char *));

        for (i = 0; i < 6; i++)
            temp_text[i] = (char *)malloc(81 * sizeof(char));

        num_of_rows = i = 0;

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
                    num_of_rows++;
                    if (i > 6)
                    {
                        perror("macro text out of bounds");
                        exit(0);
                    }
                    
                    strcpy(temp_text[i++], line);
                }
                else
                {
                    macro_keywords_list->head = add_to_list(macro_keywords_list, temp_name, temp_text, num_of_rows);
                    /* Reset string array */
                    for (i = 0; i < num_of_rows; i++)
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
    macro_pass(macro_keywords_list);
    first_pass();
}

void macro_pass(linked_list * list)
{
    char line[MAX_LENGTH + 1];
    char macro_def_line[MAX_LENGTH + 1];
    char temp_string[MAX_LENGTH + 1] = "macro ";
    int i;
    int macro_found_flag;
    node * temp;

    temp = list->head;

    while (fgets(line, MAX_LENGTH + 1, original_f))
    {
        macro_found_flag = 0;
        temp = list->head;
        /* for each line, we check if it contains one of the macro keywords */
        for (i = 0; i < get_number_of_nodes(list); i++)
        {
            /* Reset the string so we use it for checks */
            strcpy(macro_def_line, temp_string);
            strcat(macro_def_line, temp->name);

            if (strstr(line, temp->name) != NULL && strstr(line, macro_def_line) == NULL)
            {
                macro_found_flag = 1;
                for (i = 0; i < temp->num_of_rows; i++)
                {
                    if (fputs(temp->text[i], post_macro_f) == EOF)
                    {
                        fprintf(stderr, "write error");
                        exit(0);
                    }
                }
            }
            else
            {
                temp = temp->next;
            }
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
    
    free_macro_list(list->head);
}