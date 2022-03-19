#include "assembler.h"

address_linked_list * create_empty_address_list()
{
    address_linked_list * new_head;
    new_head = (address_linked_list *)malloc(sizeof(address_linked_list));
    new_head->head = NULL;

    return new_head;
}

void free_address_list(address_node * head)
{
    address_node * temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

address_node * add_to_address_list(address_linked_list * list, int new_address, int new_next_adress, char * string)
{
    address_node * new_node;
    address_node * temp;

    new_node = (address_node *)malloc(sizeof(address_node));
    
    new_node->string = (char*)malloc(81 * sizeof(char));
    strcpy(new_node->string, string);
    new_node->address = new_address;
    new_node->next_address = new_next_adress;
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

void print_address_list(address_linked_list * list)
{
    address_node * temp;
    temp = list->head;

    while (temp != NULL)
    {
        printf("missing address is %d\n", temp->address);
        printf("missing address is %d\n", temp->next_address);
        printf("name is %s\n", temp->string);
        temp = temp->next;
    }
}