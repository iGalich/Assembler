typedef struct node {
    char * name;
    char **text;
    struct node * next;
} node;

typedef struct {
    node * head;
} linked_list;



node * add_to_list(linked_list * list, char * new_name, char **new_text);

linked_list * create_empty_list();

int get_number_of_nodes(linked_list * list);