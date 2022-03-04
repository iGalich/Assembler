typedef struct node {
    void * data;
    struct node * next;
} node;

typedef struct {
    node * head;
} linked_list;

/*
 *
 */
void print_list(linked_list * list);

node * add_to_list(linked_list * list, void * data);

linked_list * create_empty_list();

int get_number_of_nodes(linked_list * list);

void print_node(node * node_name);