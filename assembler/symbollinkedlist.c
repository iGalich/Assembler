#include "assembler.h"
#include "const.h"

symbol_linked_list * create_empty_symbol_list()
{
    symbol_linked_list * new_head;
    new_head = (symbol_linked_list *)malloc(sizeof(symbol_linked_list));
    new_head->head = NULL;

    return new_head;
}

symbol_node * add_to_symbol_list(symbol_linked_list * list, char * new_symbol, int new_decimal_value, int new_base_adress, int new_offset, struct attributes new_attributes)
{
    symbol_node * new_node;
    symbol_node * temp;

    new_node = (symbol_node *)malloc(sizeof(symbol_node));

    new_node->symbol =(char *)malloc(MAX_LENGTH * sizeof(char));
    strcpy(new_node->symbol, new_symbol);

    new_node->decimal_value = new_decimal_value;
    new_node->base_address = new_base_adress;
    new_node->offset = new_offset;
    new_node->symbol_attributes = new_attributes;

    new_node->next = NULL;

    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else
    {
        temp = list->head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_node;
    }
    return (list->head);
}

int get_number_of_symbol_nodes(symbol_linked_list * list)
{
    symbol_node * temp;
    int count = 0;

    temp = list->head;

    while (temp != NULL)
    {
        temp = temp->next;
        count++;
    }
    
    return count;
}