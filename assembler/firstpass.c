#include "assembler.h"
#include "const.h"

extern FILE *post_macro_f;

/* step 1 */
int IC = 100;
int DC = 0;

void first_pass()
{
    const char *data_keyword = ".data";
    const char *string_keyword = ".string";
    const char *reserved_words[STOP + R15 + 2] = {"r0",
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
                                            "r15",
                                            "mov",
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
    
    char * line;
    char * temp;
    char ch;
    int label_found_flag = 0;
    int counter = 0;
    int temp_data_holder = 0;

    word_with_operands word_with;
    word_with_operands * p_word_with = NULL;

    word_without_operands word_without;
    word_without_operands * p_word_without = NULL;

    struct attributes label_attributes;

    data_linked_list * data_list;
    symbol_linked_list * symbol_list;

    symbol_list = create_empty_symbol_list();
    data_list = create_empty_data_list();

    line = (char *)malloc(MAX_LENGTH * sizeof(char));
    temp = (char *)malloc(MAX_LENGTH * sizeof(char));

    p_word_with = (word_with_operands *)malloc(sizeof(word_with_operands));
    p_word_with = &word_with;

    p_word_without = (word_without_operands *)malloc(sizeof(word_without_operands));
    p_word_without = &word_without;

    reset_attributes(&label_attributes);
    
    /* step 2 */
    while (fgets(line, MAX_LENGTH, post_macro_f))
    {
        printf("%s\n", line);

        counter = 0;
        /* looks for word until space is encounterd */
        /* TODO change to isspace */
        while (line[counter] != ' ')
        {
            counter++;
        }
        printf("here0\n");
        strcpy(temp, line);
        temp[strlen(temp) - (strlen(line) - counter)] = '\0';
        /*strncpy(temp, line, counter);*/
        counter = 0;
        printf("here1\n");
        /* step 3 */
        /* check if is a label by checking for a colon */
        if (strstr(temp, ":") != NULL)
        {
            printf("here2\n");
            label_found_flag = 1; /* step 4 */

            /* remove the label from the string */
            line = chop_first_n_characters(line, strlen(temp));
            line = skip_white_space_at_start(line);

            temp[strlen(temp) - 1] = '\0'; /* removes the colon */

        }
        printf("here3\n");

        /* step 5 */
        if (strstr(line, data_keyword) != NULL || strstr(line, string_keyword) != NULL)
        {
            printf("here4\n");
            /* step 6 */
            if (label_found_flag)
            {
                printf("here5\n");
                label_attributes.data = 1;
                /* TODO add error check of step 6 */
                add_to_symbol_list(symbol_list, temp, IC, calculate_base_adress(IC), IC - calculate_base_adress(IC), label_attributes);
                printf("here6\n");
            }
            /* step 7 */
            if (strstr(line, data_keyword) != NULL)
            {
                /* TODO Work on this section next */
                printf("here7\n");
                while (line != '\0')
                {
                    line = chop_first_n_characters(line, strlen(data_keyword)); /* removes .data */
                    line = skip_white_space_at_start(line);

                    /* look for number */
                    while (isdigit(line[counter]))
                    {
                        counter++;
                    }
                    printf("here8\n");
                    strncpy(temp, line, counter);
                    counter = 0;
                    line += strlen(temp) + 1;

                    temp_data_holder = atoi(temp);

                    word_without.A = 1;
                    word_without.opcode = temp_data_holder;
                    printf("here9\n");
                    add_to_data_list(data_list, IC, 0, p_word_with, p_word_without);
                    printf("here10\n");
                    printf("%d\n", data_list->head->word_without->opcode);
                }
            }
            /* TODO change this bit to get character by character and not entire string god fucking dammit */
            else if (strstr(line, string_keyword) != NULL)
            {
                printf("here11\n");
                while (line[counter] != '\n' && line[counter] != '\0')
                {
                    printf("gottem here\n");
                    printf("%s\n", line);
                    line = chop_first_n_characters(line, strlen(string_keyword));
                    line = skip_white_space_at_start(line);

                    printf("here12\n");
                    printf("%s\n", line);
                    do
                    {
                        counter++;
                        printf("%c\n", line[counter]);
                    } while (line[counter] != '"');

                    printf("here13\n");
                    strcpy(temp, line);
                    printf("%d\n", counter - 1);
                    temp[counter] = '\0';
                    /*temp[strlen(temp) - 1] = '\0';*/
                    printf("temp is : %s\n", temp);
                    printf("here14\n");
                    temp = chop_first_n_characters(temp, 1);
                    printf("%s\n", temp);
                    printf("%s\n", line);
                    printf("characters to chop : %d\n", strlen(temp) + 2);
                    printf("length of line is %d\n", strlen(line));
                    line = chop_first_n_characters(line, strlen(temp) + 2);
                    line = skip_white_space_at_start(line);
                    printf("%s\n", line);
                    counter = 0;
                    if(line[counter] == '\n' || line[counter] == '\0')
                        printf("found end\n");

                }
            }
        }
    }
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