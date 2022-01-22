#include "assembler.h"

typedef struct string_node * pointer;
typedef struct string_node {
    char * string;
    pointer next;
} string_node;

/*
 * This functions adds a node to the list.
 */
void add_to_list(pointer *, char *);

/*
 * This function frees the nodes of the list from the memory.
 */
void free_list(pointer *);

/*
 * This functions prints the nodes of a given list.
 */
void print_list(pointer);

void print_list(pointer ptr)
{
    while(ptr)
    {
        printf("%s\n", ptr->string);
        ptr = ptr->next;
    }
}

void free_list(pointer * ptr)
{
    pointer p;

    while (*ptr)
    {
        p = *ptr;
        *ptr = (*ptr)->next;
        free(p);
    }
}

void add_to_list(pointer * ptr, char * str)
{
    pointer p, new_node;

    new_node = (pointer) malloc(sizeof(string_node));
    if (!new_node)
    {
        printf("Cannot build list\n");
        exit(0);
    }

    new_node->string = str;

    p = *ptr;

    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = new_node;
    free(p);
}