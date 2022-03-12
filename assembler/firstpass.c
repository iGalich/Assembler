#include "assembler.h"
#include "const.h"

typedef struct data_node {
    int address;
    int operands_flag;
    struct word_with_opernads word_with;
    struct word_without_opernds word_without;
    struct data_node * next;
} data_node;

typedef struct {
    data_node * head;
} data_linked_list;

data_linked_list * create_empty_data_list();

data_node * add_to_data_list(data_linked_list * list, int new_base_adress, int operands_flag, struct word_with_operands word_with, struct word_without_operands word_without);

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
    word_with_operands * p_word_with = &word_with;

    word_without_operands word_without;
    word_without_operands * p_word_without = &word_without;

    struct attributes label_attributes;

    data_linked_list * data_list;
    symbol_linked_list * symbol_list;

    symbol_list = create_empty_symbol_list();
    data_list = create_empty_data_list();

    line = (char *)malloc(MAX_LENGTH * sizeof(char));
    temp = (char *)malloc(MAX_LENGTH * sizeof(char));

    reset_attributes(&label_attributes);

    /* step 2 */
    while (fgets(line, MAX_LENGTH, post_macro_f))
    {
        /* looks for word until space is encounterd */
        /* TODO change to isspace */
        while (line[counter] != ' ')
        {
            counter++;
        }
        strncpy(temp, line, counter);
        counter = 0;

        /* step 3 */
        /* check if is a label by checking for a colon */
        if (strstr(temp, ":") != NULL)
        {
            label_found_flag = 1; /* step 4 */

            /* remove the label from the string */
            line += strlen(temp);
            line = skip_white_space_at_start(line);

            temp[strlen(temp) - 1] = '\0'; /* removes the colon */
        }

        /* step 5 */
        if (strstr(line, data_keyword) != NULL || strstr(line, string_keyword) != NULL)
        {
            /* step 6 */
            if (label_found_flag)
            {
                label_attributes.data = 1;
                /* TODO add error check of step 6 */
                add_to_symbol_list(symbol_list, temp, IC, calculate_base_adress(IC), IC - calculate_base_adress(IC), label_attributes);
            }
            /* step 7 */
            if (strstr(line, data_keyword) != NULL)
            {
                while (line != '\0')
                {
                    line += strlen(data_keyword); /* removes .data */
                    line = skip_white_space_at_start(line);

                    /* look for number */
                    while (!isdigit(line[counter]))
                    {
                        counter++;
                    }
                    strncpy(temp, line, counter);
                    counter = 0;
                    line += strlen(temp) + 1;

                    temp_data_holder = atoi(temp);

                    word_without.A = 1;
                    word_without.opcode = temp_data_holder;

                    add_to_data_list(data_list, IC, 0, p_word_with, p_word_without);
                }
            }
            else if (strstr(line, string_keyword) != NULL)
            {
                while (line != '\0')
                {
                    line += strlen(string_keyword);
                    line = skip_white_space_at_start(line);

                    do
                    {
                        counter++;
                    } while (line[counter] != '"');
                    strncpy(temp, line, counter);
                    temp += 1;
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