#include "assembler.h"
#include "const.h"

data_linked_list * create_empty_data_list()
{
    data_linked_list * new_head;
    new_head = (data_linked_list *)malloc(sizeof(data_linked_list));
    new_head->head = NULL;
    
    return new_head;
}

data_node * add_to_data_list(data_linked_list * list, int new_base_adress, int operands_flag, struct word_with_operands word_with, struct word_without_operands word_without)
{

}