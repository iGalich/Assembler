#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "linkedlist.h"
#include "symbollinkedlist.h"
#include "addresslinkedlist.h"

#define MAX_LENGTH 81 /* max amount of characters in a single line in the files, including '\n' character */

typedef struct word_without_operands {
    unsigned int opcode : 16;
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int msb : 1;
} word_without_operands ;

typedef struct word_with_operands {
    unsigned int destination_address_mode : 2;
    unsigned int destination_register : 4;
    unsigned int source_address_mode : 2;
    unsigned int source_register : 4;
    unsigned int function : 4;
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int msb : 1;

} word_with_operands;

enum registers {
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15
};

enum command_names {
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    JSR,
    RED,
    PRN,
    RTS,
    STOP
};

typedef struct data_node {
    int address;
    int operands_flag;
    struct word_with_operands word_with;
    struct word_without_operands word_without;
    struct data_node * next;
} data_node;

typedef struct {
    data_node * head;
} data_linked_list;

/*
 * Create an empty data linked list
 */
data_linked_list * create_empty_data_list();

/*
 * Prints data list with info about each node
 * *list : the data list to print
 */
void print_data_list(data_linked_list * list);

/*
 * Sorts a data list using bubble sort algorihtm.
 * *start : head of the data list
 */
void bubble_sort_data_list(data_node * start);

/*
 * Prints data list, with inforamtion about each node, and warns about gaps in addresses
 * NOTE : this function isn't used during the runtime of the program, it is for test purposes only
 * *list : the data list to print
 */
void print_all_address(data_linked_list * list);

/*
 * Swap places of two data nodes in data list
 * No checking is done to check if nodes are within the same list
 * *a : first node
 * *b : second node
 */
void swap_data_nodes(data_node * a, data_node * b);

/*
 * Power calculation
 * NOTE : this is used instad of pow in math.h due to a bug I wasn't able to fix
 * base : base of power function
 * power : the power of power function
 */
int power(int base, int power);

/*
 * Resets value of a word without opernads
 * *word : the word without opernads to reset values in
 */
void reset_word_without(struct word_without_operands * word);

/*
 * Create object file
 * *list : the data list used to create the file
 */
void create_object_file(data_linked_list * list);

/*
 * Counts the total number of words in a given file
 * *file : the file to count words in
 * Returns amount of words in file
 */
int count_number_of_words_in_file(FILE * file);

/*
 * Converts an unsigned int to an array of ones and zero, representing a binary number
 */
void int_to_binary(unsigned int in, int count, int * out);

/*
 * Converts binary number (represented as an array of ones and zeros) to a decimal number
 * NOTE : this function only converts 4 binary digits
 * *arr : the binary number respresented as an array
 * Returns the decimal value
 */
int binary_to_decimal(int * arr);

/*
 * Converts the inforamtion inside of address list into new data for data list
 * *address_list : the address list to convert information from
 * *data_list : the data list to convert infromation to
 * *symbol_list : symbol list used during the conversion
 */
void convert_address_to_data(address_linked_list * address_list, data_linked_list * data_list, symbol_linked_list * symbol_list);

/*
 * Adds a new node to data list
 * *list : the list to add the node to
 * address : the address value for the new node
 * flag : operands flag value for new node
 * word_with_operands : word_with for new node
 * word_without_operands : with_without for new node
 * Returns the head of list with newly added node
 */
data_node * add_to_data_list(data_linked_list * list, int adress, int flag, word_with_operands, word_without_operands);

/*
 * This function looks in the folder for the file with the filename with the file extension .as
 * *filename : the filename we're searching for
 */
void look_for_file(char * filename);

/*
 * This function looks for the macro keyword in the file, and in the case if does find it, looks also for the keyword endm.
 */
void check_macro(void);

/* Duplicates file stored in fd1 into fd2
 * *filename : the name of the file that will be duplicated
 */
void duplicate_file(char * filename);

