#include "ray_glob.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "functions.h"
#include "globals.h"
#include "processor.h"
#include <math.h>
#include "unistd.h"

word __spu__eof(word arg1, word arg2, word arg3, Proccesor* proc){
    proc->state = PROC_EOF_INSTR;
    return 0;
}

word __spu__err(word arg1, word arg2, word arg3, Proccesor* proc){
    proc->state = PROC_ERR_INSTR;
    return 0;
}

word __spu__empty(word arg1, word arg2, word arg3, Proccesor* proc){
    proc->state = PROC_EMPTY_INSTR;
    return 0;
}

word __spu__nope(word arg1, word arg2, word arg3, Proccesor* proc){
    return 0;
}
char _is_invalid_jmp_location(word location, Proccesor* proc){
    if (location < 0 || (size_t)location >= proc->exe.meta.length) {
        proc->state = PROC_INVALID_JMP;
        return 1;
    }
    return 0;
}
word __spu__j_alw(word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg3, proc)) return 0;
    proc->state = PROC_JUMPED;
    proc->programm_counter = arg3;
    return 0;
}

word __spu__j_eq(word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg3, proc)) return 0;
    if (arg1 == arg2) {
        proc->state = PROC_JUMPED;
        proc->programm_counter = arg3;
    }
    return 0;
}

word __spu__j_gt(word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg3, proc)) return 0;
    if (arg1  > arg2) {
        proc->state = PROC_JUMPED;
        proc->programm_counter = arg3;
    }
    return 0;
}

word __spu__j_lt(word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg3, proc)) return 0;
    if (arg1  < arg2) {
        proc->state = PROC_JUMPED;
        proc->programm_counter = arg3;
    }
    return 0;
}

word __spu__j_ge(word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg3, proc)) return 0;
    if (arg1 >= arg2) {
        proc->state = PROC_JUMPED;
        proc->programm_counter = arg3;
    }
    return 0;
}

word __spu__j_le(word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg3, proc)) return 0;
    if (arg1 <= arg2) {
        proc->state = PROC_JUMPED;
        proc->programm_counter = arg3;
    }
    return 0;
}

word __spu__j_neq(word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg3, proc)) return 0;
    if (arg1 != arg2) {
        proc->state = PROC_JUMPED;
        proc->programm_counter = arg3;
    }
    return 0;
}


word __spu__call    (word arg1, word arg2, word arg3, Proccesor* proc){
    if (_is_invalid_jmp_location(arg1, proc)) return 0;

    stack_push(&proc->func_stack, proc->programm_counter+1);

    proc->state = PROC_JUMPED;
    proc->programm_counter = arg1;

    return 0;

}
word __spu__ret     (word arg1, word arg2, word arg3, Proccesor* proc){
    word ret_addr = stack_pop(&proc->func_stack);
    if (_is_invalid_jmp_location(ret_addr, proc)) return 0;

    proc->state = PROC_JUMPED;
    proc->programm_counter = ret_addr;

    return 0;

}
word __spu__halt    (word arg1, word arg2, word arg3, Proccesor* proc){
    sleep(1000000);
    return 0;
}

word __spu__move(word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1;
}

word __spu__add(word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1 + arg2;
}

word __spu__sub(word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1 - arg2;
}

word __spu__div(word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1 / arg2;
}

word __spu__mult(word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1*arg2;
}

word __spu__and     (word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1 & arg2;
}
word __spu__or      (word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1 | arg2;
}
word __spu__xor     (word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1 ^ arg2;
}

word __spu__mod     (word arg1, word arg2, word arg3, Proccesor* proc){
    return arg1 % arg2;
}


word __spu__sqrt(word arg1, word arg2, word arg3, Proccesor* proc){
    return (word)(sqrt((float)arg1 / FP_MULT) * FP_MULT);
}

word __spu__to_fp   (word arg1, word arg2, word arg3, Proccesor* proc){
    return (word)(((float)arg1) * FP_MULT);
}

word __spu__to_wh   (word arg1, word arg2, word arg3, Proccesor* proc){
    return (word)(((float)arg1) / FP_MULT);
}



word __spu__inp(word arg1, word arg2, word arg3, Proccesor* proc){
    word res = 0;
    printf("> ");
    int scaned = scanf("%d", &res);
    if (scaned != 1) {
        proc->state = PROC_INVALID_INPUT;
    }
    return res;
}

word __spu__pr_wh   (word arg1, word arg2, word arg3, Proccesor* proc){
    printf("%hd", arg1);
    return 0;
}
word __spu__pr_ch   (word arg1, word arg2, word arg3, Proccesor* proc){
    printf("%c", arg1);
    return 0;
}

word __spu__pr_fp   (word arg1, word arg2, word arg3, Proccesor* proc){
    printf("%f", (float)arg1 / FP_MULT); 
    return 0;
}

word __spu__init_screen   (word arg1, word arg2, word arg3, Proccesor* proc){
    if (proc->has_screen != 0) {
        proc->state = PROC_SCREEN_STATE_FAIL;
        return 0;
    }
    proc->has_screen = 1;
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "My First Raylib Game");
    SetTargetFPS(arg1); // Set a comfortable frame rate
    return 0;
}
word __spu__draw          (word arg1, word arg2, word arg3, Proccesor* proc){  
    if (proc->has_screen == 0) {
        proc->state = PROC_SCREEN_STATE_FAIL;
        return 0;
    }
    BeginDrawing();
        ClearBackground(BLACK);
        for (word mem_addr = 0; mem_addr < RAM_SIZE; mem_addr++){
            if (proc->ram[mem_addr] != 0){
                DrawRectangle(
                    (mem_addr % (SCR_WIDTH/PIXEL_SIZE))*PIXEL_SIZE, 
                    (mem_addr / (SCR_WIDTH/PIXEL_SIZE))*PIXEL_SIZE,
                    PIXEL_SIZE,
                    PIXEL_SIZE,
                    WHITE
                );
            }
        }
    EndDrawing();
    return 0;
}
word __spu__kill_screen   (word arg1, word arg2, word arg3, Proccesor* proc){
    if (proc->has_screen == 0){
        proc->state = PROC_SCREEN_STATE_FAIL;
        return 0;
    }
    proc->has_screen = 1;
    CloseWindow();
    return 0;
}
