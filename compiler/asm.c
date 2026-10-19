#include "asm.h"
#include "helper.h"

ASM_ERROR ERR_STATE      = ASM_OK;
size_t    ERR_LINE       = 0;
char*     ERR_LINE_TXT   = NULL;

static Instr  err_res_inst      = {};
static Instr  empty_res_inst    = {};
static Instr  end_instr         = {};

void fill_labels(parsedFile* input, Label* labels){

    size_t table_ind    = 0;
    char   has_label    = 0;
    char   cur_symb     = 0;
    size_t in_lbl_ind   = 0;
    size_t instr_count  = 0;

    for (size_t line_ind = 0; line_ind < input->line_amount; line_ind++) {
        has_label   = 0;
        in_lbl_ind  = 0;
        size_t line_length = input->pointers[line_ind].end - input->pointers[line_ind].start + 1;
        for (size_t symb_ind = 0; symb_ind < line_length; symb_ind++){
            cur_symb = input->pointers[line_ind].start[symb_ind];

            if (cur_symb == '#' || cur_symb == ';') break;

            if (cur_symb == ' ' && !has_label) continue;
            if (cur_symb == ' ' &&  has_label) break;
            if (cur_symb == '\0')              break;

            if (cur_symb == '$' && !has_label) {
                has_label = 1;
                continue;
            }

            if (can_be_name(cur_symb) && !has_label) {
                instr_count++;
                break;
            }
            if (can_be_name(cur_symb) &&  has_label){
                labels[table_ind].name[in_lbl_ind] = cur_symb;
                labels[table_ind].line             = instr_count;
                in_lbl_ind++;
                if (in_lbl_ind >= MAXARGLENGTH) {
                    ERR_LINE = line_ind;
                    ERR_STATE = ASM_TOKEN_TOO_LONG;
                    return;
                }
                continue;
            }
            
            ERR_LINE = line_ind;
            ERR_STATE = ASM_INVALID_CHARACTER;
            return;
        }
        if (has_label){
            if (strlen(labels[table_ind].name) < 1){
                ERR_LINE = line_ind;
                ERR_STATE = ASM_LABLE_TOO_SHORT;
                return;
            }
            table_ind++;
            if (table_ind >= MAXLABELCOUNT){
                ERR_LINE = line_ind;
                ERR_STATE = ASM_TOO_MANY_LABELS;
                return;
            }
        }
    }
}

word _resolve_label(char* dest,  Label labels[MAXLABELCOUNT]){

    for(size_t search_label = 0; search_label < MAXLABELCOUNT; search_label++){
        if (strcmp(dest, labels[search_label].name) == 0){
            return labels[search_label].line;
        }
    }
    ERR_STATE = ASM_INVALID_JMP;
    return -1;
}

word _resolve_register(char* name){
    if (strcmp(name, "ax") == 0){
        return 1;
    }
    if (strcmp(name, "bx") == 0){
        return 2;
    }
    if (strcmp(name, "cx") == 0){
        return 3;
    }
    if (strcmp(name, "dx") == 0){
        return 4;
    }
    if (strcmp(name, "ex") == 0){
        return 5;
    }
    if (strcmp(name, "fx") == 0){
        return 6;
    }
    if (strcmp(name, "gx") == 0){
        return 7;
    }

    ERR_STATE = ASM_BAD_REGISTER;
    return 0;
}

word _resolve_char(char* text){
    if (text[0] != '/') return (word)text[0];

    switch (text[1]) {
        case 'n': return '\n';
        case '0': return '\0';
        case '/': return '/' ;
        case '"': return '"';
        default:  return '\0';
    }
}

FLAG _resolve_arg(word* arg_val, char* arg, Label labels[MAXLABELCOUNT]){

    switch (arg[0]) {
        case '%':
            *arg_val = _resolve_register(arg+1);
            return FLAG_REG;
        case '&':
            *arg_val = _resolve_register(arg+1);
            return FLAG_MEM;
        case 'P':
            *arg_val = txt2word(arg+1);
            return FLAG_STACK;
        case '$':
            *arg_val = _resolve_label(arg+1, labels);
            return FLAG_NUM;
        case '"':
            *arg_val = _resolve_char(arg+1);
            return FLAG_NUM;
        case '-':
        default:
            *arg_val = txt2word(arg);
            return FLAG_NUM;
    }
}