/*
 * This function takes a string and returns a char pointer to the first string character after all white space at the start of the string
 * Returns a char pointer to the first string character after all white space
 * input[] : the string where it shall skip the white space at the start
 */
char *skip_white_space_at_start(char input[]);

/*
 * This function takes a string and removes all white spaces from the entire string, leaving only regular characters
 * input[] : the string where all white space will be deleted
 */
void delete_spaces(char input[]);

/*
 * This function is used after a macro keyword has been found. It goes over the .am file and replaces every appearance of the found keyword to the proper macro.
 * *list : a list containing the macro which will be put in the .am file
 * delete_line : the number of the line which contains the macro keyword
 */
void replace_macro(linked_list * list, int delete_line, int last_line);

/*
 * This function looks for instances of the  macro keyword in the file text.
 * *list : a list containing the macro which will be put in the .am file
 */
void find_macro_instances(linked_list * list);

/*
 * Create a copy of a file
 * *target : the file to copy to
 * *source : the file to copy from
 */
void create_copy(FILE * target, FILE * source);

/*
 * Free data list
 * *head : head of data list
 */
void free_data_list(data_node * head);

int count_number_of_lines_in_file(FILE * file);

/*
 * Write into post macro file with new macro definition from macro list
 * *list : list of macro keywords
 */
void macro_pass(linked_list * list);

/*
 * First pass of the algorithm
 */
void first_pass();

/*
 * With a given decimal number, calculate base address
 * num : the decimal value
 * Returns base address value
 */
int calculate_base_adress(int num);

/*
 * Resets values of an attributes struct
 * *label_attribures : the attributes struct to reset values in
 */
void reset_attributes(struct attributes * label_attributes);

/*
 * Create a .am file using the given filename
 * *filename : name of file
 */
void create_post_macro_file(char * filename);

/*
 * Removes first n characters from a given string
 * *str : string to remove characters from
 * n : number of characters to remove
 * Returns pointer to start of chopped string
 */
char * chop_first_n_characters(char * str, size_t n);

/*
 * Copies one string to another
 * Safer version of the function 'strcpy' in string.h
 * NOTE : this function is used instead of strcpy since many problems were encountered while trying to use strcpy
 */
size_t strlcpy(char *, const char *, size_t);

/*
 * Returns amount of words in a given string
 * *string : string to check amount of words in
 * Returns amount of words in the string
 */
int count_words_in_lines(char * string);

/*
 * Checks index of command in a string
 * *command : the string to find comannd index in
 * Returns index of command, -1 if not present.
 */
int get_command_index(char * command);

/*
 * Checks the index of a register in a string
 * *string : the string where the register index will be checked
 * Returns the index of the register.
 * If register isn't present, returns -1
 */
int get_register_index(char * string);

/*
 * Check which address mode we are in
 * *string : string to check in
 * index : number of register if present
 * Returns an address mode between 0 to 3
 */
int get_address_mode(char * string, int index);

/*
 * Create the entry file
 * *list : symbol list used to create file
 */
void create_entry_file(symbol_linked_list * list);

/*
 * Create the external file
 * *symbol_list : symbol list used to create the file
 * *address_list : address list used to create the file
 */
void create_external_file(symbol_linked_list * symbol_list, address_linked_list * address_list);

/*
 * Second pass of algorithm
 * *symbol_list : symbol list used in the algorithm
 * *data_list : data list used in the algorithm
 * *address_list : address list used in the algorithm
 */
void second_pass(symbol_linked_list * symbol_list, data_linked_list * data_list, address_linked_list * address_list);

/*
 * Used to reset values of words with operands and without
 * *word_with : the word with operands to reset
 * *word_without : the word without operands to reset
 */
void reset_words(struct word_with_operands * word_with, struct word_without_operands * word_without);

/*
 * Final stage of the program: creates external, entry and object files
 * *data_list : data list used to create files
 * *symbol_list : symbol list used to create files
 * *address_list : address list used to create files
 */
void build_final_files(data_linked_list * data_list, symbol_linked_list * symbol_list, address_linked_list * address_list);