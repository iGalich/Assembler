/* This linked list is used to store information about the labels encountered */

struct attributes {
    int external;
    int code;
    int entry;
    int data;
};

typedef struct symbol_node {
    char * symbol;
    int decimal_value;
    int base_address;
    int offset;
    struct attributes symbol_attributes;
    struct symbol_node * next;
} symbol_node;

typedef struct {
    symbol_node * head;
} symbol_linked_list;

/*
 * Free symbol list
 * *head : head of list
 */
void free_symbol_list(symbol_node * head);

/*
 * Adds a new symbol node to symbol list
 * *list : symbol list to add to
 * *new_symbol : name of new symbol
 * new_decimal_value : new decimal value of new node
 * new_base_address : new base address of new node
 * new_offset : new offset of new node
 * new_attributes : new attributes struct of new node
 * Returns head of list with newly added node
 */
symbol_node * add_to_symbol_list(symbol_linked_list * list, char * new_symbol, int new_decimal_value, int new_base_adress, int new_offset, struct attributes new_attributes);

/*
 * Creates an empty symbol list
 * Returns an empty symbol list
 */
symbol_linked_list * create_empty_symbol_list();

/*
 * Prints symbol list, with info about each node
 * NOTE : used for test purposes only
 * *list : symbol list to print
 */
void print_symbol_list(symbol_linked_list * list);

/*
 * Count nubmer of symbol nodes there are in the list
 * *list : the symbol list to count the nodes in
 * Returns number of nodes in list
 */
int get_number_of_symbol_nodes(symbol_linked_list * list);

/*
 * Finds symbol in list and changes its entry attribute to 1
 * *list : list to look for symbol in
 * *symbol_name : name of symbol to find
 * Returns 1 if symbol found, 0 otherwise
 */
int find_symbol_and_change_entry(symbol_linked_list * list, char * symbol_name);

/*
 * Finds symbol in list
 * *list : list to look for symbol in
 * *symbol_name : name of symbol to look for
 * Returns the symbol_node with name that was asked for
 * Returns NULL if wasn't found
 */
symbol_node * find_symbol_with_name(symbol_linked_list * list, char * symbol_name);