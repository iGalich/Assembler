#include <math.h>
#include "assembler.h"

extern FILE *post_macro_f;
extern FILE * copy_f;

extern char * global_filename;

int external_flag;
int entry_flag;
int ICF;
int DCF;

const char * command_names[16] = {"mov",
                                  "cmp",
                                  "add",
                                  "sub",
                                  "lea",
                                  "clr",
                                  "not",
                                  "inc",
                                  "dec",
                                  "jmp",
                                  "bne",
                                  "jsr",
                                  "red",
                                  "prn",
                                  "rts",
                                  "stop"};

const char * register_names[R15 + 1] =  {"r0",
                                        "r1",
                                        "r2",
                                        "r3",
                                        "r4",
                                        "r5",
                                        "r6",
                                        "r7",
                                        "r8",
                                        "r9",
                                        "r10",
                                        "r11",
                                        "r12",
                                        "r13",
                                        "r14",
                                        "r15"};

/* STEP 1 initialization */
int IC = 100;
int DC = 0;
int L = 0;

void first_pass()
{
    struct stat sb;

    char * file_contents;
    char * previous_word;
    char * symbol_word;

    char ch;

    const char * data_keyword = ".data";
    const char * string_keyword = ".string";
    const char * entry_keyword = ".entry";
    const char * extern_keyword = ".extern";
    const char * macro_start_keyword = "macro";
    const char * macro_end_keyword = "endm";
    

    int symbol_first_flag = 0;
    int label_found_flag = 0;
    int j, k, x;
    int temp_data_holder = 0;
    int command_index = 0;
    int error_found_flag = 0;
    int register_index = 0;
    int global_L = 0;
    int word_count = 0;


    word_with_operands word_with;
    word_without_operands word_without;

    struct attributes label_attributes;

    data_linked_list * data_list;
    symbol_linked_list * symbol_list;
    address_linked_list * address_list;

    symbol_list = create_empty_symbol_list();
    data_list = create_empty_data_list();
    address_list = create_empty_address_list();

    reset_attributes(&label_attributes);

    reset_words(&word_with, &word_without);

    if (!(copy_f = fopen("copy.am", "w+")))
    {
        fprintf(stderr, "cannot create copy file\n");
        exit(0);
    }
    
    while ((ch = fgetc(post_macro_f)) != EOF){
        fputc(ch, copy_f);
    }

    rewind(post_macro_f);
    rewind(copy_f);

    if (stat("copy.am", &sb) == -1)
    {
        perror("stat error\n");
        exit(0);
    }   

    previous_word = malloc(sb.st_size);
    file_contents = malloc(sb.st_size);
    symbol_word = malloc(sb.st_size);

    external_flag = entry_flag = 0;

    word_count = count_number_of_words_in_file(post_macro_f);
    
    /* STEP 2 get line, if not goto step 17 */
    /*while(fscanf(post_macro_f, " %[^\n ]", file_contents) != EOF)*/
    for(k = 0; k < word_count; k++)
    {
        fscanf(post_macro_f, " %[^\n ]", file_contents);
        step_02:
        /* skip over macro definition */
        if (!strcmp(file_contents, macro_start_keyword))
        {
            while (strcmp(file_contents, macro_end_keyword))
            {
                fscanf(post_macro_f, " %[^\n ]", file_contents);
                k++;
                if (k >= word_count)
                    goto escape;
            }
            continue; /* going for next word after finding end of macro definition */
        }

        label_found_flag = 0;
        /* STEP 3 check for label, if not goto step 5 */
        if (strstr(file_contents, ":") != NULL)
        {
            /* STEP 4 label found -> flag on */
            label_found_flag = 1;
            file_contents[strlen(file_contents) - 1] = '\0'; /* remove colon */
            strlcpy(previous_word, file_contents, strlen(file_contents) + 1);

            fscanf(post_macro_f, " %[^\n ]", file_contents);
            k++;
            if (k >= word_count)
                    goto escape;
        }

        /* STEP 5 check if it is an instruction to store data, if not goto step 8 */
        if (strstr(file_contents, data_keyword) != NULL || strstr(file_contents, string_keyword) != NULL)
        {
            /* STEP 6 store symbol */
            if (label_found_flag)
            {
                label_attributes.data = 1;

                for (x = 0; x < 16; x++)
                {
                    if (!strcmp(previous_word, command_names[x]) || !strcmp(previous_word, register_names[x]))
                    {
                        fprintf(stderr, "symbol %s uses a reserved keyword\n", previous_word);
                        error_found_flag = 1;
                    }
                }
                add_to_symbol_list(symbol_list, previous_word, IC, calculate_base_adress(IC), IC - calculate_base_adress(IC), label_attributes);
                reset_attributes(&label_attributes);
                label_found_flag = 0;
            }
            /* STEP 7 check which data type to store, store and then increase DC; return to step 2 */
            if (strstr(file_contents, data_keyword) != NULL)
            {
                step_07_data:
                fscanf(post_macro_f, " %[^\n ]", file_contents);
                k++;
                if (k >= word_count)
                    goto escape;

                /* check if it is not a number, if true it means we got to the next line */
                if (isalpha(file_contents[0]) || file_contents[0] == '.')
                    goto step_02;

                if (file_contents[0] == ',')
                    file_contents = chop_first_n_characters(file_contents, 1); /* remove comma from start */
                else if (file_contents[strlen(file_contents) - 1] == ',')
                    file_contents[strlen(file_contents) - 1] = '\0'; /* remove comma from end */

                temp_data_holder = atoi(file_contents);

                word_without.A = 1;
                word_without.opcode = temp_data_holder;

                add_to_data_list(data_list, IC, 0, word_with, word_without);
                IC++;
                reset_words(&word_with, &word_without);

                DC++;

                goto step_07_data;
            }
            else /* we know for sure it is a string data type */
            {
                step_07_string:

                fscanf(post_macro_f, " %[^\n ]", file_contents);
                k++;
                if (k >= word_count)
                    goto escape;

                /* check if it is a string, otherwise we got to the next line */
                if (file_contents[0] != '"')
                    goto step_02;

                if (file_contents[0] == '"')
                    file_contents = chop_first_n_characters(file_contents, 1); /* remove quotation marks */
                if (file_contents[strlen(file_contents) - 1] == '"')
                    file_contents[strlen(file_contents) - 1] = '\0'; /* remove quotation marks */

                word_without.A = 1;
                /* adding each letter */
                for (j = 0; j < strlen(file_contents); j++)
                {
                    word_without.opcode = file_contents[j];
                    add_to_data_list(data_list, IC, 0, word_with, word_without);
                    IC++;
                    DC++;
                }
                word_without.opcode = 0;
                add_to_data_list(data_list, IC, 0, word_with, word_without);
                reset_words(&word_with, &word_without);
                IC++;
                DC++;

                goto step_07_string;
            }
        continue; /* go back to step 2 */
        }
        
        if (label_found_flag)
        {
            label_attributes.code = 1;
            add_to_symbol_list(symbol_list, previous_word, IC, calculate_base_adress(IC), IC - calculate_base_adress(IC), label_attributes);
            reset_attributes(&label_attributes);
            label_found_flag = 0;
        }
        
        /* STEP 8 check if extern or entry, else goto step 11 */
        else if (strstr(file_contents, extern_keyword))
        {
            external_flag = 1;
            /* STEP 10 add symbol to symbol list with extern attribute */
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            k++;
            if (k >= word_count)
                    goto escape;

            for (x = 0; x < 16; x++)
            {
                if (!strcmp(previous_word, command_names[x]) || !strcmp(previous_word, register_names[x]))
                {
                        fprintf(stderr, "symbol %s uses a reserved keyword\n", previous_word);
                        error_found_flag = 1;
                }
            }

            label_attributes.external = 1;
            add_to_symbol_list(symbol_list, file_contents, 0, 0, 0, label_attributes);

            reset_attributes(&label_attributes);
            continue;
        }
        /* STEP 9 if entry go back to step 2 */
        else if (strstr(file_contents, entry_keyword))
        {
            entry_flag = 1;
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            k++;
            if (k >= word_count)
                    goto escape;

            continue;
        }
        /* STEP 11 check for label, add if true */
        else if (strstr(file_contents, ":"))
        {
            strlcpy(previous_word, file_contents, strlen(file_contents) + 1);

            fscanf(post_macro_f, " %[^\n ]", file_contents);
            k++;
            if (k >= word_count)
                    goto escape;

            previous_word[strlen(previous_word) - 1] = '\0'; /* remove colon */

            label_attributes.code = 1;
            add_to_symbol_list(symbol_list, previous_word, IC, calculate_base_adress(IC), IC - calculate_base_adress(IC), label_attributes);
            reset_attributes(&label_attributes);
        }
        /* STEP 12 check for command */
        if ((command_index = get_command_index(file_contents)) == -1)
        {
            error_found_flag = 1;
            fprintf(stderr ,"found error with command index %s\n", file_contents);
        }
        
        /* first group, commands that get 2 operands */
        if (command_index >= MOV && command_index <= LEA)
        {
            word_without.A = 1;
            
            switch (command_index)
            {
            case MOV:
                word_without.opcode = pow(2, 0);
                break;
            case CMP:
                word_without.opcode = pow(2, 1);
                break;
            case ADD:
            case SUB:
                word_without.opcode = pow(2, 2);
                break;
            case LEA:
                word_without.opcode = pow(2, 4);
                break;

            /* UNREACHABLE */
            default:
                break;
            }

            add_to_data_list(data_list, IC, 0, word_with, word_without);
            reset_words(&word_with, &word_without);
            IC++;
            /* check the source operand */
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            k++;
            if (k >= word_count)
                    goto escape;

            word_with.A = 1;
            if (command_index == ADD)
                word_with.function = 10;
            else if (command_index == SUB)
                word_with.function = 11;

            register_index = get_register_index(file_contents);

            if (register_index == -1)
                word_with.source_register = 0;
            else
                word_with.source_register = register_index;
            
            word_with.source_address_mode = get_address_mode(file_contents, register_index);
            if (L == 2 && (word_with.source_address_mode == 1 || word_with.source_address_mode == 2))
            {
                if (word_with.source_address_mode == 2)
                {
                    file_contents[strcspn(file_contents, "[")] = '\0';
                }

                strcpy(symbol_word, file_contents);
                symbol_first_flag = 1;
            }
            if (word_with.source_address_mode == 0)
            {
                word_without.A = 1;
                file_contents = chop_first_n_characters(file_contents, 1); /* removes hashtag */
                word_without.opcode = atoi(file_contents);
                add_to_data_list(data_list, IC, 0, word_with, word_without);
                IC++;
                reset_word_without(&word_without);
            }

            /* check the destination operand */
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            k++;
            if (k >= word_count)
                    goto escape;

            register_index = get_register_index(file_contents);

            if (register_index == -1)
                word_with.destination_register = 0;
            else
                word_with.destination_register = register_index;

            word_with.destination_address_mode = get_address_mode(file_contents, register_index);
            if (L == 2 && (word_with.destination_address_mode == 1 || word_with.destination_address_mode == 2))
            {
                if (word_with.destination_address_mode == 2)
                {
                    file_contents[strcspn(file_contents, "[")] = '\0';
                }

                strcpy(symbol_word, file_contents);
            }

            add_to_data_list(data_list, IC, 1, word_with, word_without);
            IC++;
            
            if (word_with.destination_address_mode == 0 && !symbol_first_flag)
            {
                word_without.A = 1;
                file_contents = chop_first_n_characters(file_contents, 1); /* removes hashtag */
                word_without.opcode = atoi(file_contents);
                add_to_data_list(data_list, IC, 0, word_with, word_without);
                IC++;
                reset_word_without(&word_without);
            }
            if (!symbol_first_flag)
                reset_words(&word_with, &word_without);
        }
        /* second group, commands with a single operands */
        else if (command_index >= CLR && command_index <= PRN)
        {
            word_without.A = 1;
            switch (command_index)
            {
            case CLR:
            case NOT:
            case INC:
            case DEC:
                word_without.opcode = pow(2, 5);
                break;
            case JMP:
            case BNE:
            case JSR:
                word_without.opcode = pow(2, 9);
                break;
            case RED:
                word_without.opcode = pow(2, 12);
                break;
            case PRN:
                word_without.opcode = pow(2, 13);
                break;
            
            /* UNREACHABLE */
            default:
                break;
            }
            add_to_data_list(data_list, IC, 0, word_with, word_without);
            IC++;
            reset_words(&word_with, &word_without);

            /* check single operand */
            fscanf(post_macro_f, " %[^\n ]", file_contents);
            k++;
            if (k >= word_count)
                    goto escape;

            word_with.A = 1;
            if (command_index == CLR || command_index == JMP)
                word_with.function = 10;
            else if (command_index == NOT || command_index == BNE)
                word_with.function = 11;
            else if (command_index == INC || command_index == JSR)
                word_with.function = 12;
            else if (command_index == DEC)
                word_with.function = 13;
            
            word_with.source_register = 0;
            word_with.source_address_mode = 0;

            register_index = get_register_index(file_contents);

            if (register_index == -1)
                word_with.destination_register = 0;
            else
                word_with.destination_register = register_index;

            word_with.destination_address_mode = get_address_mode(file_contents, register_index);

            if (L == 2 && (word_with.destination_address_mode == 1 || word_with.destination_address_mode == 2))
            {
                if (word_with.destination_address_mode == 2)
                {
                    file_contents[strcspn(file_contents, "[")] = '\0';
                }

                strcpy(symbol_word, file_contents);
            }

            add_to_data_list(data_list, IC, 1, word_with, word_without);
            IC++;
            reset_word_without(&word_without);
            if (word_with.destination_address_mode == 0)
            {
                word_without.A = 1;
                file_contents = chop_first_n_characters(file_contents, 1);
                word_without.opcode = atoi(file_contents);
                add_to_data_list(data_list, IC, 0, word_with, word_without);
                IC++;
                reset_words(&word_with, &word_without);
            }
            reset_words(&word_with, &word_without);
        }

        /* third group, no operands */
        else if (command_index == RTS || command_index == STOP)
        {
            word_without.A = 1;

            if (command_index == RTS)
                word_without.opcode = pow(2, 14);
            else
                word_without.opcode = pow(2, 15);

            add_to_data_list(data_list, IC, 0, word_with, word_without);
            reset_words(&word_with, &word_without);
            IC++;
        }
        IC += L; /* STEP 16 */
        if (L == 2)
        {
            add_to_address_list(address_list, IC - L, IC - L + 1, symbol_word);
        }
        if (symbol_first_flag && word_with.destination_address_mode == 0)
        {
            word_without.A = 1;
            file_contents = chop_first_n_characters(file_contents, 1); /* removes hashtag */
            word_without.opcode = atoi(file_contents);
            add_to_data_list(data_list, IC, 0, word_with, word_without);
            IC++;
            reset_word_without(&word_without);
            reset_words(&word_with, &word_without);
        }
        symbol_first_flag = 0;
        global_L += L;
        L = 0;
        continue; /* back to step 2 */
    }
    escape:
    /* STEP 17, after reading entire file, stop if errors were found */
    if (error_found_flag)
    {
        fprintf(stderr, "errors were found during first pass, stopping program\n");
        exit(0);
    }

    /* STEP 18 , save IC and DC for use in second pass*/
    ICF = IC;
    DCF = DC;

    /* STEP 20 , begin second pass */
    rewind(post_macro_f);

    second_pass(symbol_list, data_list, address_list);
}

