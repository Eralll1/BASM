#include "../globals/globals.h"
#include "../outer/read_file.h"
#include "../globals/op_helper.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>


typedef enum {
    ASM_OK                  = 0,
    ASM_INVALID_COMMAND     = 1,
    ASM_INVALID_CHARACTER   = 2,
    ASM_INVALID_ARG         = 3,
    ASM_INVALID_JMP         = 4,
    ASM_LABLE_TOO_SHORT     = 5,
    ASM_TOKEN_TOO_LONG      = 6,
    ASM_TOO_MANY_LABELS     = 7,
    ASM_BAD_INSTR           = 8,
    ASM_ALLOC_FAIL          = 9,
    ASM_BAD_FILE            = 10,
    ASM_BAD_REGISTER        = 11,
    ASM_DUP_LABEL           = 12,
} ASM_ERROR;



typedef struct {
    char name[MAXLABELSIZE];
    word line;
} Label;

typedef enum {
    PR_NAME,
    PR_ARG1,
    PR_ARG2,
    PR_ARG3,
    PR_NOTHING,
} PR_STATE;


extern ASM_ERROR ERR_STATE;
extern size_t    ERR_LINE;
extern char*     ERR_LINE_TXT;

Instr parse_instr(char* line, Label labels[MAXLABELCOUNT], size_t line_ind);

// private maybe..? (static)
void cut_comments(parsedFile* input);
void fill_labels(parsedFile* input, Label* labels);
void check_and_init_consts();



Code code_fill(parsedFile* input);
void code_save_as_bin(Code to_save, FILE* output);
void code_free(Code* to_free);
Code code_create(char* input_filename);