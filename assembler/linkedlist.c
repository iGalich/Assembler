#include "assembler.h"
#include "linkedlist.h"

linked_list * create_empty_list()
{
    linked_list * new_head;
    new_head = (linked_list *)malloc(sizeof(linked_list));
    new_head->head = NULL;

    return new_head;
}

node * add_to_list(linked_list * list, void * data)
{
    node * new_node;
    node * temp;

    new_node = (node *)malloc(sizeof(node));
    new_node->data = malloc(sizeof(char *));
    new_node->data = (char *)data;
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

void print_list(linked_list * list)
{
    node * temp;
    temp = list->head;
    while (temp != NULL)
    {
        printf("%s", (char *)(temp->data));
        temp = temp->next;
    }
}