int count_number_of_words_in_file(FILE * file)
{
    int count = 0;
    char ch, prev_c = 'a';

    while ((ch = getc(file)) != EOF)
    {
        if (isspace(ch) && isspace(prev_c))
        {
            prev_c = ch;
            continue;
        }
        if (isspace(ch))
        {
            count++;
        }
        prev_c = ch;
    }
    rewind(file);
    return count;
}

int get_address_mode(char * string, int index)
{
    if (string[0] == ',')
        string = chop_first_n_characters(string, 1);
    if (string[strlen(string) - 1] == ',')
        string[strlen(string) - 1] = '\0';

    if (string[0] == '#')
        return 0;
    if (strstr(string, "["))
    {
        L += 2;
        return 2;
    }
    if (index >= 0 && !strcmp(string, register_names[index]))
        return 3;
    /* direct address, using a label */
    else
    {
        L += 2;
        return 1;
    }
}

int get_register_index(char * string)
{
    int i;

    if (string[0] == ',')
        string = chop_first_n_characters(string, 1);
    if (string[strlen(string) - 1] == ',')
        string[strlen(string) - 1] = '\0';

    for (i = 0; i < 16; i++)
    {
        if (!strcmp(string, register_names[i]))
            return i;
    }
    return -1;
}

int get_command_index(char * command)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (!strcmp(command, command_names[i]))
            return i;
    }
    fprintf(stderr, "%s is not an applicable command\n", command);
    return -1;
}

void reset_word_without(struct word_without_operands * word)
{
    word->A = 0;
    word->E = 0;
    word->msb = 0;
    word->opcode = 0;
    word->R = 0;
}

void reset_words(struct word_with_operands * word_with, struct word_without_operands * word_without)
{
    word_with->A = 0;
    word_with->destination_address_mode = 0;
    word_with->destination_register = 0;
    word_with->E = 0;
    word_with->function = 0;
    /* word_with->msb = 0; Always set to zero */
    word_with->R = 0;
    word_with->source_address_mode = 0;
    word_with->source_register = 0;

    word_without->A = 0;
    word_without->E = 0;
    word_without->msb = 0;
    word_without->opcode = 0;
    word_without->R = 0;
}

int calculate_base_adress(int num)
{
    while (num % 16 != 0)
        num--;
    return num;
}

void reset_attributes(struct attributes * label_attributes)
{
    label_attributes->code = 0;
    label_attributes->data = 0;
    label_attributes->entry = 0;
    label_attributes->external = 0;
}