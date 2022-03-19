/* This linked list is used to save information about missing words after the first pass */

typedef struct address_node {
    int address;
    int next_address;
    char * string;
    struct address_node * next;
} address_node;

typedef struct {
    address_node * head;
} address_linked_list;

/* Create an empty address linked list */
address_linked_list * create_empty_address_list();

/*
 * This function adds a new node to the list
 * *list : the address list to add the node to
 * new_address : the address value of the new node added
 * next_address : the next_address value of the new node added
 * *string : the name of the new node
 */
address_node * add_to_address_list(address_linked_list * list, int new_address, int next_address, char * string);

/*
 * This function prints the list, with information about each node
 * NOTE : this function isn't used in the program, it is for test purposes only
 * *list : the list to print
 */
void print_address_list(address_linked_list * list);

/*
 * Free address list
 * *head : head of list
 */
void free_address_list(address_node * head);