#include "assembler.h"

FILE *original_f; /* original .as file */
FILE *post_macro_f; /* post macro file with the file extension .am */

void look_for_file(char * filename)
{
    int filename_length = 0;
    int extension_length = 3;

    strcat(filename, ".as"); /* adds the file extension .as to the filename */

    if (!(original_f = fopen(filename, "r")))
    {
        fprintf(stderr, "cannot open file\n");
        exit(0);
    }
    
    filename_length = strlen(filename);
    filename[filename_length - extension_length] = '\0'; /* remove the file extension */

    duplicate_file(filename);
    check_macro(filename);
}

void duplicate_file(char * filename)
{
    char ch;
    int filename_length = 0;
    int extension_length = 3;

    strcat(filename, ".am"); /* addsthe file extension .am to the file name */

    if (!(post_macro_f = fopen(filename, "w")))
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
    

    
}