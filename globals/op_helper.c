#include "op_helper.h"
#include "globals.h"
#include <string.h>
// #include <raylib.h>
// #include "raylib.h"
opcode_t get_code_by_name(char* name){
    size_t ind = 0;
    
    while (ind < OP_CODE_COUNT 
        && strcmp(OP_NAME_MAP[ind].name, name) != 0) {
            ind++;
    }
    if (ind == OP_CODE_COUNT){
        return get_code_by_name("ERR");
    }
    return OP_NAME_MAP[ind].opcode;
}