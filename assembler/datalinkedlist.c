#include "assembler.h"
#include "const.h"

data_linked_list * create_empty_data_list()
{
    data_linked_list * new_head;
    new_head = (data_linked_list *)malloc(sizeof(data_linked_list));
    new_head->head = NULL;
    
    return new_head;
}

data_node * add_to_data_list(data_linked_list * list, int new_base_adress, int new_operands_flag, word_with_operands * new_word_with, word_without_operands * new_word_without)
{
    data_node * new_node;
    data_node * temp;

    new_node = (data_node *)malloc(sizeof(data_node));

    new_node->address = new_base_adress;
    new_node->operands_flag = new_operands_flag;
    new_node->word_with = new_word_with;
    new_node->word_without = new_word_without;

    new_node->next = NULL;

    if(list->head == NULL)
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