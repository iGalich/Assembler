#include "assembler.h"

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

/* NOTE: this function is unused in the code, and is only present for test functions */
void print_list(linked_list * list)
{
    node * temp;
    temp = list->head;
    /* We check that the next node is different from null because we don't want to print the end of the macro "endm" */
    while (temp != NULL && temp->next != NULL)
    {
        printf("%s", (char *)(temp->data));
        temp = temp->next;
    }
}

void print_node(node * node_name)
{
    printf("%s", (char *)(node_name->data));
}

int get_number_of_nodes(linked_list * list)
{
    node * temp;
    int count = 0;

    temp = list->head;

    while (temp != NULL)
    {
        temp = temp->next;
        count++;
    }

    return count;
}