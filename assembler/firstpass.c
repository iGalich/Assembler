#include "assembler.h"
#include "const.h"

extern FILE *post_macro_f;
extern FILE * copy_f;

extern char * global_filename;

/* STEP 1 initialization */
int IC = 100;
int DC = 0;

void first_pass()
{
    struct stat sb;

    char * file_contents;
    char * previous_word;
    char * line;

    char ch;

    const char * data_keyword = ".data";
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

    int num_of_words = 0;
    int label_found_flag = 0;
    int j, i = 0;
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

    p_word_with = (word_with_operands *)malloc(sizeof(word_with_operands));
    p_word_with = &word_with;

    p_word_without = (word_without_operands *)malloc(sizeof(word_without_operands));
    p_word_without = &word_without;

    line = (char *)malloc(MAX_LENGTH * sizeof(char));

    reset_attributes(&label_attributes);

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

    file_contents = malloc(sb.st_size);
    previous_word = malloc(sb.st_size);

    /* STEP 2 get line, if not goto step 17 */
    while(fscanf(post_macro_f, " %[^\n ]", file_contents) != EOF)
    {
        step_02:
        printf("%s\n", file_contents);
        /* STEP 3 check for label, if not goto step 5 */
        if (strstr(file_contents, ":") != NULL)
        {
            /* STEP 4 label found -> flag on */
            label_found_flag = 1;
            file_contents[strlen(file_contents) - 1] = '\0'; /* remove colon */

            strlcpy(previous_word, file_contents, strlen(previous_word));

            fscanf(post_macro_f, " %[^\n ]", file_contents);
        }

        /* STEP 5 check if it is an instruction to store data, if not goto step 8 */
        if (strstr(file_contents, data_keyword) != NULL || strstr(file_contents, string_keyword) != NULL)
        {
            /* STEP 6 store symbol */
            if (label_found_flag)
            {
                label_attributes.data = 1;
                /* TODO add error check of step 6 */
                add_to_symbol_list(symbol_list, previous_word, IC, calculate_base_adress(IC), IC - calculate_base_adress(IC), label_attributes);

                reset_attributes(&label_attributes);
            }
            /* STEP 7 check which data type to store, store and then increase DC; return to step 2 */
            if (strstr(file_contents, data_keyword) != NULL)
            {
                step_07_data:

                fscanf(post_macro_f, " %[^\n ]", file_contents);

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

                printf("before add %d\n", temp_data_holder);

                add_to_data_list(data_list, IC, 0, p_word_with, p_word_without);

                printf("added %d\n", data_list->head->word_without->opcode);

                reset_words(p_word_with, p_word_without);

                DC++;

                goto step_07_data;
            }
            else /* we know for sure it is a string data type */
            {
                step_07_string:

                fscanf(post_macro_f, " %[^\n ]", file_contents);

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
                    printf("before add %c\n", file_contents[j]);

                    word_without.opcode = file_contents[j];
                    add_to_data_list(data_list, IC, 0, p_word_with, p_word_without);
                    printf("added %d\n", data_list->head->word_without->opcode);
                }
                reset_words(p_word_with, p_word_without);

                DC++;

                goto step_07_string;
            }
        continue; /* go back to step 2 */
        }
    }
}


void reset_words(struct word_with_operands * word_with, struct word_without_operands * word_without)
{
    word_with->A = 0;
    word_with->destination_adress = 0;
    word_with->destination_register = 0;
    word_with->E = 0;
    word_with->function = 0;
    word_with->msb = 0;
    word_with->R = 0;
    word_with->source_adress = 0;
    word_with->source_register = 0;

    word_without->A = 0;
    word_without->E = 0;
    word_without->msb = 0;
    word_without->opcode = 0;
    word_without->R = 0;
}

void first_pass2()
{
    struct stat sb;

    char *file_contents;

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
    
    if (stat("text1.am", &sb) == -1)
    {
        perror("stat\n");
        exit(0);
    }    

    file_contents = malloc(sb.st_size);

    while(fscanf(post_macro_f, " %[^\n ]", file_contents) != EOF)
    {
        printf("%s\n", file_contents);
    }

    exit(0);

    /* step 2 */
    while (fgets(line, MAX_LENGTH, post_macro_f) != NULL)
    {
        line = skip_white_space_at_start(line);
        line[strcspn(line, "\n")] = '\0';
        memset(temp, 0, strlen(temp));


        counter = 0;
        /* looks for word until space is encounterd */
        /* TODO change to isspace */
        while (!isspace(line[counter]))
        {
            counter++;
        }

        /*strcpy(temp, line);*/
        strlcpy(temp, line, strlen(line));

        temp[counter] = '\0';
        counter = 0;

        /* step 3 */
        /* check if is a label by checking for a colon */
        if (strstr(temp, ":") != NULL)
        {

            label_found_flag = 1; /* step 4 */

            /* remove the label from the string */
            line = chop_first_n_characters(line, strlen(temp));
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

                line = chop_first_n_characters(line, strlen(data_keyword)); /* removes .data */
                line = skip_white_space_at_start(line);
                while (line[counter] != '\n' && line[counter] != '\0')
                {

                    /* look for number */
                    while (isdigit(line[counter]) || line[counter] == '-')
                    {
                        counter++;
                    }

                    /*strncpy(temp, line, counter);*/
                    strlcpy(temp, line, counter + 1);

                    counter = 0;
                    line += strlen(temp) + 1;

                    temp_data_holder = atoi(temp);

                    word_without.A = 1;
                    word_without.opcode = temp_data_holder;

                    add_to_data_list(data_list, IC, 0, p_word_with, p_word_without);

                    printf("added %d\n", data_list->head->word_without->opcode);
                }
            }
            /* TODO change this bit to get character by character and not entire string god fucking dammit */
            else if (strstr(line, string_keyword) != NULL)
            {

                while (line[counter] != '\n' && line[counter] != '\0')
                {

                    line = chop_first_n_characters(line, strlen(string_keyword));
                    line = skip_white_space_at_start(line);


                    do
                    {
                        counter++;

                    } while (line[counter] != '"');


                    /*strcpy(temp, line);*/
                    strlcpy(temp, line, strlen(line));

                    temp[counter] = '\0';
                    /*temp[strlen(temp) - 1] = '\0';*/

                    temp = chop_first_n_characters(temp, 1);

                    line = chop_first_n_characters(line, strlen(temp) + 2);
                    line = skip_white_space_at_start(line);

                    counter = 0;
                    if(line[counter] == '\n' || line[counter] == '\0')
                        printf("found end\n");

                }
            }
        }
        /*memset(line, 0, strlen(line));*/
        line[0] = '\0';
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