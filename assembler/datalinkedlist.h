typedef struct data_node {
    int address;
    int operands_flag;
    struct word_with_operands * word_with;
    struct word_without_operands * word_without;
    struct data_node * next;
} data_node;

typedef struct {
    data_node * head;
} data_linked_list;

data_linked_list * create_empty_data_list();