#include "assembler.h"

extern FILE * post_macro_f;

void second_pass(symbol_linked_list * symbol_list, data_linked_list * data_list)
{
    struct stat sb;

    char * file_contents;

    const char * data_keyword = ".data";
    const char * string_keyword = ".string";
    const char * extern_keyword = ".extern";
    const char * entry_keyword = ".entry";

    int error_found_flag = 0;

    if (stat("copy.am", &sb) == -1)
    {
        perror("stat error\n");
        exit(0);
    }   

    file_contents = malloc(sb.st_size);

    /* TEST */

    /*printf("------\n");
    printf("priting data list\n");
    print_data_list(data_list);
    printf("printing symbol list\n");
    print_symbol_list(symbol_list);

    exit(0);*/
    /* END OF TEST */ 

    printf("starting second pass\n");
    /* STEP 1 go over lines, if eof goto step 7 */
    while(fscanf(post_macro_f, " %[^\n ]", file_contents) != EOF)
    {
        /* STEP 2, skip if label */
        if (strstr(file_contents, ":"))
            continue;

        /* STEP 3, skip if data instruction or extern */
        if (!strcmp(file_contents, data_keyword) || !strcmp(file_contents, string_keyword) || !strcmp(file_contents, extern_keyword))
            continue;

        /* STEP 4, check if entry, else go to step 6 */
        if (!strcmp(file_contents, entry_keyword))
        {
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            /* STEP 5 add entry attribute */
            if (!find_symbol_and_change_entry(symbol_list ,file_contents))
            {
                printf("error : symbol wasn't found\n");
                error_found_flag = 1;
            }
        }

    }
    
    /* STEP 7 */
}