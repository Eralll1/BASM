#include "../globals/processor.h"
#include "../globals/op_helper.h"
#include "../compiler/helper.h"
#include "../compiler/asm.h"


Proccesor   processor_create    ();
void        processor_mainloop  (Proccesor* proc);
void        processor_free      (Proccesor* proc);
void        processor_instr     (Proccesor* proc);

Code        read_bin            (Proccesor* proc, cmd_args args);
word        get_value           (Proccesor* proc, FLAG flag, word arg_raw, char nondestr);


int main(int argc, char* argv[]){

    cmd_args args = parse_args(argc, argv);
    assert(args.input_filename != NULL);

    Proccesor pr = processor_create();
    pr.exe = read_bin(&pr, args);

    if (pr.state != PROC_OK){
        printf("File read failed with code: %d", pr.state);
        return 1;
    }

    // TODO move while to another function +
    processor_mainloop(&pr);
    

    processor_free(&pr);

}

void processor_mainloop(Proccesor* proc){
    while (proc->programm_counter < proc->exe.meta.length) {

        processor_instr(proc);
        
        switch (proc->state) {
            case PROC_EMPTY_INSTR:
                proc->state = PROC_OK;
            case PROC_OK:
                break;
            case PROC_EOF_INSTR:
                return;
            case PROC_JUMPED: 
                break;
            default:
                printf("Proccessor failed with error < %d > at line < %zu >\n", proc->state, proc->programm_counter);
                return;
        }

        if (proc->state == PROC_JUMPED){
            proc->state = PROC_OK;
            continue;
        }

        proc->programm_counter++;
    }
    return;
}

word get_value(Proccesor* proc, FLAG flag, word arg_raw, char nondestr){
    switch (flag) {
        case FLAG_NUM:
            return arg_raw;
        case FLAG_REG:
            if (arg_raw >= REGISTERCOUNT || arg_raw < 0){
                proc->state = PROC_INVALID_REG;
                return 0;
            }
            return proc->registers[arg_raw];
        case FLAG_MEM:
            if (arg_raw >= REGISTERCOUNT || arg_raw < 0){
                proc->state = PROC_INVALID_REG;
                return 0;
            }
            word addr = proc->registers[arg_raw];
            if (addr >= RAM_SIZE){
                proc->state = PROC_RAM_OVERFLOW;
                return 0;
            } else if (addr < 0){
                proc->state = PROC_RAM_UNDERFLOW;
                return 0;
            }

            return proc->ram[addr];
        case FLAG_STACK:
            if (nondestr) { return stack_topval(&proc->data_stack); }
            else          { return stack_pop   (&proc->data_stack); }
        default:
            proc->state = PROC_INVALID_FLAG;
            return 0;
    }
}


void processor_instr(Proccesor* proc){
    
    Instr instruction = proc->exe.instructions[proc->programm_counter];

    word arg1_val = get_value(
        proc,
        instruction.flags.arg1, 
        instruction.arg1,
        0
    );

    word arg2_val = get_value(
        proc,
        instruction.flags.arg2, 
        instruction.arg2,
        0
    );

    word arg3_val = get_value(
        proc,
        instruction.flags.arg3,
        instruction.arg3,
        1        
    );

    if (proc->state != PROC_OK) return;

    if (OP_CODE_FUNC_MAP[instruction.opcode] == NULL){
        proc->state = PROC_INVALID_OPCODE;
        return;
    }

    word resualt = OP_CODE_FUNC_MAP[instruction.opcode](
        arg1_val, 
        arg2_val, 
        arg3_val,
        proc
    );

    if(proc->state == PROC_JUMPED)
        return;

    switch (instruction.flags.arg3) {
        case FLAG_NUM:
            break;
        case FLAG_REG:
            if (instruction.arg3 >= REGISTERCOUNT || instruction.arg3 < 0){
                proc->state = PROC_INVALID_REG;
            } else {
                proc->registers[instruction.arg3] = resualt;
            }
            break;
        case FLAG_STACK:
            stack_push(&proc->data_stack, resualt);
            break;
        case FLAG_MEM:
            if (instruction.arg3 >= REGISTERCOUNT || instruction.arg3 < 0){
                proc->state = PROC_INVALID_REG;
            }
            word addr = proc->registers[instruction.arg3];
            if (addr >= RAM_SIZE){
                proc->state = PROC_RAM_OVERFLOW;
            } else if (addr < 0){
                proc->state = PROC_RAM_UNDERFLOW;
            } else {
                proc->ram[addr] = resualt;
            }
            break;
    }

    return;

}

Code read_bin(Proccesor* proc, cmd_args args){

    static Code err_res = {};

    FILE* fp = fopen(args.input_filename, "rb");
    if(fp == NULL){
        proc->state = PROC_FILE_OPEN_FAIL;
        return err_res;
    }

    Header header = {};
    if (fread(&header, sizeof(header), 1, fp) != 1){
        proc->state = PROC_INVALID_BIN;
        return err_res;
    }

    Instr* instructions = calloc(header.length, sizeof(Instr));
    if (instructions == NULL){
        proc->state = PROC_CALLOC_FAIL;
        return err_res;
    }
    if (fread(instructions, sizeof(Instr), header.length, fp) != header.length){
        proc->state = PROC_BIN_TOO_SHORT;
        return err_res;
    }

    if (fclose(fp) == EOF){
        proc->state = PROC_FILE_CLOSE_FAIL;
        return err_res;
    }

    Code res = {
        .instructions = instructions,
        .meta = header,
    };

    return res;
}

Proccesor processor_create(){

    Stack data_stack = {};
    stack_create(&data_stack, 8);

    Stack func_stack = {};
    stack_create(&func_stack, 8);

    Proccesor res = {
        .data_stack = data_stack,
        .func_stack = func_stack,
        .exe = {},
        .registers = {},
        .programm_counter = 0,
    };

    return res;
}

void processor_free(Proccesor* processor){
    stack_free(&processor->data_stack);
    stack_free(&processor->func_stack);
    code_free(&processor->exe);
}

