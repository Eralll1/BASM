#include "asm.h"
#include "helper.h"


// Needed as asm.c is linked in better.c for code_free


int main(int argc, char* argv[]) {
    
    cmd_args args = parse_args(argc, argv);

    assert(args.input_filename != NULL);

    Code bytecode = code_create(args.input_filename);

    FILE* output = (args.output_filename == NULL) ? stdout : fopen(args.output_filename, "w");
    if (ERR_STATE != ASM_OK){
        if (ERR_LINE_TXT != NULL){
            printf("Some error: <%d> at line <%zu>:<%s>\n", ERR_STATE, ERR_LINE, ERR_LINE_TXT);
        } else {
            printf("Some error: <%d> at line <%zu>\n", ERR_STATE, ERR_LINE);
        }
        return 1;
    }
    
    code_save_as_bin(bytecode, output);

    code_free(&bytecode);
    if (args.output_filename != NULL) {
        fflush(output);
        fclose(output);
    }
    
    return 0;
    // TODO read \ write better

}
