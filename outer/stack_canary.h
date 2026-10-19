#include "mylib.h"
#include "../globals/globals.h"

#ifndef ST_TYPE
#error "ST_TYPE should be defined"
#else
typedef ST_TYPE stack_t;
#endif


#define ST_STACKSIZE 5

#define CANARY_AMOUNT 2
#define CANARY_VAL 0xDED
#define DEFAULT_VAL -1

typedef enum {
    ST_OK = 0,
    ST_left_cn_damaged  = 1<<0,
    ST_right_cn_damaged = 1<<1,
    ST_invalid_cur_size = 1<<2,
    ST_invalid_max_size = 1<<3,
    ST_invalid_mem_ptr  = 1<<4,
    ST_NULL_mem_ptr     = 1<<5,
    ST_calloc_fail      = 1<<6,
    ST_invalid_stack    = 1<<7,
    ST_empty_pop        = 1<<8,
} Stack_Exceptions;

typedef struct {
    stack_t* mem_start;
    size_t cur_size;
    size_t max_size;
    int status;
} Stack;


void dump_stack(FILE* output, Stack* stack);

#define CHECK_STACK_RET(stack, ret) do {  \
    assert(stack != NULL); \
    stack->status |= _stack_validate(stack); \
    if (stack->status != ST_OK) {dump_stack(stderr, stack); return ret;} \
} while (0)

#define CHECK_STACK_NO_RET(stack) do {  \
    assert(stack != NULL); \
    stack->status |= _stack_validate(stack); \
    if (stack->status != ST_OK) {dump_stack(stderr, stack); return;} \
} while (0)



// Безобразно зато единообразно)

// TODO: push->stack_push et cetera      +
// TODO: Reorder functions               +
// TODO: implement stack_free            +
// TODO: mb create_stack -> stack_create +

int         _stack_validate       (Stack* stack);
stack_t*    _get_top_ptr          (Stack* stack);
stack_t*    _get_canary_at_end    (Stack* stack);

void        stack_create          (Stack* stack, size_t elem_count);
void        stack_resize          (Stack* stack, size_t new_size);
void        stack_push            (Stack* stack, stack_t elem);
stack_t     stack_topval          (Stack* stack);
stack_t     stack_pop             (Stack* stack);
void        stack_free            (Stack* stack);

void        stack_print           (Stack* stack, FILE* output);



