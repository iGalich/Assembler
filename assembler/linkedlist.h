typedef struct node {
    char * name;
    char **text;
    struct node * next;
    int num_of_rows;
} node;

typedef struct {
    node * head;
} linked_list;



node * add_to_list(linked_list * list, char * new_name, char **new_text, int number_of_rows);

linked_list * create_empty_list();

int get_number_of_nodes(linked_list * list);