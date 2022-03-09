#include "assembler.h"

linked_list * create_empty_list()
{
    linked_list * new_head;
    new_head = (linked_list *)malloc(sizeof(linked_list));
    new_head->head = NULL;

    return new_head;
}

node * add_to_list(linked_list * list, char * new_name, char **new_text, int number_of_rows)
{
    int i;

    node * new_node;
    node * temp;

    new_node = (node *)malloc(sizeof(node));

    new_node->name = (char *)malloc(81 * sizeof(char));

    strcpy(new_node->name, new_name);

    new_node->text = (char **)calloc(6, sizeof(char *));
    for (i = 0; i < 6; i++)
        new_node->text[i] = (char *)calloc(81, sizeof(char));

    for (i = 0; i < number_of_rows; i++)
    {
        strcpy(new_node->text[i], new_text[i]);
    }

    new_node->num_of_rows = number_of_rows;

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