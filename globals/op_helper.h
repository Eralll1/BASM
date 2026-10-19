#ifndef OPERATIONS_GR
#define OPERATIONS_GR

#include "globals.h"
#include "processor.h"

typedef struct {
    const word  opcode;
    const char* name;
} opcode_map_obj;

extern size_t         const OP_CODE_COUNT;
extern opcode_map_obj const OP_NAME_MAP[];
extern func_ptr_t     const OP_CODE_FUNC_MAP[];

opcode_t get_code_by_name(char* name);

#endif




