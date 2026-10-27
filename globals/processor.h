#ifndef PROCESSOR_GR
#define PROCESSOR_GR

#include "../outer/stack_canary.h"
#include "ray_glob.h"

#include <stdlib.h>

typedef enum {
    PROC_OK                 = 0,
    PROC_EOF_INSTR          = 1,
    PROC_ERR_INSTR          = 2,
    PROC_EMPTY_INSTR        = 3,
    PROC_JUMPED             = 4,
    PROC_NOT_IMPLEMENTED    = 5,
    // PROC_INPUT              = 6,
    // PROC_OUTPUT             = 7,
    PROC_INVALID_REG        = 8,
    PROC_INVALID_OPCODE     = 9,
    PROC_FILE_OPEN_FAIL     = 10,
    PROC_INVALID_BIN        = 11,
    PROC_CALLOC_FAIL        = 12,
    PROC_BIN_TOO_SHORT      = 13,
    PROC_FILE_CLOSE_FAIL    = 14,
    PROC_INVALID_JMP        = 15,
    // PROC_OUTPUT_CHAR        = 16,
    // PROC_OUTCALL            = 17,
    PROC_INVALID_INPUT      = 18,
    PROC_RAM_OVERFLOW       = 19,
    PROC_RAM_UNDERFLOW      = 20,
    PROC_INVALID_FLAG       = 21,
    PROC_SCREEN_STATE_FAIL  = 22,
    PROC_NEGATIVE_SIZE      = 32,
} PROC_STATE;

typedef struct {
    Stack data_stack;
    Stack return_stack;
    word registers[REGISTERCOUNT];
    Code exe;
    size_t programm_counter;
    PROC_STATE state;
    word ram[RAM_SIZE];
    char has_screen;
} Proccesor;


typedef word(* func_ptr_t)(word, word, word, Proccesor*);

#endif 