void check_and_init_consts(){
    if (err_res_inst.opcode == 0){
        err_res_inst.opcode =  get_code_by_name("ERR");
    }
    if (empty_res_inst.opcode == 0){
        empty_res_inst.opcode =  get_code_by_name("ERR");
    }
    // do not check end_instr as it is always 0
    // if (empty_res_inst.op == 0){
    //     empty_res_inst.op =  get_code_by_name("ERR");
    // }
}
/*
// works because of substitution rules in macros
#define STR(s) #s
#define STRX(s) STR(s)

// no quotes support
Instr parse_instr_sscanfed(char* line, Label labels[MAXLABELCOUNT], size_t line_ind){
    char name[MAXOPNAME+1]    = {}; // +1 to avoid overflows
    char arg1[MAXARGLENGTH+1] = {};
    char arg2[MAXARGLENGTH+1] = {};
    char arg3[MAXARGLENGTH+1] = {};

    const static char format[] = "%" STRX(MAXOPNAME) "s %" STRX(MAXARGLENGTH) "s %" STRX(MAXARGLENGTH) "s %" STRX(MAXARGLENGTH) "s";

    int scaned = sscanf(line, format, name, arg1, arg2, arg3);

    if (scaned == 0)
        return empty_res_inst;
    if (scaned == 1 && name[0] == '$') // label
        return empty_res_inst;
    

    Instr real_res = {
        .opcode = get_code_by_name(name),
    };

    if (real_res.opcode == err_res_inst.opcode){
        ERR_STATE = ASM_INVALID_COMMAND;
        ERR_LINE = line_ind;
        return err_res_inst;
    }

    real_res.flags.arg1 = _resolve_arg(&real_res.arg1, arg1, labels);
    real_res.flags.arg2 = _resolve_arg(&real_res.arg2, arg2, labels);
    real_res.flags.arg3 = _resolve_arg(&real_res.arg3, arg3, labels);

    if (ERR_STATE != ASM_OK){
        ERR_LINE = line_ind;
        return err_res_inst;
    }

    return real_res;

    
}
*/
Instr parse_instr(char* line, Label labels[MAXLABELCOUNT], size_t line_ind){

    char name[MAXOPNAME]    = {};
    char arg1[MAXARGLENGTH] = {};
    char arg2[MAXARGLENGTH] = {};
    char arg3[MAXARGLENGTH] = {};
    
    size_t buffer_ind       = 0;
    PR_STATE parse_step     = PR_NAME;
    char is_in_quotes       = 0;


    if (is_all(line, ' ')) return empty_res_inst; // skip blank lines

    for (size_t ind = 0; line[ind] != '\0' && line[ind] != '\n'; ind++){

        if (line[ind] == '"' && (ind == 0 || line[ind-1] != '/'))
            is_in_quotes = !is_in_quotes;

        if (line[ind] == ' ' && !is_in_quotes){
            // if prev was not ' ' - enter new state
            if (ind > 0 && line[ind-1] != ' '){
                buffer_ind = 0;
                parse_step++;
            }
            continue;
        }

        if (line[ind] == '$' && parse_step == PR_NAME && buffer_ind == 0) {
            return empty_res_inst;
        }

        if (can_be_name(line[ind]) || is_in_quotes || line[ind] == '"'){
            if ((parse_step == PR_NAME && buffer_ind > MAXOPNAME) || (parse_step != PR_NAME && buffer_ind > MAXARGLENGTH)){
                ERR_STATE = ASM_TOKEN_TOO_LONG;
                return err_res_inst;

            }
                
            switch (parse_step) {
                case PR_NAME:
                    name[buffer_ind] = line[ind];
                    buffer_ind++;
                    break;
                case PR_ARG1:
                    arg1[buffer_ind] = line[ind];
                    buffer_ind++;
                    break;
                case PR_ARG2:
                    arg2[buffer_ind] = line[ind];
                    buffer_ind++;
                    break;
                case PR_ARG3:
                    arg3[buffer_ind] = line[ind];
                    buffer_ind++;
                    break;
                case PR_NOTHING:
                    ERR_LINE = line_ind;
                    ERR_STATE = ASM_INVALID_COMMAND;
                    return err_res_inst;

            }
            continue;
        }

        
        ERR_LINE = line_ind;
        ERR_STATE = ASM_INVALID_CHARACTER;
        return err_res_inst;
    }

    if (name[0] == '\0') return empty_res_inst;

    Instr real_res = {
        .opcode = get_code_by_name(name),
    };

    if (real_res.opcode == err_res_inst.opcode){
        ERR_STATE = ASM_INVALID_COMMAND;
        ERR_LINE = line_ind;
        return err_res_inst;
    }

    real_res.flags.arg1 = _resolve_arg(&real_res.arg1, arg1, labels);
    real_res.flags.arg2 = _resolve_arg(&real_res.arg2, arg2, labels);
    real_res.flags.arg3 = _resolve_arg(&real_res.arg3, arg3, labels);

    if (ERR_STATE != ASM_OK){
        ERR_LINE = line_ind;
        return err_res_inst;
    }

    return real_res;
}


