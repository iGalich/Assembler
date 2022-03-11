typedef struct data_node {
    int address;
    struct data_node * next;
} data_node;

typedef struct {
    data_node * head;
} data_linked_list;

data_linked_list * create_empty_data_list();