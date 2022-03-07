#include "assembler.h"

linked_list * create_empty_list()
{
    linked_list * new_head;
    new_head = (linked_list *)malloc(sizeof(linked_list));
    new_head->head = NULL;

    return new_head;
}

node * add_to_list(linked_list * list, char * new_name, char **new_text)
{
    int i, j;

    node * new_node;
    node * temp;

    new_node = (node *)malloc(sizeof(node));

    new_node->name = (char *)malloc(81 * sizeof(char));

    new_node->name = new_name;

    new_node->text = (char **)calloc(6, sizeof(char *));
    for (i = 0; i < 6; i++)
        new_node->text[i] = (char *)calloc(81, sizeof(char));

    for (i = 0; i < sizeof(new_text) / sizeof(new_text[0]); i++)
    {
        for (j = 0; j < sizeof(new_text[0]) / sizeof(new_text)[0][0]; j++)
        {
            new_node->text[i][j] = new_text[i][j];
        }
    }

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