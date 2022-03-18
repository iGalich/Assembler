#include "assembler.h"

extern FILE * post_macro_f;
extern const char * command_names[16];

void second_pass(symbol_linked_list * symbol_list, data_linked_list * data_list, address_linked_list * address_list)
{
    struct stat sb;

    char * file_contents;
    char * previous_word;

    const char * keywords[5] = {".data",
                                ".string",
                                ".extern",
                                ".entry",
                                ":"};

    int error_found_flag = 0;
    int looking_for_symbol_flag = 0;
    int i, j;

    address_node * temp;
    symbol_node * symbol;

    word_with_operands word_with;
    word_without_operands  word_without;

    if (stat("copy.am", &sb) == -1)
    {
        perror("stat error\n");
        exit(0);
    }   

    file_contents = malloc(sb.st_size);
    previous_word = malloc(sb.st_size);

    temp = address_list->head;

    /* TEST */

    /*printf("------\n");
    printf("priting data list\n");
    print_data_list(data_list);
    printf("printing symbol list\n");
    print_symbol_list(symbol_list);
    printf("printing address list\n");
    print_address_list(address_list);

    exit(0);*/
    /* END OF TEST */ 

    /* STEP 1 go over lines, if eof goto step 7 */
    while(fscanf(post_macro_f, " %[^\n ]", file_contents) != EOF)
    {

        /* STEP 4, check if entry, else go to step 6 */
        if (!strcmp(file_contents, keywords[3]))
        {
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            /* STEP 5 add entry attribute */
            if (!find_symbol_and_change_entry(symbol_list ,file_contents))
            {
                printf("error : symbol %s wasn't found\n", file_contents);
                error_found_flag = 1;
            }
        }

        for (i = 0; i < 16; i++)
        {
            if (!strcmp(file_contents, command_names[i]) && strstr(previous_word, keywords[4]) == NULL)
            {
                looking_for_symbol_flag = 1;
                while (looking_for_symbol_flag && fscanf(post_macro_f, " %[^\n ]", file_contents) != EOF)
                {
                    for (j = 0; j < 5; j++)
                    {
                        if (strstr(file_contents, keywords[j]))
                        {
                            looking_for_symbol_flag = 0;
                            goto escape;
                        }

                    }
                    if ((symbol = find_symbol_with_name(symbol_list, file_contents)) != NULL)
                    {
                        /* adding base adress */
                        word_without.R = 1;
                        word_without.opcode = symbol->base_address;
                        add_to_data_list(data_list, temp->address, 0, word_with, word_without);
                        /* adding offset */
                        word_without.opcode = symbol->offset;
                        temp = temp->next;
                        add_to_data_list(data_list, temp->address, 0, word_with, word_without);
                        reset_words(&word_with, &word_without);
                        if (temp->next != NULL)
                            temp = temp->next;
                    }
                }
            }
        }
        escape:
        strlcpy(previous_word, file_contents, strlen(previous_word));
    }
    
    /* STEP 7 */
    if (error_found_flag)
    {
        fprintf(stderr, "errors were found during second pass, stopping program\n");
        exit(0);
    }

    bubble_sort_data_list(data_list->head);
    build_final_files(data_list);
}
