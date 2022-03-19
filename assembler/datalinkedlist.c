#include "assembler.h"

/* This linked list is used to store the data of each word */

data_linked_list * create_empty_data_list()
{
    data_linked_list * new_head;
    new_head = (data_linked_list *)malloc(sizeof(data_linked_list));
    new_head->head = NULL;
    
    return new_head;
}

void free_data_list(data_node * head)
{
    data_node * temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void print_all_address(data_linked_list * list)
{
    data_node * temp;
    int prev_add = 99;
    temp = list->head;

    while (temp != NULL)
    {
        printf("address is %d\n", temp->address);
        prev_add = temp->address;
        temp = temp->next;
        if (temp != NULL && prev_add + 1 != temp->address)
            printf("STANGE JUMP\n");
    }
}

void print_data_list(data_linked_list * list)
{
    data_node * temp;

    temp = list->head;

    while (temp != NULL)
    {
        printf("adress is %d\n", temp->address);
        if (temp->operands_flag == 0)
        {
            printf("A is %d , E is %d , R is %d , opcode is %d\n", temp->word_without.A, temp->word_without.E, temp->word_without.R, temp->word_without.opcode);
        }
        else
        {
            printf("A is %d \n", temp->word_with.A);
            printf("E is %d\n", temp->word_without.E);
            printf("R is %d\n", temp->word_with.R);
            printf("source register is %d and source address mode is %d \n", temp->word_with.source_register, temp->word_with.source_address_mode);
            printf("des register is %d and des address mode is %d \n", temp->word_with.destination_register, temp->word_with.destination_register);
            printf("function is %d\n", temp->word_with.function);
        }
        temp = temp->next;
    }
}

void bubble_sort_data_list(data_node * start)
{
    int swapped, i;
    data_node * pointer1;
    data_node * left_pointer = NULL;
  
    /* Checking for empty list */
    if (start == NULL)
        return;
  
    do
    {
        swapped = 0;
        pointer1 = start;
  
        while (pointer1->next != left_pointer)
        {
            if (pointer1->address > pointer1->next->address)
            { 
                swap_data_nodes(pointer1, pointer1->next);
                swapped = 1;
            }
            pointer1 = pointer1->next;
        }
        left_pointer = pointer1;
    }
    while (swapped);
    
}

void swap_data_nodes(data_node * a, data_node * b)
{
    int temp = a->address;
    a->address = b->address;
    b->address = temp;
}

data_node * add_to_data_list(data_linked_list * list, int new_base_adress, int new_operands_flag, struct word_with_operands new_word_with, struct word_without_operands new_word_without)
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