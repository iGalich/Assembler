typedef struct address_node {
    int address;
    struct address_node * next;
} address_node;

typedef struct {
    address_node * head;
} address_linked_list;

address_linked_list * create_empty_address_list();

address_node * add_to_address_list(address_linked_list * list, int new_address);

void print_address_list(address_linked_list * list);