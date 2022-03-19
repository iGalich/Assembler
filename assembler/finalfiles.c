#include "assembler.h"

#define LAST_ADRESS 8191 /* Final address of our RAM */

extern int external_flag;
extern int entry_flag;
extern int ICF;
extern int DCF;

extern char * global_filename;

void build_final_files(data_linked_list * data_list, symbol_linked_list * symbol_list, address_linked_list * address_list)
{
    if (external_flag)
        create_external_file(symbol_list, address_list);

    if (entry_flag)
        create_entry_file(symbol_list);
    
    create_object_file(data_list);

    /* FINAL FREE */
    free(global_filename);
    free(address_list->head);
    free(data_list->head);
    free(symbol_list->head);
}

void create_external_file(symbol_linked_list * symbol_list, address_linked_list * address_list)
{
    FILE * external_file; /* .ext */

    symbol_node * temp_symbol = (symbol_node *)malloc(sizeof(symbol_node));
    address_node * temp_address = address_list->head;

    strcat(global_filename, ".ext");
    if (!(external_file = fopen(global_filename, "w+")))
    {
        fprintf(stderr, "cannot open external file\n");
        exit(0);
    }
    global_filename[strlen(global_filename) - 4] = '\0';

    while (temp_address != NULL)
    {
        if (!(temp_symbol = find_symbol_with_name(symbol_list, temp_address->string)))
            fprintf(stderr ,"couldn't find symbol with name %s\n", temp_address->string);

        if (temp_symbol->symbol_attributes.external == 1)
        {
            fprintf(external_file, "%s BASE %d\n", temp_address->string, temp_address->address);
            fprintf(external_file, "%s OFFSET %d\n\n", temp_address->string, temp_address->next_address);
        }
        temp_address = temp_address->next;
    }
}

void create_entry_file(symbol_linked_list * list)
{
    FILE * entry_file; /* .ent */

    symbol_node * temp = list->head;

    strcat(global_filename, ".ent");
    if (!(entry_file = fopen(global_filename, "w+")))
    {
        fprintf(stderr, "cannot open entry file\n");
        exit(0);
    }
    global_filename[strlen(global_filename) - 4] = '\0';

    while (temp != NULL)
    {
        if (temp->symbol_attributes.entry == 1)
            fprintf(entry_file, "%s,%d,%d\n", temp->symbol ,temp->base_address, temp->offset);
        
        temp = temp->next;
    }
}

void create_object_file(data_linked_list * list)
{
    FILE * object_file; /* .ob */

    int binary_array[20];
    int decimal_array[5];

    int starting_address = 100;
    int i = 0;
    int j = 0;

    char group_letter = 'A';

    data_node * temp = list->head;

    strcat(global_filename, ".ob");
    if (!(object_file = fopen(global_filename, "w+")))
    {
        fprintf(stderr, "cannot create object file\n");
        exit(0);
    }
    global_filename[strlen(global_filename) - 3] = '\0';

    fprintf(object_file, "%d %d\n", ICF - starting_address - DCF, DCF);

    while (temp != NULL)
    {
        fprintf(object_file, "%04d ", starting_address++);
        if (temp->operands_flag == 0)
        {
            binary_array[i++] = temp->word_without.msb;
            binary_array[i++] = temp->word_without.A;
            binary_array[i++] = temp->word_without.R;
            binary_array[i++] = temp->word_without.E;
            int_to_binary(temp->word_without.opcode, 16, binary_array + i);
        }
        else /* if temp->operands_flag == 1 aka working on word with operands*/
        {
            binary_array[i++] = temp->word_with.msb;
            binary_array[i++] = temp->word_with.A;
            binary_array[i++] = temp->word_with.R;
            binary_array[i++] = temp->word_with.E;
            int_to_binary(temp->word_with.function, 4, binary_array + i);
            i += 4;
            int_to_binary(temp->word_with.source_register, 4, binary_array + i);
            i += 4;
            int_to_binary(temp->word_with.source_address_mode, 2, binary_array + i);
            i += 2;
            int_to_binary(temp->word_with.destination_register, 4, binary_array + i);
            i += 4;
            int_to_binary(temp->word_with.destination_address_mode, 2, binary_array + i);
        }
        i = 0;

        for (i = 0; i < 5; i++)
        {
            decimal_array[i] = binary_to_decimal(binary_array + j);
            j += 4;
        }
        j = 0;
        fprintf(object_file, "%c%x-", group_letter++, decimal_array[j++]);
        i += 4;
        fprintf(object_file, "%c%x-", group_letter++, decimal_array[j++]);
        i += 4;
        fprintf(object_file, "%c%x-", group_letter++, decimal_array[j++]);
        i += 4;
        fprintf(object_file, "%c%x-", group_letter++, decimal_array[j++]);
        i += 4;
        fprintf(object_file, "%c%x", group_letter++, decimal_array[j++]);

        fprintf(object_file, "\n");

        temp = temp->next;
        group_letter = 'A';
        i = j = 0;
    }
    if (starting_address > LAST_ADRESS)
        fprintf(stderr, "RAM memory exceeded\n");
}

int binary_to_decimal(int * arr)
{
    int decimal_value = 0;
    int i = 0;

    /* this function only converts 4 binary digits to their decimal value */
    while (i < 4)
    {
        decimal_value += arr[i] * power(2, 3 - i);
        i++;
    }
    return decimal_value;
}

void int_to_binary(unsigned int in, int count, int * out)
{
    unsigned int mask = 1U << (count - 1);
    int i;
    for (i = 0; i < count; i++) 
    {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}