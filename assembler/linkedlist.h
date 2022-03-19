/* This linked list is used to store information about macros */

typedef struct node {
    char * name;
    char **text;
    struct node * next;
    int num_of_rows;
} node;

typedef struct {
    node * head;
} linked_list;

/*
 * Free macro list
 * *head : head of macro list to free
 */
void free_macro_list(node * head);

/*
 * Add new node to macro list
 * *list : macro list to add to
 * *new_name : name of new node
 * **new_text : text of new node
 * number_of_rows : num_of_rows value for new node
 * Returns head of list with newly added now
 */
node * add_to_list(linked_list * list, char * new_name, char **new_text, int number_of_rows);

/*
 * Create an empty macro list
 * Returns newly created list
 */
linked_list * create_empty_list();

/*
 * Counts number of nodes in macro list, essentially counting number of macro definitions found
 * *list : macro list to count nodes in
 * Returns number of nodes in macro list
 */
int get_number_of_nodes(linked_list * list);