#include "assembler.h"

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
    /*const char *end_of_macro_pattern = "mend";*/
    char line[MAX_LENGTH + 1];
    /*char *target = NULL;*/
    char *start = NULL;
    /*char *end = NULL;
    char *found_macro; */

    int line_number;

    /*fgets(line, MAX_LENGTH+1,post_macro_f);
    start = strstr(line,start_of_macro_pattern);
    if (start)
    {
        start = strstr(line, start_of_macro_pattern);
        printf("%s\n", start);
        return;
    }
    printf("%s\n here\n", start);
    return;*/

    for (line_number = 0; fgets(line, MAX_LENGTH + 1, post_macro_f); line_number++)
    {
        if (start == NULL)
            start = strstr(line, start_of_macro_pattern);
        else
        {   
            printf("%s", start);
            continue;
        }
        /*found_macro = line[strlen(start)];
        printf("%s", found_macro);*/
    }

/*
    while (fgets(line, MAX_LENGTH + 1, post_macro_f))
    {
        if (start == NULL)
            start = strstr(line, start_of_macro_pattern);
        if (start != NULL)
        {
            start += strlen(start_of_macro_pattern);
            if (end == NULL)
                end = strstr(start, end_of_macro_pattern);
            target = (char *) malloc(end - start + 1);
            memcpy(target, start, end - start);
            target[end - start] = '\0';
        }
        if (target)
            printf("%s\n", target);
        free(target);
    }*/
}