void _check_dup_labels(Label labels[MAXLABELCOUNT]){

    size_t label_count = 0;

    for (size_t label_ind = 0; label_ind < MAXLABELCOUNT; label_ind++){
        if (labels[label_ind].name[0] != '\0'){
            label_count++;
        } else{
            break;
        }
    }

    for (size_t ind1 = 0; ind1 < label_count; ind1++) {
        for (size_t ind2; ind2 < label_count; ind2++) {
            if (ind1 == ind2) continue;
            if (strcmp(labels[ind1].name, labels[ind2].name) == 0){
                ERR_STATE = ASM_DUP_LABEL;
                return;
            }
        }
    }
}

void cut_comments(parsedFile* input){

    for (size_t line_ind = 0; line_ind < input->line_amount; line_ind++){

        size_t length = input->pointers[line_ind].end - input->pointers[line_ind].start + 1; 
        char is_in_quotes = 0;

        for (size_t char_ind = 0; char_ind < length; char_ind++){

            char* char_ptr = input->pointers[line_ind].start + char_ind;

            if (*char_ptr == '"'){
                is_in_quotes = !is_in_quotes;
            }

            if (!is_in_quotes && (*char_ptr == '#' || *char_ptr == ';')){
                *char_ptr = '\0';
                if (char_ind == 0){
                    input->pointers[line_ind].end = char_ptr;
                } else {
                    input->pointers[line_ind].end = char_ptr-1;
                }
                break;
            }
        }
    }
}

Code code_fill(parsedFile* input){

    check_and_init_consts();

    static Code err_res = {};

    Label   labels[MAXLABELCOUNT] = {};
    Instr*  instructions = calloc(input->line_amount, sizeof(Instr));
    if (instructions == NULL) {
        ERR_STATE = ASM_ALLOC_FAIL;
        return err_res;
    }

    Instr   next = {};
    size_t  instr_ind = 0;

    cut_comments(input);
    fill_labels(input, labels);
    _check_dup_labels(labels);

    if (ERR_STATE != ASM_OK){
        return err_res;
    }

    for (size_t line_ind = 0; line_ind < input->line_amount; line_ind++) {
        
        next = parse_instr(input->pointers[line_ind].start, labels, instr_ind);
        if (ERR_STATE != ASM_OK) {
            ERR_LINE = line_ind;
            return err_res;
        }

        if        (next.opcode == empty_res_inst.opcode){
            continue;
        } else if (next.opcode == err_res_inst  .opcode){
            if (ERR_STATE == ASM_OK) {
                ERR_STATE = ASM_BAD_INSTR;
                ERR_LINE = line_ind;
            } // do not overwrite ERR_STATE
            return err_res;
        }

        instructions[instr_ind] = next;
        instr_ind++;
    }
    // resize to remove all empty bytes
    instructions = realloc(instructions, instr_ind*sizeof(Instr));
    if (instructions == NULL) {
        ERR_STATE = ASM_ALLOC_FAIL;
        return err_res;
    }
    Header meta = {.length = instr_ind};
    Code res = {
        .instructions = instructions,
        .meta = meta,
    };

    return res;
}

void code_save_as_bin(Code to_save, FILE* output){

    // TODO write some kind of proper header
    fwrite(&to_save.meta, sizeof(to_save.meta), 1, output);

    check_and_init_consts();

    fwrite(to_save.instructions, sizeof(to_save.instructions[0]), to_save.meta.length, output);
    fwrite(&end_instr, sizeof(end_instr), 1, output);
    
}

void code_free(Code* to_free){
    free(to_free->instructions);
}

Code code_create(char* input_filename){

    Code err_res = {};

    parsedFile input = parseFile(input_filename);
    if (input.state != FILE_OK){
        ERR_STATE = ASM_BAD_FILE;
        return err_res;
    }

    Code bytecode = code_fill(&input);
    destroy_parsed_file(&input);

    return bytecode;
}

// TODO refactor asm / asm_main +

