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

symbol_node * add_to_symbol_list(symbol_linked_list * list, char * new_symbol, int new_decimal_value, int new_base_adress, int new_offset, struct attributes new_attributes);

symbol_linked_list * create_empty_symbol_list();

void print_symbol_list(symbol_linked_list * list);

int get_number_of_symbol_nodes(symbol_linked_list * list);

void update_data_symbols(symbol_linked_list * list, int L);

int find_symbol_and_change_entry(symbol_linked_list * list, char * symbol_name);

symbol_node * find_symbol_with_name(symbol_linked_list * list, char * symbol_name);