#ifndef GLOBALS
#define GLOBALS

#include <stdint.h>

#include "../outer/mylib.h"


typedef uint8_t _flag_t;
typedef uint8_t  opcode_t;
typedef int32_t  word;

typedef struct {
    _flag_t arg1 :2;
    _flag_t arg2 :2;
    _flag_t arg3 :2;
} flags_t;

typedef enum {
    FLAG_NUM    = 0b00,
    FLAG_REG    = 0b01,
    FLAG_MEM    = 0b10,
    FLAG_STACK  = 0b11,
} FLAG;

#define MAXOPNAME 100
#define MAXARGLENGTH 20
#define MAXAGRC 3
#define MAXLABELCOUNT 20
#define MAXLABELSIZE 20
#define WRITECHUNK 1024

#define REGISTERCOUNT 8
#define FP_POS 4
#define FP_MULT (2<<FP_POS)
#define ST_TYPE word

typedef struct {
    flags_t flags;
    opcode_t opcode;
    word arg1;
    word arg2;
    word arg3;
} Instr;

typedef struct {
    size_t length;
    uint16_t unused2;
    uint16_t unused3;
    uint16_t unused4;
    uint16_t unused5;
    uint16_t unused6;
    uint16_t unused7;
} Header;

typedef struct {
    Instr* instructions;
    Header meta;
} Code;

#endif