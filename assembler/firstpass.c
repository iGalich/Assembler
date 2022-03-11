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
    char temp[MAX_LENGTH];
    char ch;
    int labal_found_flag = 0;
    int counter = 0;

    struct attributes label_attributes;

    data_linked_list * data_list;
    symbol_linked_list * symbol_list;

    symbol_list = create_empty_symbol_list();
    data_list = create_empty_data_list();

    line = (char *)malloc(MAX_LENGTH * sizeof(char));

    reset_attributes(&label_attributes);

    /* step 2 */
    while (fgets(line, MAX_LENGTH, post_macro_f))
    {
        /* looks for word until space is encounterd */
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
            labal_found_flag = 1; /* step 4 */

            /* remove the labal from the string */
            line += strlen(temp) + 1;
            line = skip_white_space_at_start(line);

            /* looks for word until space is encounterd */
            while (line[counter] != ' ')
            {
                counter++;
            }
            strncpy(temp, line, counter);
            counter = 0;
        }

        /* step 5 */
        if (strstr(temp, data_keyword) != NULL || strstr(temp, string_keyword) != NULL)
        {
            /* step 6 */
            if (labal_found_flag)
            {
                label_attributes.data = 1;
                /* TODO add error check of step 6 */
                add_to_symbol_list(symbol_list, temp, IC, calculate_base_adress(IC), IC - calculate_base_adress(IC), label_attributes);
            }
            /* step 7 */
            else
            {
                if (strstr(temp, data_keyword) != NULL)
                {

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