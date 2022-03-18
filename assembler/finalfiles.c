#include "assembler.h"

extern int external_flag;
extern int entry_flag;
extern int ICF;
extern int DCF;

extern char * global_filename;

void build_final_files(data_linked_list * data_list)
{
    FILE * object_file; /* .ob */
    FILE * external_file; /* .ext */
    FILE * entry_file; /* .ent */

    if (external_flag)
    {
        strcat(global_filename, ".ext");
        if (!(external_file = fopen(global_filename, "w+")))
        {
            fprintf(stderr, "cannot open external file\n");
            exit(0);
        }
        global_filename[strlen(global_filename) - 4] = '\0';
    }
    if (entry_flag)
    {
        strcat(global_filename, ".ent");
        if (!(entry_file = fopen(global_filename, "w+")))
        {
            fprintf(stderr, "cannot open entry file\n");
            exit(0);
        }
        global_filename[strlen(global_filename) - 4] = '\0';
    }

    print_all_address(data_list);

    
}